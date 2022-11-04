#include "database.h"
#include<QSqlRecord>
database::database(QObject *parent)
    : QObject{parent}
{

}

void database::openDatabase(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database failed";
    }
    else
    {
        qDebug() << "Database: Open Successfully ! ... ok";
    }
}

bool database::createTabel()
{
       QSqlQuery query;
       bool success = false;
       query.prepare("CREATE TABLE IF NOT EXISTS people(ids integer primary key, name text);");
       if(query.exec())
       {
           success = true;
           qDebug()<<"Table Create Successfully !";
           return success;
       }
       else
       {
            qDebug() << "Table Create Error !"
                     << query.lastError();
            return success;
       }
       return success;
}

bool database::addPerson(QString name)
{
    bool success = false;
    // you should check if args are ok first...
    QSqlQuery query;
    query.prepare("INSERT INTO people (name) VALUES (:name)");
    query.bindValue(":name", name);
    if(query.exec())
    {
        success = true;
        qDebug()<<"Person Added into database successfully !";
        return success;
    }
    else
    {
        qDebug() << "addPerson add error ! "
                 << query.lastError();
        return success;
    }
    return success;
}

bool database::checkExists(QString name)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM people WHERE name = (:name)");
    query.bindValue(":name", name);

    if (query.exec())
    {
        if (query.next())
        {
            // it exists
            return true;
        }
    }
    return false;
}

bool database::removePerson(QString name)
{
    bool success = false;
    if (checkExists(name))
    {
        QSqlQuery query;
        query.prepare("DELETE FROM people WHERE name = (:name)");
        query.bindValue(":name", name);
        success = query.exec();

        if(!success)
        {
            qDebug() << "removePerson person error:"
                     << query.lastError();
            return false;
        }else
        {
            qDebug()<<"Person Deleted Successfully from the database ... !";
            return true;
        }
    }
    return false;
}

bool database::removeAll()
{
    QSqlQuery query;
    query.prepare("DELETE FROM people");
    bool ret = query.exec();
    return ret;
}

void database::printAllPerson()
{
    QSqlQuery query("SELECT * FROM people");
    int count = 0;
    while (query.next()) {
        count++;
        QSqlRecord record = query.record();
        qDebug() << "Name : " << record.value(1).toString()<<Qt::endl;

    }
}
