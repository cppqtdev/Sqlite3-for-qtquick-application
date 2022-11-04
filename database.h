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
