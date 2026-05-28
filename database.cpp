#include "database.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QCoreApplication>

Database::Database(QObject *parent) : QObject(parent)
{
    // 数据库固定存放在系统应用数据目录
    dbPath = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation) + "/techManSys.db";
}

Database::~Database() { closeDatabase(); }

Database& Database::instance()
{
    static Database instance;
    return instance;
}

void Database::closeDatabase()
{
    if (db.isOpen()) db.close();
}

bool Database::openDatabase(const QString& path)
{
    if (!path.isEmpty()) {
        dbPath = path;
    }

    // 首次运行：从旧位置（项目根目录）迁移已有数据
    QString oldDb = QCoreApplication::applicationDirPath();
    for (int i = 0; i < 8; ++i) {
        QString candidate = QDir(oldDb).absoluteFilePath("sqllite/techManSys.db");
        if (QFileInfo::exists(candidate)) {
            QString newDir = QFileInfo(dbPath).path();
            QDir().mkpath(newDir);
            if (!QFileInfo::exists(dbPath)) {
                QFile::copy(candidate, dbPath);
            }
            break;
        }
        QDir d(oldDb);
        if (!d.cdUp()) break;
        oldDb = d.absolutePath();
    }

    // 确保目录存在
    QDir().mkpath(QFileInfo(dbPath).path());

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (db.open()) {
        createTables();
        return true;
    }
    return false;
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

    // 插入默认管理员账户（仅当 users 表为空时）
    QSqlQuery checkAdmin;
    checkAdmin.exec("SELECT COUNT(*) FROM users");
    if (checkAdmin.next() && checkAdmin.value(0).toInt() == 0) {
        QSqlQuery insAdmin;
        insAdmin.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
        insAdmin.bindValue(":u", "admin");
        insAdmin.bindValue(":p", "admin123");
        insAdmin.exec();
    }

    // qDebug() << "所有表创建完成";
    return true;
}

bool Database::addHonorWallRecord(const QByteArray& imageData, const QString& description)
{
    if (!db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("INSERT INTO honorWall (image_data, description, added_date) "
                  "VALUES (:img, :desc, datetime('now','localtime'))");
    query.bindValue(":img", imageData);
    query.bindValue(":desc", description);

    if (query.exec()) return true;
    qDebug() << "添加荣誉记录失败:" << query.lastError().text();
    return false;
}

QList<QMap<QString, QVariant>> Database::getAllHonorWallRecords()
{
    QList<QMap<QString, QVariant>> records;
    if (!db.isOpen()) return records;

    QSqlQuery query("SELECT * FROM honorWall ORDER BY id ASC");
    while (query.next()) {
        QMap<QString, QVariant> record;
        record["id"] = query.value("id");
        record["image_data"] = query.value("image_data");
        record["description"] = query.value("description");
        record["added_date"] = query.value("added_date");
        records.append(record);
    }
    return records;
}

bool Database::updateHonorWallRecord(int id, const QByteArray& imageData, const QString& description)
{
    if (!db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("UPDATE honorWall SET image_data = :img, description = :desc WHERE id = :id");
    query.bindValue(":img", imageData);
    query.bindValue(":desc", description);
    query.bindValue(":id", id);

    if (query.exec()) return true;
    qDebug() << "更新荣誉记录失败:" << query.lastError().text();
    return false;
}

bool Database::deleteHonorWallRecord(int id)
{
    if (!db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM honorWall WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) return true;
    qDebug() << "删除荣誉记录失败:" << query.lastError().text();
    return false;
}

bool Database::authenticateUser(const QString& username, const QString& password)
{
    if (!db.isOpen()) return false;

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :uname");
    query.bindValue(":uname", username);

    if (query.exec() && query.next()) {
        QString storedPwd = query.value("password").toString();
        return storedPwd == password;
    }
    return false;
}

bool Database::changePassword(const QString& username, const QString& oldPassword, const QString& newPassword)
{
    if (!db.isOpen()) return false;

    // 先验证旧密码
    if (!authenticateUser(username, oldPassword)) return false;

    QSqlQuery query;
    query.prepare("UPDATE users SET password = :newpwd WHERE username = :uname");
    query.bindValue(":newpwd", newPassword);
    query.bindValue(":uname", username);

    if (query.exec()) return true;
    qDebug() << "修改密码失败:" << query.lastError().text();
    return false;
}

bool Database::addStudent(const QString& id, const QString& name, const QString& gender,
                          const QString& birthday, const QString& joinDate,
                          const QString& studyGoal, const QString& progress,
                          const QByteArray& photoData)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO studentInfo (id, name, gender, birthday, join_date, study_goal, progress, photo) "
                  "VALUES (:id, :name, :gender, :birthday, :join_date, :study_goal, :progress, :photo)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":gender", gender);
    query.bindValue(":birthday", birthday);
    query.bindValue(":join_date", joinDate);
    query.bindValue(":study_goal", studyGoal);
    query.bindValue(":progress", progress);
    query.bindValue(":photo", photoData);

    if (query.exec()) {
        qDebug() << "学生信息添加成功";
        return true;
    } else {
        qDebug() << "添加学生信息失败:" << query.lastError().text();
        return false;
    }
}

bool Database::updateStudent(const QString& id, const QString& field, const QVariant& value)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    // 白名单校验，防止 SQL 注入
    QStringList allowedFields = {"name", "gender", "birthday", "join_date", "study_goal", "progress", "photo"};
    if (!allowedFields.contains(field)) {
        qDebug() << "不允许的字段:" << field;
        return false;
    }

    QSqlQuery query;
    QString sql = QString("UPDATE studentInfo SET %1 = :value WHERE id = :id").arg(field);
    query.prepare(sql);
    query.bindValue(":value", value);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "学生信息更新成功:" << id << field;
        return true;
    } else {
        qDebug() << "更新学生信息失败:" << query.lastError().text();
        return false;
    }
}

bool Database::deleteStudent(const QString& id)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM studentInfo WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "学生信息删除成功:" << id;
        return true;
    } else {
        qDebug() << "删除学生信息失败:" << query.lastError().text();
        return false;
    }
}

QList<QMap<QString, QVariant>> Database::getAllStudents()
{
    QList<QMap<QString, QVariant>> students;
    
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return students;
    }

    QSqlQuery query("SELECT * FROM studentInfo");
    while (query.next()) {
        QMap<QString, QVariant> student;
        student["id"] = query.value("id");
        student["name"] = query.value("name");
        student["gender"] = query.value("gender");
        student["birthday"] = query.value("birthday");
        student["join_date"] = query.value("join_date");
        student["study_goal"] = query.value("study_goal");
        student["progress"] = query.value("progress");
        student["photo"] = query.value("photo");
        students.append(student);
    }

    return students;
}

bool Database::addFinancialRecord(const QString& studentId, const QString& paymentDate,
                                   double amount, const QString& paymentType, const QString& notes)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO financialRecords (student_id, payment_date, amount, payment_type, notes) "
                  "VALUES (:student_id, :payment_date, :amount, :payment_type, :notes)");
    query.bindValue(":student_id", studentId);
    query.bindValue(":payment_date", paymentDate);
    query.bindValue(":amount", amount);
    query.bindValue(":payment_type", paymentType);
    query.bindValue(":notes", notes);

    if (query.exec()) {
        qDebug() << "缴费记录添加成功";
        return true;
    } else {
        qDebug() << "添加缴费记录失败:" << query.lastError().text();
        return false;
    }
}

QList<QMap<QString, QVariant>> Database::getAllFinancialRecords()
{
    QList<QMap<QString, QVariant>> records;

    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return records;
    }

    QSqlQuery query("SELECT fr.*, si.name AS student_name "
                    "FROM financialRecords fr "
                    "LEFT JOIN studentInfo si ON fr.student_id = si.id "
                    "ORDER BY fr.payment_date ASC");
    while (query.next()) {
        QMap<QString, QVariant> record;
        record["id"] = query.value("id");
        record["student_id"] = query.value("student_id");
        record["student_name"] = query.value("student_name");
        record["payment_date"] = query.value("payment_date");
        record["amount"] = query.value("amount");
        record["payment_type"] = query.value("payment_type");
        record["notes"] = query.value("notes");
        records.append(record);
    }

    return records;
}

bool Database::deleteFinancialRecord(int id)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM financialRecords WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "缴费记录删除成功:" << id;
        return true;
    } else {
        qDebug() << "删除缴费记录失败:" << query.lastError().text();
        return false;
    }
}

bool Database::updateFinancialRecord(int id, const QString& paymentDate,
                                      double amount, const QString& paymentType, const QString& notes)
{
    if (!db.isOpen()) {
        qDebug() << "数据库未打开";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE financialRecords SET payment_date = :date, amount = :amount, "
                  "payment_type = :type, notes = :notes WHERE id = :id");
    query.bindValue(":date", paymentDate);
    query.bindValue(":amount", amount);
    query.bindValue(":type", paymentType);
    query.bindValue(":notes", notes);
    query.bindValue(":id", id);

    if (query.exec()) {
        qDebug() << "缴费记录更新成功:" << id;
        return true;
    } else {
        qDebug() << "更新缴费记录失败:" << query.lastError().text();
        return false;
    }
}
