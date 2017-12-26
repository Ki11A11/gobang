#ifndef REMOTE_H
#define REMOTE_H
#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkSession>
#include <QDataStream>
#include <QHostAddress>
#include <QTcpServer>
class RequestInvoker;
class RequestHandler;
class CommunicationManager:public QObject{
    Q_OBJECT
public:
    CommunicationManager();
    ~CommunicationManager();
signals:
    void request_Remote();
    void listen_Remote();
public slots:
    void handle_Response(QString msg);
    void handle_Request(QString msg);
private:
    QThread requestThread;
    QThread listenThread;
};

class RequestInvoker:public QObject{
    Q_OBJECT
public:
    RequestInvoker();
    ~RequestInvoker();
signals:
    void return_Response(QString msg);
    void listen_Remote();
public slots:
    void Request();
private slots:
    void replayFinished(QNetworkReply* reply);
    void displayError(QAbstractSocket::SocketError socketError);
    void readRemote();
    void sessionOpened();
    void socketConnected();
private:
    QTcpSocket *tcpSocket = nullptr;
    QNetworkSession* networkSession = nullptr;
};

class RequestHandler:public QObject{
    Q_OBJECT
public:
    RequestHandler();
    ~RequestHandler();
signals:
    void return_Request(QString msg);
public slots:
    void Listen();
private slots:
    void newConnection();
    void readRemote();
private:
    QTcpServer* tcpServer=nullptr;
};


#endif // REMOTE_H
