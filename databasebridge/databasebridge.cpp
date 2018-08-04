#include "databasebridge.h"

#include <QTextCodec>
#include <QtDebug>
#include <QSqlDriver>
#include <QSqlError>

#include <typeinfo>

DataBaseBridge* DataBaseBridge::instance = nullptr;
DataBaseBridge::DataBaseBridgeGarbo DataBaseBridge::garbo;

DataBaseBridge::DataBaseBridge(QObject *parent) : QObject (parent)
{

}
DataBaseBridge::DataBaseBridge(
        const QString &hostName,
        const QString &databaseName,
        const QString &userName,
        const QString &password,
        QObject *parent) :
    QObject(parent),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    query(db)
{
    // enable Chinese
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    // create SQLite database, using QSQLITE driver
    db.setHostName(hostName);
    db.setDatabaseName(databaseName);
    db.setUserName(userName);
    db.setPassword(password);
    db.open();
    qDebug()<<"open";
}
DataBaseBridge::DataBaseBridge(const QString &databaseName, QObject *parent) :
    QObject (parent),
    db(QSqlDatabase::addDatabase("QSQLITE")),
    query(db)
{
    // enable Chinese
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

    db.setDatabaseName(databaseName);
    db.open();
    assert(db.isOpen());
}
void DataBaseBridge::init(const QString &hostName, QObject *parent)
{
    if(instance)
    {
        qDebug()<<"instance already exists";
        return ;
    }
    instance = new DataBaseBridge(hostName, parent);
    assert(instance);
}
DataBaseBridge *DataBaseBridge::getInstance()
{
    if(!instance)
    {
        throw std::logic_error("instance is not initialized, please run init() first");
       // return nullptr;
    }
    return instance;
}

bool DataBaseBridge::dropTable(const QString &table)
{
    // delete all data in the table and discard the table.
    if(db.tables().contains(table))
    {
        return checkSuccess(query.exec(QString("DROP TABLE %1").arg(table)), tr("drop table"));
    }
    else
    {
        qDebug()<<QString("could not delete %1, no such table").arg(table);
        return false;
    }
}
//bool DataBaseBridge::createTable(const QString &table, const QStringList& fields)
//{
//    //not tested
//    if(db.tables().contains(table))
//    {
//        qDebug()<<"table already exists";
//        return ;
//    }
//    dropTable(tr("automobile"));
//    bool success = query.exec(QString(
//                                  "create table %1 (id int primary key, attibute varchar, type varchar, kind varchar, carnumber int)"
//                                  ).arg(table));
//    checkSuccess(success, tr("create table"));
//}
bool DataBaseBridge::createTable(const QString &table, const QString& fields)
{
    // create table with given table names and fields
    if(db.tables().contains(table))
    {
        qDebug()<<"table already exists";
        return false;
    }
    return checkSuccess(query.exec(QString("create table %1 (%2)").arg(table).arg(fields)), tr("create table"));

}

QString DataBaseBridge::generateInsertString(int cntVar)
{
    // generate a string like "(?,?,...,?,?)"
    // number of question mark corresponds to cntVar
    QString str("(?");
    for(int i=1; i<cntVar; i++)
    {
        str += QString(",?");
    }
    return str + QString(")");
}
bool DataBaseBridge::exec(const QString &sql)
{
    // execute sql query
    return query.exec(sql);
}
const QList<QVariant> &DataBaseBridge::searchFirstItem(const QString& table, const QString& target, const QString& condition)
{
    // return the first item found in table, according to condition.
    // if target not specified, all fields will be selected.
    // if condition not specified, all items will be selected.

    checkSuccess(query.exec(QString("SELECT %1 FROM %2 %3").arg(target).arg(table).arg(addWherePrefix(condition))), "search");

    // create list variant
    this->listvar.clear();
    if(query.next())
    {
        for(int i = 0;i < query.record().count(); i++)
        {
            listvar.append(query.value(i));
        }
    }
    return listvar;
}
bool DataBaseBridge::insertItem(const QString& table, const QList<QVariant>& varList)
{
    // insert item, whose attributes are specified in varList, into table.
    query.clear();
    query.prepare(QString("INSERT INTO %1 VALUES %2").arg(table).arg(generateInsertString(varList.size())));

    int seq = 0;
    for(QList<QVariant>::const_iterator it = varList.begin(); it != varList.end(); it++)
    {
        query.bindValue(seq, *it);
        seq++;
    }
    return checkSuccess(query.exec(), "insert");
}

bool DataBaseBridge::checkSuccess(bool success, const QString& action)
{
    if(success)
    {
        qDebug()<<action<<" success";
    }
    else
    {
        qDebug()<<query.lastError()<<' '<<action<<" fail";
    }
    return success;
}
bool DataBaseBridge::deleteItem(const QString& table, const QString& condition)
{
    // delete Items that meet the condition from table
    // if condition is not specified, ALL data from table will be deleted

    bool success = query.exec(QString("DELETE FROM %1 %2").arg(table).arg(addWherePrefix(condition)));
    return checkSuccess(success, "delete");
}

void DataBaseBridge::openDataBase()
{
    if(db.isOpen()) db.open();
    qDebug()<<"database open";
}
void DataBaseBridge::closeDataBase()
{
    if(db.isOpen()) db.close();
    qDebug()<<"database closed";
}

bool DataBaseBridge::isDataBaseOpen()
{
    return db.isOpen();
}

QString DataBaseBridge::addWherePrefix(const QString& condition)
{
    if(!condition.isEmpty())
    {
        return QString("WHERE ") + condition;
    }
    else
    {
        return condition;
    }
}
DataBaseBridge::~DataBaseBridge()
{
    if(db.isOpen()) db.close();
    qDebug()<<"database closed";
}
