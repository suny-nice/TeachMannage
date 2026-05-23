#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include <QWidget>

namespace Ui {
class StudentWidget;
}

class StudentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StudentWidget(QWidget *parent = nullptr);
    ~StudentWidget();
    void loadStudentData();

private slots:
    void on_pushButton_clicked();
    void on_btnDeleteList_clicked();
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::StudentWidget *ui;
};

#endif // STUDENTWIDGET_H
