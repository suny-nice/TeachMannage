#include "mainwindow.h"
#include "database.h"
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

    Database::instance();
    MainWindow w;
    w.show();
    return a.exec();
}
