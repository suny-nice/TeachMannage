#include "studentwidget.h"
#include "ui_studentwidget.h"
#include "database.h"
#include "addstudentdialog.h"
#include <QMessageBox>
#include <QPixmap>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QBuffer>
#include <QMenu>
#include <QCalendarWidget>
#include <QVBoxLayout>
#include <QDialog>

StudentWidget::StudentWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StudentWidget)
{
    ui->setupUi(this);

    ui->tableWidget->verticalHeader()->setFixedWidth(35);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(80);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setTextElideMode(Qt::ElideNone);

    // 文本列原地编辑后保存到数据库
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, [this](QTableWidgetItem *item) {
        int row = item->row();
        int col = item->column();
        // 只处理文本列：姓名(1)、学习目标(5)
        QStringList fields = {"id", "name", "gender", "birthday", "join_date", "study_goal", "progress", "photo"};
        if (col != 1 && col != 5) return;
        QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
        if (!idItem) return;
        QString field = (col < fields.size()) ? fields[col] : "";
        if (field.isEmpty()) return;
        Database::instance().updateStudent(idItem->text(), field, item->text());
        // 不需要 loadStudentData，文本已经更新
    });
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

        QTableWidgetItem *idItem = new QTableWidgetItem(student["id"].toString());
        idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 0, idItem);
        QTableWidgetItem *nameItem = new QTableWidgetItem(student["name"].toString());
        ui->tableWidget->setItem(row, 1, nameItem);
        QTableWidgetItem *genderItem = new QTableWidgetItem(student["gender"].toString());
        genderItem->setFlags(genderItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 2, genderItem);
        QTableWidgetItem *birthItem = new QTableWidgetItem(student["birthday"].toString());
        birthItem->setFlags(birthItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 3, birthItem);
        QTableWidgetItem *joinItem = new QTableWidgetItem(student["join_date"].toString());
        joinItem->setFlags(joinItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 4, joinItem);
        QTableWidgetItem *goalItem = new QTableWidgetItem(student["study_goal"].toString());
        ui->tableWidget->setItem(row, 5, goalItem);
        QTableWidgetItem *progItem = new QTableWidgetItem(student["progress"].toString());
        progItem->setFlags(progItem->flags() & ~Qt::ItemIsEditable);
        ui->tableWidget->setItem(row, 6, progItem);

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

void StudentWidget::on_btnDeleteList_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的行");
        return;
    }

    QTableWidgetItem *item = ui->tableWidget->item(row, 0);
    if (!item) {
        QMessageBox::warning(this, "错误", "无法获取该行数据");
        return;
    }

    QString id = item->text();
    QString name = ui->tableWidget->item(row, 1) ? ui->tableWidget->item(row, 1)->text() : "";

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除学生 \"%1\" (编号: %2) 吗？").arg(name, id),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool success = Database::instance().deleteStudent(id);
        if (success) {
            QMessageBox::information(this, "成功", "学生信息删除成功");
            loadStudentData();
        } else {
            QMessageBox::warning(this, "失败", "学生信息删除失败");
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

void StudentWidget::on_tableWidget_cellDoubleClicked(int row, int column)
{
    // 第0列（编号）禁止编辑
    if (column == 0) return;

    QTableWidgetItem *idItem = ui->tableWidget->item(row, 0);
    if (!idItem) return;
    QString id = idItem->text();

    // 列号 -> 数据库字段
    QStringList fields = {"", "name", "gender", "birthday", "join_date", "study_goal", "progress", "photo"};
    if (column < 1 || column >= fields.size()) return;
    QString field = fields[column];

    // ---- 性别：右键弹出菜单 ----
    if (column == 2) {
        QMenu menu;
        QString cur = ui->tableWidget->item(row, column) ? ui->tableWidget->item(row, column)->text() : "";
        QAction *a1 = menu.addAction("男");
        QAction *a2 = menu.addAction("女");
        QAction *a3 = menu.addAction("保密");
        a1->setCheckable(true); a2->setCheckable(true); a3->setCheckable(true);
        if (cur == "男") a1->setChecked(true);
        else if (cur == "女") a2->setChecked(true);
        else if (cur == "保密") a3->setChecked(true);

        QRect cellRect = ui->tableWidget->visualItemRect(ui->tableWidget->item(row, column));
        QPoint pos = ui->tableWidget->viewport()->mapToGlobal(cellRect.bottomLeft());

        QAction *chosen = menu.exec(pos);
        if (chosen) {
            Database::instance().updateStudent(id, field, chosen->text());
            loadStudentData();
        }
    }
    // ---- 生日 / 加入时间：弹出日历 ----
    else if (column == 3 || column == 4) {
        QString cur = ui->tableWidget->item(row, column) ? ui->tableWidget->item(row, column)->text() : "";
        QDate d = QDate::fromString(cur, "yyyy-MM-dd");
        if (!d.isValid()) d = QDate::currentDate();

        QDialog *popup = new QDialog(this, Qt::Popup);
        QVBoxLayout *lay = new QVBoxLayout(popup);
        lay->setContentsMargins(0, 0, 0, 0);
        QCalendarWidget *cal = new QCalendarWidget();
        cal->setSelectedDate(d);
        lay->addWidget(cal);
        popup->adjustSize();

        QRect cellRect = ui->tableWidget->visualItemRect(ui->tableWidget->item(row, column));
        QPoint pos = ui->tableWidget->viewport()->mapToGlobal(cellRect.bottomLeft());
        popup->move(pos);
        popup->show();

        connect(cal, &QCalendarWidget::clicked, this, [this, id, field, popup](const QDate &date) {
            Database::instance().updateStudent(id, field, date.toString("yyyy-MM-dd"));
            popup->close();
            popup->deleteLater();
            loadStudentData();
        });
    }
    // ---- 当前进度：右键弹出菜单 ----
    else if (column == 6) {
        QMenu menu;
        QString cur = ui->tableWidget->item(row, column) ? ui->tableWidget->item(row, column)->text() : "";
        QStringList items = {"0%", "20%", "40%", "60%", "80%", "100%"};
        for (const QString &s : items) {
            QAction *a = menu.addAction(s);
            a->setCheckable(true);
            if (s == cur) a->setChecked(true);
        }

        QRect cellRect = ui->tableWidget->visualItemRect(ui->tableWidget->item(row, column));
        QPoint pos = ui->tableWidget->viewport()->mapToGlobal(cellRect.bottomLeft());

        QAction *chosen = menu.exec(pos);
        if (chosen) {
            Database::instance().updateStudent(id, field, chosen->text());
            loadStudentData();
        }
    }
    // ---- 照片：文件选择器 ----
    else if (column == 7) {
        QString fileName = QFileDialog::getOpenFileName(this, "选择照片", "", "图片文件 (*.jpg *.jpeg *.png *.bmp)");
        if (!fileName.isEmpty()) {
            QPixmap pixmap(fileName);
            if (!pixmap.isNull()) {
                QByteArray data;
                QBuffer buf(&data);
                buf.open(QIODevice::WriteOnly);
                pixmap.save(&buf, "JPG");
                Database::instance().updateStudent(id, field, data);
                loadStudentData();
            }
        }
    }
    // ---- 其他文本列：就地编辑 ----
    else {
        QTableWidgetItem *item = ui->tableWidget->item(row, column);
        if (item) {
            ui->tableWidget->editItem(item);
        }
    }
}
