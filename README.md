# Sqlite3-for-qtquick-application

Written by raghav
Posted on November 4, 2022  13 min read

Accessing SQL databases from C++ applications is very simple with Qt library. Here is some short examples that presents how to do it. I have chosen 
SQLite
 engine because it’s the easiest engine to set up (it requires no server, no configuration…), still it’s suitable for the most of possible applications.

How To Install sqlite3 in windows 11 || Windows 10
Before you begin – sql drivers
Qt requires drivers to deal with SQL databases. Depending on your distribution, you can have the drivers installed by default with your Qt or not. If you face problems like “QSqlDatabase: QSQLITE driver not loaded” you need to install drivers manually. Error should be followed by a console output that informs you which drivers you have installed: “QSqlDatabase: available drivers:”. Read more about sql drivers….

Design and create the database
Let’s assume, for simplicity, that we need to store people data – only name and corresponding id. With SQLite you can create such a simple database with two console commands. First line is creating database people. Second one is creating the table people. It contains a key (integer) and name (text). To leave the SQLite console just type “. quit”.


$ sqlite3 mydatabase.db
sqlite> CREATE TABLE people (ids integer primary key, name text);
sqlite>.quit
Congrats, your database is ready. Now we can create C++ application to play with it.

Access database from Qt application
Create new Qt Quick project. In .pro file you need to add:

QT += sql


Add sql to your .pro file
It will link your project against QtSql module. If you don’t use QtCreator but another IDE, for example Visual Studio, your project may need setting linker dependency to QtSql.lib. You need to do it if your compilation fails with unresolved external symbol error. In VS linker settings is located in Properties/Configuration properties/Linker/Input. In the Additional Dependencies add Qt5Sqld.lib for Debug and Qt5Sql.lib for release.

Now you can create a class responsible for database access. Let’s create a class database.

database.h :

// database.h 

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QObject>
class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = nullptr);
    void openDatabase(const QString& path);
private:
    QSqlDatabase m_db;

signals:

};

#endif // DATABASE_H
database.cpp :

// database.cpp

#include "database.h"

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
Now include the header file of database ( database.h ) into the main.cpp file of your qt project.

main.cpp :

// main.cpp 

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "database.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    /*Create Database class Object*/
    database mydataObject;
    mydataObject.openDatabase("C:\\Users\\codeit\\Desktop\\sqlite3\\mydatabase.db");


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
In openDatabase(path) pass the path of your database.db file location:

Now build the project using CTRL + R and see the output in application output section.


Congratulation you are successfully open sqlite3 database in windows.

Now it’s time to perform some operations on sqlite3 database using qt c++.
For managing people resources we need the possibility to:

add new person,
remove person,
check if person exists in database,
print all persons,
delete all persons.
Actions can be implemented in following way: define QSqlQuery, then call prepare method. Prepare argument can be raw SQL query string, or it can contain placeholders for variables — placeholders must begin with colon. After prepare call bindValue to fill the placeholders with proper values. When the query is ready, execute it.

Create A Table in sqlite3 database :
For managing people resources we need to first create the table into the sqlite3 database using the cpp . So, firstly we create the API for creating the table .

bool
createTabel
(); Creating the table for storing the people information into the table.

Add bool
createTabel
(); into the database.h file.

bool createTabel();
Add the following API into the database.cpp file

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
Now build and run the project using the ctrl+r and see the output into application output section :


you can also confirm the table created success or not using the command line .

Before creating the table run the command .tableinto the sqlite> terminal .


Now you can see there is no table present into the mydatabase .

Now Create the table .. And see the output .


Now you can see these are one table present into the mydatabase.db which is name people

Now table create successfully, it’s time to inset some people data into the table. so, we need to create API to insert the data into table.

Add this API into the database.h file:

bool addPerson(QString name);
and definition into the database.cpp file:

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
And add the following code into the main.cpp file after the create table .

    // Insert 3 people into the table
    mydataObject.addPerson("Aksh Singh");
    mydataObject.addPerson("Ram Singh");
    mydataObject.addPerson("hemant Singh");
Now build and run the code and absorb the behaviors :

Output :

Database: Open Successfully ! ... ok
Table Create Successfully !
Person Added into database successfully !
Person Added into database successfully !
Person Added into database successfully !
And similarly, all other APIs works try it yourself, if you face any issue, please let me know into the comment section.

database.cpp file :

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
database.h file

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QObject>
class database : public QObject
{
    Q_OBJECT
public:
    explicit database(QObject *parent = nullptr);
    void openDatabase(const QString& path);
    bool createTabel();
    bool addPerson(QString name);
    bool checkExists(QString name);
    bool removePerson(QString name);
    bool removeAll();
    void printAllPerson();

private:
    QSqlDatabase m_db;

signals:

};

#endif // DATABASE_H
main.cpp

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "database.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    /*Create Database class Object*/
    database mydataObject;
    mydataObject.openDatabase("C:\\Users\\codeit\\Desktop\\sqlite3\\mydatabase.db");

    // create the table
    mydataObject.createTabel();

    // Insert 3 people into the table
    mydataObject.addPerson("Aksh Singh");
    mydataObject.addPerson("Ram Singh");
    mydataObject.addPerson("hemant Singh");

   /* Check if Person Exits*/
    bool isPersonExists = mydataObject.checkExists("Aksh Singh");
    if(isPersonExists){
        qInfo()<<"Person Exits..";
    }else{
         qInfo()<<"Person Not Exits..";
    }

    /*Print All The Person*/

    mydataObject.printAllPerson();

    /*Remove a person  */

     bool isRemoved = mydataObject.checkExists("Aksh Singh");
     if(isRemoved){
         qInfo()<<"Person Removed..";
     }else{
          qInfo()<<"Person Not Removed..";
     }

     /*Now Again Print All The Person*/

       mydataObject.printAllPerson();

       /* Remove All The Person */
       bool isRemoveAll = mydataObject.removeAll();
       if(isRemoveAll){
           qInfo()<<"All Person Removed..";
       }else{
            qInfo()<<"All Person Not Removed..";
       }

       /*Now Again Print All The Person*/

        mydataObject.printAllPerson();


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
Output :

Database: Open Successfully ! ... ok
Table Create Successfully !
Person Added into database successfully !
Person Added into database successfully !
Person Added into database successfully !
Person Exits..
Name :  "Aksh Singh" 

Name :  "Ram Singh" 

Name :  "hemant Singh" 

Person Removed..
Name :  "Aksh Singh" 

Name :  "Ram Singh" 

Name :  "hemant Singh" 

All Person Removed..
Congratulations you are successfully performed sqlite3 basic operation using qt quick application. do practice more and perform some more operation using qt quick application.

if you are facing any issue during practice comment out your problem .
