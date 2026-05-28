#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QVariant>
#include <QList>
#include <QMap>

class Database : public QObject
{
    Q_OBJECT
public:
    static Database& instance();
    void closeDatabase();
    bool openDatabase(const QString& path = "");
    QString getDatabasePath() const;
    void setDatabasePath(const QString& path);
    bool createTables();

    bool addStudent(const QString& id, const QString& name, const QString& gender, 
                    const QString& birthday, const QString& joinDate, 
                    const QString& studyGoal, const QString& progress, 
                    const QByteArray& photoData);
    QList<QMap<QString, QVariant>> getAllStudents();
    bool deleteStudent(const QString& id);
    bool updateStudent(const QString& id, const QString& field, const QVariant& value);

    bool addFinancialRecord(const QString& studentId, const QString& paymentDate,
                            double amount, const QString& paymentType, const QString& notes);
    QList<QMap<QString, QVariant>> getAllFinancialRecords();
    bool deleteFinancialRecord(int id);
    bool updateFinancialRecord(int id, const QString& paymentDate,
                               double amount, const QString& paymentType, const QString& notes);

    // honorWall 操作
    bool addHonorWallRecord(const QByteArray& imageData, const QString& description);
    QList<QMap<QString, QVariant>> getAllHonorWallRecords();
    bool updateHonorWallRecord(int id, const QByteArray& imageData, const QString& description);
    bool deleteHonorWallRecord(int id);

    // 用户认证
    bool authenticateUser(const QString& username, const QString& password);
    bool changePassword(const QString& username, const QString& oldPassword, const QString& newPassword);

private:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    
    QSqlDatabase db;
    QString dbPath;
};

#endif // DATABASE_H