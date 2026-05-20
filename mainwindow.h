#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QFile>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class StudentWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void loadStyleSheet()
    {
        QFile file(":/style.qss");
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QString::fromUtf8(file.readAll());
            this->setStyleSheet(styleSheet);
            file.close();
        }
    }

private:
    Ui::MainWindow *ui;
    QButtonGroup *buttonGroup;
};
#endif // MAINWINDOW_H