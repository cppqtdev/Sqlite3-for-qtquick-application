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
