#ifndef FINANCIALWIDGET_H
#define FINANCIALWIDGET_H

#include <QWidget>
#include <QChart>
#include <QChartView>
#include <QPieSeries>
#include <QLineSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QMap>
#include <QVariant>

namespace Ui {
class FinancialWidget;
}

class FinancialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinancialWidget(QWidget *parent = nullptr);
    ~FinancialWidget();

    void loadData();

private slots:
    void onFilterChanged();
    void onAddClicked();
    void onDeleteClicked();
    void onModifyClicked();

private:
    void setupCharts();
    void refreshPieChart(const QList<QMap<QString, QVariant>>& records);
    void refreshLineChart(const QList<QMap<QString, QVariant>>& records);
    void populateStudentCombo();
    void applyFilter();
    void appendTableRow(const QMap<QString, QVariant>& rec);
    int  findRecordId(int row) const;

    Ui::FinancialWidget *ui;

    // 全部记录缓存（用于筛选）
    QList<QMap<QString, QVariant>> allRecords;

    // 饼状图
    QChart *pieChart;
    QChartView *pieChartView;
    QPieSeries *pieSeries;

    // 折线图
    QChart *lineChart;
    QChartView *lineChartView;
    QLineSeries *lineSeries;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
};

#endif // FINANCIALWIDGET_H
