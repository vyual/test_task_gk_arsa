#include "include/DatabaseConnector.h"

DatabaseConnector::DatabaseConnector(QObject *parent)
    : QObject{parent}
{
    connectionName = QString("%1").arg((qintptr)QThread::currentThread());
    if(!QSqlDatabase::connectionNames().contains(connectionName))
        connect();
}

QJsonObject DatabaseConnector::managers(QString& name) {
    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if(!db.isOpen())
        return QJsonObject();

    QSqlQuery query(db);
    query.prepare("SELECT * FROM managers WHERE name = :name");
    query.bindValue(":name", name);

    if(!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError();
        return QJsonObject();
    }

    QJsonArray managersArray;

    while(query.next()) {
        QJsonObject manager;
        manager["id"] = query.value("id").toInt();
        manager["family"] = query.value("family").toString();
        manager["name"] = query.value("name").toString();
        managersArray.append(manager);
    }

    QJsonObject response;
    response["managers"] = managersArray;

    return response;
}

bool DatabaseConnector::connect()
{
    bool ok;

    const QSettings databaseSecrets("database_secrets.ini", QSettings::Format::IniFormat);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);

    db.setHostName(databaseSecrets.value("Database/DATABASE_HOST").toString());
    db.setDatabaseName(databaseSecrets.value("Database/DATABASE_NAME").toString());
    db.setUserName(databaseSecrets.value("Database/DATABASE_USER").toString());
    db.setPassword(databaseSecrets.value("Database/DATABASE_PASSWORD").toString());

    ok = db.open();

    if(ok)
        qDebug() << "Connected to database. Connection:" << connectionName;
    else
        qDebug() << "Failed to connect to database";

    return ok;
}

void DatabaseConnector::disconnect()
{
    qDebug() << "Disconnecting from database";
    {
        QSqlDatabase db = QSqlDatabase::database(connectionName);
        db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    qDebug() << "Disconnected from database";
}
