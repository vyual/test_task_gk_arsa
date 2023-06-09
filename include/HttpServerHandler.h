#ifndef HTTPSERVERHANDLER_H
#define HTTPSERVERHANDLER_H

#include <QObject>
#include <QHttpServer>
#include <QFile>
#include <QSslKey>
#include <QSslConfiguration>
#include "include/DatabaseConnector.h"
#include "qjsonobject.h"
#include <QtConcurrent>

#define internalErrorResponse QHttpServerResponse("Internal error", QHttpServerResponse::StatusCode::InternalServerError)

///
/// \class HttpServerHandler
/// \brief Класс, управляющий классом QHttpServer, обрабатывающий входящие запросы.
///
class HttpServerHandler : public QObject
{
    Q_OBJECT
public:
    explicit HttpServerHandler(QThreadPool *databasePool, QObject *parent = nullptr);

    short getServerPort() const;
    void setServerPort(short newServerPort);

    ///
    /// \brief Установка сертификата из файла.
    /// \param path - Путь к файлу приватного ключа.
    /// \param format - Формат кодирования ключа.
    /// \return true при успешном присваивании, false при ошибке.
    ///
    bool setSslCertificate(const QString& path, QSsl::EncodingFormat format = QSsl::Pem);

    ///
    /// \brief Установка приватного ключа из файла.
    /// \param path - Путь к файлу приватного ключа.
    /// \param password - Пароль приватного ключа.
    /// \param algorithm - Алгоритм приватного ключа.
    /// \param format - Формат кодирования ключа.
    /// \return true при успешном присваивании, false при ошибке.
    ///
    bool setSslPrivateKey(const QString& path, const QByteArray& password = QByteArray(), QSsl::KeyAlgorithm algorithm = QSsl::Rsa, QSsl::EncodingFormat format = QSsl::Pem);

    static const QHttpServerResponse &getInternalErrorResponse();

private:
    QHttpServer server;

    QThreadPool *databaseConnectionsPool;

    QSslCertificate sslCertificate;
    QSslKey sslPrivateKey;
    short serverPort = 8080;

    ///
    /// \brief Обработчик запроса, который вызывается из лямбда-функций, вызываемых QHttpServer::route.
    /// \param solver - указатель на любой класс, решающий задачу (наследник AbstractSolver).
    /// \param request - Http запрос QHttpServerRequest.
    /// \return
    ///
    QFuture<QHttpServerResponse> processRequest(const QHttpServerRequest &request);



signals:
    ///
    ///  Вызывается после успешного запуска сервера
    ///
    void serverStarted();

public slots:

    ///
    /// Слот для запуска сервера
    ///
    void startServer();
};

#endif // HTTPSERVERHANDLER_H
