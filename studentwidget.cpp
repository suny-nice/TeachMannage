#include "studentwidget.h"
#include "ui_studentwidget.h"
#include "database.h"
#include "addstudentdialog.h"
#include <QMessageBox>
#include <QPixmap>
#include <QTableWidgetItem>

StudentWidget::StudentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentWidget)
{
    ui->setupUi(this);
    
    ui->tableWidget->verticalHeader()->setFixedWidth(35);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(80);
}

StudentWidget::~StudentWidget()
{
    delete ui;
}

void StudentWidget::loadStudentData()
{
    ui->tableWidget->setRowCount(0);
    
    QList<QMap<QString, QVariant>> students = Database::instance().getAllStudents();
    
    for (const QMap<QString, QVariant>& student : students) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(student["id"].toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(student["name"].toString()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(student["gender"].toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(student["birthday"].toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(student["join_date"].toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(student["study_goal"].toString()));
        ui->tableWidget->setItem(row, 6, new QTableWidgetItem(student["progress"].toString()));
        
        QByteArray photoData = student["photo"].toByteArray();
        if (!photoData.isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(photoData);
            QLabel *label = new QLabel();
            label->setPixmap(pixmap.scaled(60, 80, Qt::KeepAspectRatio));
            ui->tableWidget->setCellWidget(row, 7, label);
        }
    }
    
}

void StudentWidget::on_pushButton_clicked()
{
    AddStudentDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString id = dialog.getId();
        QString name = dialog.getName();
        QString gender = dialog.getGender();
        QString birthday = dialog.getBirthday();
        QString joinDate = dialog.getJoinDate();
        QString studyGoal = dialog.getStudyGoal();
        QString progress = dialog.getProgress();
        QByteArray photoData = dialog.getPhotoData();
        
        bool success = Database::instance().addStudent(id, name, gender, birthday, joinDate, studyGoal, progress, photoData);
        
        if (success) {
            QMessageBox::information(this, "成功", "学生信息添加成功");
            loadStudentData();
        } else {
            QMessageBox::warning(this, "失败", "学生信息添加失败");
        }
    }
}