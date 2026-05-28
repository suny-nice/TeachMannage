#include "schedulewidget.h"
#include "ui_schedulewidget.h"
#include "database.h"
#include <QDate>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QTimeEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QMap>

ScheduleWidget::ScheduleWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScheduleWidget)
{
    ui->setupUi(this);

    times << "上午1" << "上午2" << "下午1" << "下午2" << "晚上1" << "晚上2";

    initYearComboBox();
    initWeekComboBox();
    initTableWidget();

    connect(ui->yearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleWidget::onYearChanged);
    connect(ui->weekComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ScheduleWidget::onWeekChanged);
    connect(ui->prevWeekBtn, &QPushButton::clicked, this, &ScheduleWidget::onPrevWeek);
    connect(ui->nextWeekBtn, &QPushButton::clicked, this, &ScheduleWidget::onNextWeek);
    connect(ui->addButton, &QPushButton::clicked, this, &ScheduleWidget::onAddCourse);
    connect(ui->deleteButton, &QPushButton::clicked, this, &ScheduleWidget::onDeleteCourse);
    connect(ui->tableWidget, &QTableWidget::itemChanged, this, &ScheduleWidget::onTableItemChanged);

    updateDateRangeLabel();
    loadScheduleData();
}

ScheduleWidget::~ScheduleWidget()
{
    delete ui;
}

void ScheduleWidget::initYearComboBox()
{
    int currentYear = QDate::currentDate().year();
    for (int year = 2020; year <= currentYear + 5; ++year) {
        ui->yearComboBox->addItem(QString::number(year), year);
    }
    ui->yearComboBox->setCurrentText(QString::number(currentYear));
}

void ScheduleWidget::initWeekComboBox()
{
    for (int week = 1; week <= 52; ++week) {
        ui->weekComboBox->addItem(QString("第 %1 周").arg(week), week);
    }
    int currentWeek = QDate::currentDate().weekNumber();
    ui->weekComboBox->setCurrentIndex(currentWeek - 1);
}

void ScheduleWidget::initTableWidget()
{
    ui->tableWidget->setRowCount(7);
    ui->tableWidget->setColumnCount(times.size());

    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setTextElideMode(Qt::ElideNone);

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < times.size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(i, j, item);
        }
    }
    
    updateHeaders();
}

void ScheduleWidget::updateDateRangeLabel()
{
    int year = ui->yearComboBox->currentData().toInt();
    int week = ui->weekComboBox->currentData().toInt();

    QDate startDate(year, 1, 1);
    int dayOfWeek = startDate.dayOfWeek();
    int daysToAdd = (week - 1) * 7 + (1 - dayOfWeek);
    startDate = startDate.addDays(daysToAdd);

    QDate endDate = startDate.addDays(6);

    ui->dateRangeLabel->setText(QString("%1-%2-%3到%4-%5-%6")
                                .arg(year).arg(startDate.month()).arg(startDate.day())
                                .arg(year).arg(endDate.month()).arg(endDate.day()));
    
    updateHeaders();
}

void ScheduleWidget::updateHeaders()
{
    int year = ui->yearComboBox->currentData().toInt();
    int week = ui->weekComboBox->currentData().toInt();

    QDate startDate(year, 1, 1);
    int dayOfWeek = startDate.dayOfWeek();
    int daysToAdd = (week - 1) * 7 + (1 - dayOfWeek);
    startDate = startDate.addDays(daysToAdd);

    QStringList weekDays;
    weekDays << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期日";
    
    for (int i = 0; i < 7; ++i) {
        QDate currentDate = startDate.addDays(i);
        QString headerText = QString("%1\n%2/%3")
                            .arg(weekDays[i])
                            .arg(currentDate.month())
                            .arg(currentDate.day());
        QTableWidgetItem *item = new QTableWidgetItem(headerText);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setVerticalHeaderItem(i, item);
    }
    
    for (int i = 0; i < times.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(times[i]);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setHorizontalHeaderItem(i, item);
    }
}

void ScheduleWidget::loadScheduleData()
{
    ui->tableWidget->blockSignals(true);

    int year = ui->yearComboBox->currentData().toInt();
    int week = ui->weekComboBox->currentData().toInt();
    QPair<int, int> key = qMakePair(year, week);

    // 缓存没有则从数据库加载
    if (!scheduleData.contains(key)) {
        QVector<QVector<QString>> data(7, QVector<QString>(times.size()));

        QDate startDate(year, 1, 1);
        int daysToAdd = (week - 1) * 7 + (1 - startDate.dayOfWeek());
        QDate monday = startDate.addDays(daysToAdd);

        QMap<QString, int> timeToCol = {
            {"09:00", 0}, {"11:00", 1}, {"14:00", 2},
            {"16:00", 3}, {"19:00", 4}, {"21:00", 5}
        };

        QSqlQuery query;
        query.prepare("SELECT date, time, course_name FROM schedule "
                       "WHERE date >= :m AND date <= :s");
        query.bindValue(":m", monday.toString("yyyy-MM-dd"));
        query.bindValue(":s", monday.addDays(6).toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                int dayIdx = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd").dayOfWeek() - 1;
                int col = timeToCol.value(query.value(1).toString(), -1);
                if (dayIdx >= 0 && dayIdx < 7 && col >= 0 && col < times.size())
                    data[dayIdx][col] = query.value(2).toString();
            }
        }
        scheduleData[key] = data;
    }

    QVector<QVector<QString>> data = scheduleData[key];
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < times.size(); ++j) {
            ui->tableWidget->item(i, j)->setText(data[i][j]);
        }
    }

    ui->tableWidget->blockSignals(false);
}

void ScheduleWidget::saveScheduleData()
{
    int year = ui->yearComboBox->currentData().toInt();
    int week = ui->weekComboBox->currentData().toInt();
    QPair<int, int> key = qMakePair(year, week);

    QVector<QVector<QString>> data(7, QVector<QString>(times.size()));
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < times.size(); ++j) {
            data[i][j] = ui->tableWidget->item(i, j)->text();
        }
    }

    scheduleData[key] = data;
}

void ScheduleWidget::onYearChanged(int index)
{
    Q_UNUSED(index);
    updateDateRangeLabel();
    loadScheduleData();
}

void ScheduleWidget::onWeekChanged(int index)
{
    Q_UNUSED(index);
    updateDateRangeLabel();
    loadScheduleData();
}

void ScheduleWidget::onPrevWeek()
{
    int currentIndex = ui->weekComboBox->currentIndex();
    if (currentIndex > 0) {
        ui->weekComboBox->setCurrentIndex(currentIndex - 1);
    } else {
        int yearIndex = ui->yearComboBox->currentIndex();
        if (yearIndex > 0) {
            ui->yearComboBox->setCurrentIndex(yearIndex - 1);
            ui->weekComboBox->setCurrentIndex(51);
        }
    }
}

void ScheduleWidget::onNextWeek()
{
    int currentIndex = ui->weekComboBox->currentIndex();
    if (currentIndex < 51) {
        ui->weekComboBox->setCurrentIndex(currentIndex + 1);
    } else {
        int yearIndex = ui->yearComboBox->currentIndex();
        if (yearIndex < ui->yearComboBox->count() - 1) {
            ui->yearComboBox->setCurrentIndex(yearIndex + 1);
            ui->weekComboBox->setCurrentIndex(0);
        }
    }
}

void ScheduleWidget::onAddCourse()
{
    ui->tableWidget->closePersistentEditor(ui->tableWidget->currentItem());
    
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择要添加课程的单元格");
        return;
    }
    
    if (selectedItems.size() > 1) {
        return;
    }
    
    int dayIndex = selectedItems[0]->row();
    int timeIndex = selectedItems[0]->column();

    if (!ui->tableWidget->item(dayIndex, timeIndex)->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "该时间段已被占用");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("添加课程");
    
    QFormLayout layout(&dialog);
    
    QComboBox* nameCombo = new QComboBox();
    QSqlQuery nameQuery("SELECT name FROM studentInfo");
    while (nameQuery.next()) {
        nameCombo->addItem(nameQuery.value(0).toString());
    }
    
    QTimeEdit* timeEdit = new QTimeEdit();
    timeEdit->setDisplayFormat("HH:mm");
    QMap<int, QTime> timePresets = {
        {0, QTime(9, 0)},
        {1, QTime(11, 0)},
        {2, QTime(14, 0)},
        {3, QTime(16, 0)},
        {4, QTime(19, 0)},
        {5, QTime(21, 0)}
    };
    if (timePresets.contains(timeIndex)) {
        timeEdit->setTime(timePresets[timeIndex]);
    }
    
    layout.addRow("学生姓名：", nameCombo);
    layout.addRow("课程时间：", timeEdit);
    
    QPushButton* cancelBtn = new QPushButton("取消");
    QPushButton* okBtn = new QPushButton("确定");
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(okBtn);
    layout.addRow(btnLayout);
    
    connect(okBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    if (dialog.exec() == QDialog::Accepted) {
        QString studentName = nameCombo->currentText();
        QString courseTime = timeEdit->time().toString("HH:mm");
        
        int year = ui->yearComboBox->currentData().toInt();
        int week = ui->weekComboBox->currentData().toInt();
        QDate startDate(year, 1, 1);
        int dayOfWeek = startDate.dayOfWeek();
        int daysToAdd = (week - 1) * 7 + (1 - dayOfWeek) + dayIndex;
        QDate date = startDate.addDays(daysToAdd);
        QString dateStr = date.toString("yyyy-MM-dd");
        
        QSqlQuery query;
        query.prepare("INSERT OR REPLACE INTO schedule (date, time, course_name) VALUES (:date, :time, :course_name)");
        query.bindValue(":date", dateStr);
        query.bindValue(":time", courseTime);
        query.bindValue(":course_name", studentName);
        
        if (query.exec()) {
            ui->tableWidget->item(dayIndex, timeIndex)->setText(studentName);
            saveScheduleData();
            QMessageBox::information(this, "提示", "课程添加成功");
        } else {
            QMessageBox::warning(this, "错误", "添加课程失败：" + query.lastError().text());
        }
    }
}

void ScheduleWidget::onDeleteCourse()
{
    ui->tableWidget->closePersistentEditor(ui->tableWidget->currentItem());
    
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();
    
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先选择要删除的课程");
        return;
    }
    
    int year = ui->yearComboBox->currentData().toInt();
    int week = ui->weekComboBox->currentData().toInt();
    QDate startDate(year, 1, 1);
    int dayOfWeek = startDate.dayOfWeek();
    
    QMap<int, QString> timeMap = {
        {0, "09:00"}, {1, "11:00"}, {2, "14:00"}, 
        {3, "16:00"}, {4, "19:00"}, {5, "21:00"}
    };
    
    for (QTableWidgetItem* item : selectedItems) {
        int dayIndex = item->row();
        int timeIndex = item->column();
        
        int daysToAdd = (week - 1) * 7 + (1 - dayOfWeek) + dayIndex;
        QDate date = startDate.addDays(daysToAdd);
        QString dateStr = date.toString("yyyy-MM-dd");
        
        QSqlQuery query;
        query.prepare("DELETE FROM schedule WHERE date = :date AND time = :time");
        query.bindValue(":date", dateStr);
        query.bindValue(":time", timeMap[timeIndex]);
        query.exec();
        
        ui->tableWidget->item(dayIndex, timeIndex)->setText("");
    }
    
    saveScheduleData();
}

void ScheduleWidget::onTableItemChanged(QTableWidgetItem *item)
{
    if (item) {
        saveScheduleData();
    }
}