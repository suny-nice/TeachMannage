#ifndef SCHEDULEWIDGET_H
#define SCHEDULEWIDGET_H

#include <QWidget>
#include <QMap>
#include <QTableWidget>

namespace Ui {
class ScheduleWidget;
}

class ScheduleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScheduleWidget(QWidget *parent = nullptr);
    ~ScheduleWidget();

private slots:
    void onYearChanged(int index);
    void onWeekChanged(int index);
    void onPrevWeek();
    void onNextWeek();
    void onAddCourse();
    void onDeleteCourse();
    void onTableItemChanged(QTableWidgetItem *item);

private:
    void initYearComboBox();
    void initWeekComboBox();
    void initTableWidget();
    void updateDateRangeLabel();
    void updateHeaders();
    void loadScheduleData();
    void saveScheduleData();

    Ui::ScheduleWidget *ui;
    QMap<QPair<int, int>, QVector<QVector<QString>>> scheduleData;
    QStringList times;
};

#endif // SCHEDULEWIDGET_H