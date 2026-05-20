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

private:
    Ui::StudentWidget *ui;
};

#endif // STUDENTWIDGET_H