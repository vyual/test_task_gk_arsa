#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QThread>
#include <QSqlDriver>
#include <QSqlError>
#include <QJsonObject>
#include <QJsonArray>

///
/// Класс, работающий с БД, инкапсулируя SQL-запросы
///
class DatabaseConnector : public QObject
{
    Q_OBJECT

public:

    static const int MAX_DATABASE_CONNECTIONS_COUNT = 5;

    explicit DatabaseConnector(QObject *parent = nullptr);

    /// Получение пользователя по имени
    QJsonObject managers(QString& name);


private:

    QString connectionName;

public slots:

    ///
    /// \brief Подключение к БД, используя данные из database_secrets.ini.
    /// \return true, если подключение удалось, иначе false.
    ///
    bool connect();

    ///
    /// \brief Отключение от БД.
    ///
    void disconnect();

};

#endif // DATABASECONNECTOR_H
