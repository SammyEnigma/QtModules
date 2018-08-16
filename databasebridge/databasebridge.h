#ifndef DATABASEBRIDGE_H
#define DATABASEBRIDGE_H

#include <QObject>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QDebug>

//typedef QList<QVariant> QVarList;


class DataBaseBridge : public QObject
{
    Q_OBJECT
private:
    DataBaseBridge& operator=(const DataBaseBridge&);
    DataBaseBridge(const DataBaseBridge&);

    explicit DataBaseBridge(QObject *parent = nullptr);
    explicit DataBaseBridge(const QString &hostName, const QString &databaseName,
                            const QString &userName, const QString &password, QObject *parent = nullptr);
    explicit DataBaseBridge(const QString &hostName, QObject *parent = nullptr);

public:

    // get instance
    static DataBaseBridge *getInstance();
    static void init(const QString &hostName, QObject *parent = nullptr);


    // construction and destruction function

    // disable (copy) construct function and assignment function



    ~DataBaseBridge();
    // table operation
    //bool createTable(const QString &table, const QStringList& fields);
    bool createTable(const QString &table, const QString& fields);
    bool dropTable(const QString &table);
    // item operation
    bool insertItem(const QString& table, const QList<QVariant>& varList);
    bool deleteItem(const QString& table, const QString& condition = QString());
    const QList<QVariant> &searchFirstItem(const QString& table,
                                      const QString& target = QString("*"),
                                      const QString& condition = QString());
    // query operation
    bool exec(const QString &sql);
    // database operation
    void openDataBase();
    void closeDataBase();
    bool isDataBaseOpen();


protected:
    bool checkSuccess(bool success, const QString &action);
    QString generateInsertString(int cntVar);
    QString addWherePrefix(const QString& str);

private:

    QList<QVariant> listvar;
    QSqlDatabase db;
    QSqlQuery query;


    class DataBaseBridgeGarbo
    {
    public:
        ~DataBaseBridgeGarbo()
        {
            qDebug()<<"delete instance";
            if(instance) delete instance;
        }
    };
    static DataBaseBridge *instance;
    static DataBaseBridgeGarbo garbo;
};

#endif // DATABASEBRIDGE_H
