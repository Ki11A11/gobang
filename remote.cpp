#include "remote.h"
#include <QtNetwork>
#include <QMessageBox>
CommunicationManager::CommunicationManager(){
    RequestInvoker* requestInvoker = new RequestInvoker;
    requestInvoker->moveToThread(&requestThread);
    RequestHandler* requestHandler = new RequestHandler;
    requestHandler->moveToThread(&listenThread);

    connect(&listenThread,&QThread::finished,requestHandler,&QObject::deleteLater);
    connect(this,SIGNAL(listen_Remote()),requestHandler,SLOT(Listen()));
    connect(requestHandler,SIGNAL(return_Request(QString)),this,SLOT(handle_Request(QString)));

    connect(&requestThread,&QThread::finished,requestInvoker,&QObject::deleteLater);
    connect(this,SIGNAL(request_Remote()),requestInvoker,SLOT(Request()));
    connect(requestInvoker,SIGNAL(return_Response(QString)),this,SLOT(handle_Response(QString)));
    connect(requestInvoker,SIGNAL(listen_Remote()),this,SIGNAL(listen_Remote()));
    requestThread.start();
    listenThread.start();
}

CommunicationManager::~CommunicationManager(){
    requestThread.quit();
    requestThread.wait();
    listenThread.quit();
    listenThread.wait();
}

void CommunicationManager::handle_Response(QString msg){
    qDebug()<<msg;

}
void CommunicationManager::handle_Request(QString msg){
    qDebug()<<msg;
}

RequestInvoker::RequestInvoker(){
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QIODevice::readyRead, this, &RequestInvoker::readRemote);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
            this, &RequestInvoker::displayError);
    connect(tcpSocket,&QTcpSocket::connected,this,&RequestInvoker::socketConnected);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();
        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &RequestInvoker::sessionOpened);
        networkSession->open();
    }

}

RequestInvoker::~RequestInvoker(){
    tcpSocket->abort();
    tcpSocket->close();
}

void RequestInvoker::socketConnected(){
    QString requestStr = "GET /start HTTP/1.1\r\nhost: 120.25.61.118:5000\r\n\r\n";
    QString teststr= "1$Test";
    QByteArray ba;
    ba.append(teststr);
    tcpSocket->write(ba);
}

void RequestInvoker::sessionOpened(){
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

void RequestInvoker::readRemote(){
    quint16 count = tcpSocket->bytesAvailable();
    QByteArray ba = tcpSocket->readAll();
    QString dataStr(ba);
    qDebug()<<dataStr.split('$')[1];
}

void RequestInvoker::displayError(QAbstractSocket::SocketError socketError){
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"the host was not found, plz check the host name and port settings.";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<"the connection was refused by the peer, Make sure the server is running.";
        break;
    default:
        qDebug()<<tcpSocket->errorString();
    }
}


void RequestInvoker::Request(){

//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replayFinished(QNetworkReply*)));

//    manager->get(QNetworkRequest(QUrl("http://120.25.61.118/start")));

    tcpSocket->abort();
    tcpSocket->connectToHost("localhost",5000);
//    tcpSocket->connectToHost("120.25.61.118",
//                                5000);
}

void RequestInvoker::replayFinished(QNetworkReply* reply){
    if(reply->error()==QNetworkReply::NoError){
        QByteArray response = reply->readAll();
        QString s(response);
        emit return_Response(s);
    }else{
        emit return_Response(reply->errorString());
    }
}


RequestHandler::RequestHandler(){
    Listen();
}

RequestHandler::~RequestHandler(){
    tcpServer->disconnect();
    tcpServer->close();
}

void RequestHandler::Listen(){
    tcpServer = new QTcpServer(this);
    connect(tcpServer,&QTcpServer::newConnection,this,&RequestHandler::newConnection);
    if(!tcpServer->listen(QHostAddress::Any,1234)){
        qDebug()<<"Server could not start.";
    }
}

void RequestHandler::newConnection(){
    QTcpSocket *client = tcpServer->nextPendingConnection();
//    quint16 count = client->bytesAvailable();
//    QByteArray ba = client->readAll();
    connect(client,&QTcpSocket::readyRead,this,&RequestHandler::readRemote);
    connect(client,&QTcpSocket::disconnected,client,&QTcpSocket::deleteLater);
    QString testStr = "1$Hello Test!~";
    QByteArray ba;
    ba.append(testStr);
    client->write(ba);
        client->flush();
        client->waitForBytesWritten(3000);
    QString data(ba);
    qDebug()<<data;
}

void RequestHandler::readRemote(){
    QTcpSocket* client = dynamic_cast<QTcpSocket*>(sender());
    quint16 count = client->bytesAvailable();
    QByteArray ba = client->readAll();
    QString str(ba);
    qDebug()<<str;
}
