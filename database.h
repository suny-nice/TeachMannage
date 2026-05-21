#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>
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

private:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    
    QSqlDatabase db;
    QString dbPath = "/Users/liaoyuanqing/Documents/Qt/InterProgram/StudentMannageSystem/sqllite/techManSys.db";
};

#endif // DATABASE_H