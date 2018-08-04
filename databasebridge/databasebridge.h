#ifndef DATABASEBRIDGE_H
#define DATABASEBRIDGE_H

#include <QObject>
#include <QString>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>


//typedef QList<QVariant> QVarList;




class DataBaseBridge : public QObject
{
    Q_OBJECT
public:
    // construction and destruction function
    explicit DataBaseBridge(QObject *parent = nullptr);
    explicit DataBaseBridge(const QString &hostName, const QString &databaseName,
                            const QString &userName, const QString &password, QObject *parent = nullptr);
    explicit DataBaseBridge(const QString &hostName, QObject *parent = nullptr);
    ~DataBaseBridge();
    // table operation
    void createTable(const QString &table, const QStringList& fields);
    void createTable(const QString &table, const QString& fields);
    void dropTable(const QString &table);
    // item operation
    void insertItem(const QString& table, const QList<QVariant>& varList);
    void deleteItem(const QString& table, const QString& condition = QString());
    const QList<QVariant> &searchFirstItem(const QString& table,
                                      const QString& target = QString("*"),
                                      const QString& condition = QString());
    // query operation
    bool exec(const QString &sql);
    // database operation
    void openDataBase();
    void closeDataBase();
    bool isDataBaseOpen();

signals:

public slots:

protected:
    void checkSuccess(bool success, const QString &action);
    QString generateInsertString(int cntVar);
    QString addWherePrefix(const QString& str);

private:
    QList<QVariant> listvar;
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // DATABASEBRIDGE_H
