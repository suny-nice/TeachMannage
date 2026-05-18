#include "database.h"
#include <QFileInfo>

Database::Database(QObject *parent) : QObject(parent)
{
}

Database::~Database()
{
    closeDatabase();
}

Database& Database::instance()
{
    static Database instance;
    return instance;
}

void Database::closeDatabase()
{
    if (db.isOpen()) {
        db.close();
        qDebug() << "数据库已关闭";
    }
}

bool Database::openDatabase(const QString& path)
{
    if (!path.isEmpty()) {
        dbPath = path;
    }
    
    QString folder = QFileInfo(dbPath).path();
    QDir dir(folder);
    if (!dir.exists()) {
        dir.mkpath(folder);
        qDebug() << "创建文件夹:" << folder;
    }
    
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
    
    if (db.open()) {
        qDebug() << "数据库打开成功:" << dbPath;
        createTables();
        return true;
    } else {
        qDebug() << "数据库打开失败:" << db.lastError().text();
        return false;
    }
}

QString Database::getDatabasePath() const
{
    return dbPath;
}

void Database::setDatabasePath(const QString& path)
{
    dbPath = path;
}

bool Database::createTables()
{
    QSqlQuery query;
    
    QString sqlStudentInfo = 
        "CREATE TABLE IF NOT EXISTS studentInfo ("
        "id TEXT PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "gender TEXT, "
        "birthday TEXT, "
        "join_date TEXT, "
        "study_goal TEXT, "
        "progress TEXT, "
        "photo BLOB)";
    query.exec(sqlStudentInfo);
    
    QString sqlSchedule = 
        "CREATE TABLE IF NOT EXISTS schedule ("
        "date TEXT, "
        "time TEXT, "
        "course_name TEXT, "
        "PRIMARY KEY (date, time))";
    query.exec(sqlSchedule);
    
    QString sqlFinancial = 
        "CREATE TABLE IF NOT EXISTS financialRecords ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "student_id TEXT NOT NULL, "
        "payment_date TEXT NOT NULL, "
        "amount REAL NOT NULL, "
        "payment_type TEXT NOT NULL, "
        "notes TEXT)";
    query.exec(sqlFinancial);
    
    QString sqlHonor = 
        "CREATE TABLE IF NOT EXISTS honorWall ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "image_data BLOB NOT NULL, "
        "description TEXT NOT NULL, "
        "added_date TEXT)";
    query.exec(sqlHonor);
    
    QString sqlUsers = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT NOT NULL UNIQUE, "
        "password TEXT NOT NULL)";
    query.exec(sqlUsers);
    
    qDebug() << "所有表创建完成";
    return true;
}

bool Database::insertStudent(const QString& id, const QString& name, const QString& gender,
                            const QString& birthday, const QString& join_date,
                            const QString& study_goal, const QString& progress)
{
    QSqlQuery query;
    query.prepare("INSERT INTO studentInfo (id, name, gender, birthday, join_date, study_goal, progress) "
                  "VALUES (:id, :name, :gender, :birthday, :join_date, :study_goal, :progress)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":birthday", birthday);
    query.bindValue(":join_date", join_date);
    query.bindValue(":study_goal", study_goal);
    query.bindValue(":progress", progress);
    
    if (query.exec()) {
        qDebug() << "学生信息插入成功:" << name;
        return true;
    } else {
        qDebug() << "学生信息插入失败:" << query.lastError().text();
        return false;
    }
}

QList<QMap<QString, QVariant>> Database::getAllStudents()
{
    QList<QMap<QString, QVariant>> result;
    QSqlQuery query("SELECT * FROM studentInfo ORDER BY id");
    
    while (query.next()) {
        QMap<QString, QVariant> student;
        student["id"] = query.value("id").toString();
        student["name"] = query.value("name").toString();
        student["gender"] = query.value("gender").toString();
        student["birthday"] = query.value("birthday").toString();
        student["join_date"] = query.value("join_date").toString();
        student["study_goal"] = query.value("study_goal").toString();
        student["progress"] = query.value("progress").toString();
        result.append(student);
    }
    
    return result;
}

void Database::fictionalFunction()
{
    qDebug() << "这是一个虚构函数";
}