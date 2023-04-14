#include "include/HttpServerHandler.h"

HttpServerHandler::HttpServerHandler(QThreadPool *databasePool, QObject *parent) : QObject(parent)
{
    this->databaseConnectionsPool = databasePool;
//    assert(setSslCertificate(pathToCertificate));
//    assert(setSslPrivateKey(pathToPrivateKey));
//    server.sslSetup(sslCertificate, sslPrivateKey);

    server.route("/", [] () {
        return "Index";
    });

    server.route("/api/v1/managers", QHttpServerRequest::Method::Get, [&] (const QHttpServerRequest &request) {
        return QtConcurrent::run(databaseConnectionsPool, [&request] () {
            DatabaseConnector databaseConnector;

            QUrl url = request.url();
            QUrlQuery postParameters(url);

            QString nameValue = postParameters.queryItemValue("name");
            QJsonObject result = databaseConnector.managers(nameValue);

            return QHttpServerResponse(result);
        });
    });

    server.route("/api/v1/get_sum", QHttpServerRequest::Method::Post, [&] (const QHttpServerRequest &request) {

        QUrl url = request.url();
        QUrlQuery postParameters(url);

        qDebug() << postParameters.toString();
        QString aValue = postParameters.queryItemValue("a");
        QString bValue = postParameters.queryItemValue("b");

        int a = aValue.toInt();
        int b = bValue.toInt();
        int sum = a + b;

        QJsonObject jsonObject;
        jsonObject["a"] = a;
        jsonObject["b"] = b;
        jsonObject["sum"] = sum;

        return jsonObject;
    });
}

short HttpServerHandler::getServerPort() const
{
    return serverPort;
}

void HttpServerHandler::setServerPort(short newServerPort)
{
    serverPort = newServerPort;
}

bool HttpServerHandler::setSslCertificate(const QString& path, QSsl::EncodingFormat format)
{
    QFile certificateFile(path);
    if (!certificateFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    sslCertificate = QSslCertificate(certificateFile.readAll(), format);
    return true;
}

bool HttpServerHandler::setSslPrivateKey(const QString& path, const QByteArray& password, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format)
{
    QFile keyFile(path);
    if (!keyFile.open(QIODevice::ReadOnly)) {
        return false;
    }

    sslPrivateKey = QSslKey(keyFile.readAll(), algorithm, format, QSsl::PrivateKey, password);
    return true;
}

void HttpServerHandler::startServer()
{
    short newPort = server.listen(QHostAddress::Any, serverPort);
    assert(newPort == serverPort);
    qDebug() << "Server started";
    emit serverStarted();
}



