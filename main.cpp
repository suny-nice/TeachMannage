#include "mainwindow.h"
#include "database.h"
#include "logindialog.h"
#include "setting.h"
#include <QApplication>
#include <QLoggingCategory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("StudentMannageSystem");

    // 屏蔽 Qt 调试打印信息
    QLoggingCategory::setFilterRules(
        "qt.pointer.dispatch=false\n"
        "qt.gui.imageio=false\n");

    // 根据配置设置数据库路径
    Database::instance();
    QString dbPath = Setting::instance().getDatabasePath();
    if (!dbPath.isEmpty()) {
        Database::instance().openDatabase(dbPath);
    } else {
        Database::instance().openDatabase();
        // 将默认数据库路径写入配置
        Setting::instance().setDatabasePath(Database::instance().getDatabasePath());
    }

    // 先弹出登录对话框
    LoginDialog login;
    if (login.exec() != QDialog::Accepted) {
        return 0;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
