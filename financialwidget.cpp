#include "financialwidget.h"
#include "ui_financialwidget.h"
#include "database.h"
#include "addfinancialdialog.h"

#include <QMessageBox>
#include <QTableWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMap>
#include <QDateTime>
#include <algorithm>

// ──────────────────────────────────────────────
// 构造函数 / 析构函数
// ──────────────────────────────────────────────

FinancialWidget::FinancialWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FinancialWidget)
    , pieChart(nullptr)
    , pieChartView(nullptr)
    , pieSeries(nullptr)
    , lineChart(nullptr)
    , lineChartView(nullptr)
    , lineSeries(nullptr)
    , axisX(nullptr)
    , axisY(nullptr)
{
    ui->setupUi(this);

    // 垂直布局比例：上(标题+表格+饼图)60% : 下(折线图)40%
    ui->verticalLayout->setStretch(0, 6);
    ui->verticalLayout->setStretch(1, 4);

    // 配置表格
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(30);
    ui->tableWidget->verticalHeader()->setVisible(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setTextElideMode(Qt::ElideNone);

    // 初始化日期筛选控件（默认显示"不限"）
    ui->dateStart->setDate(QDate(2025, 1, 1));
    ui->dateStart->setSpecialValueText("不限");
    ui->dateEnd->setDate(QDate::currentDate());
    ui->dateEnd->setSpecialValueText("不限");

    // 初始化图表
    setupCharts();

    // ── 信号连接 ──

    // 筛选条件变化
    connect(ui->comboStudent, &QComboBox::currentTextChanged,
            this, &FinancialWidget::onFilterChanged);
    connect(ui->dateStart, &QDateEdit::dateChanged,
            this, &FinancialWidget::onFilterChanged);
    connect(ui->dateEnd, &QDateEdit::dateChanged,
            this, &FinancialWidget::onFilterChanged);

    // 操作按钮
    connect(ui->btnAddFilter, &QPushButton::clicked,
            this, &FinancialWidget::onAddClicked);
    connect(ui->btnDelete, &QPushButton::clicked,
            this, &FinancialWidget::onDeleteClicked);
    connect(ui->btnModify, &QPushButton::clicked,
            this, &FinancialWidget::onModifyClicked);
}

FinancialWidget::~FinancialWidget()
{
    delete ui;
}

// ──────────────────────────────────────────────
// 初始化图表容器
// ──────────────────────────────────────────────

void FinancialWidget::setupCharts()
{
    // ---------- 饼状图 ----------
    pieChart = new QChart();
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->legend()->setAlignment(Qt::AlignBottom); // 图例在饼图下方
    pieChart->legend()->hide();                        // 无数据时完全空白
    pieChart->setMargins(QMargins(0, 0, 0, 0));
    pieChart->setBackgroundRoundness(0);

    pieSeries = new QPieSeries();
    pieChart->addSeries(pieSeries);

    pieChartView = new QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieChartView->setFrameShape(QFrame::NoFrame);
    pieChartView->setStyleSheet("QChartView { border: none; background: transparent; }");
    pieChartView->viewport()->setAutoFillBackground(false);
    pieChart->setBackgroundPen(QPen(Qt::NoPen));
    // 图例字体（方块+类型+金额）
    QFont legendFont = pieChart->legend()->font();
    legendFont.setPointSize(9);
    pieChart->legend()->setFont(legendFont);

    // 替换占位 QWidget 为 QChartView
    QVBoxLayout *pieLayout = new QVBoxLayout(ui->pieChartWidget);
    pieLayout->setContentsMargins(0, 0, 0, 0);
    pieLayout->addWidget(pieChartView);

    // ---------- 折线图 ----------
    lineChart = new QChart();
    lineChart->setAnimationOptions(QChart::SeriesAnimations);
    lineChart->legend()->hide();
    lineChart->setMargins(QMargins(0, 0, 0, 0));
    lineChart->setBackgroundRoundness(0);

    lineSeries = new QLineSeries();
    lineSeries->setName("缴费金额");
    lineSeries->setColor(QColor("#E74C3C"));
    lineSeries->setPointsVisible(true);
    lineChart->addSeries(lineSeries);

    // X 轴（日期）
    axisX = new QDateTimeAxis();
    axisX->setFormat("MM-dd");
    axisX->setLabelsAngle(-45);
    axisX->setTitleText("日期");
    lineChart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    // Y 轴（金额）
    axisY = new QValueAxis();
    axisY->setLabelFormat("%.0f");
    axisY->setTitleText("金额(元)");
    lineChart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    lineChartView = new QChartView(lineChart);
    lineChartView->setRenderHint(QPainter::Antialiasing);
    lineChartView->setFrameShape(QFrame::NoFrame);
    lineChartView->setStyleSheet("QChartView { border: none; background: transparent; }");
    lineChartView->viewport()->setAutoFillBackground(false);
    lineChart->setBackgroundPen(QPen(Qt::NoPen));

    QVBoxLayout *lineLayout = new QVBoxLayout(ui->lineChartWidget);
    lineLayout->setContentsMargins(0, 0, 0, 0);
    lineLayout->addWidget(lineChartView);
}

// ──────────────────────────────────────────────
// 加载全部数据
// ──────────────────────────────────────────────

void FinancialWidget::loadData()
{
    allRecords = Database::instance().getAllFinancialRecords();

    // 刷新学生下拉框
    populateStudentCombo();

    // 应用当前筛选条件
    applyFilter();
}

// ──────────────────────────────────────────────
// 填充学生下拉框
// ──────────────────────────────────────────────

void FinancialWidget::populateStudentCombo()
{
    QString current = ui->comboStudent->currentText();
    ui->comboStudent->blockSignals(true);
    ui->comboStudent->clear();
    ui->comboStudent->addItem("全部");

    QList<QMap<QString, QVariant>> students = Database::instance().getAllStudents();
    for (const auto& s : students) {
        ui->comboStudent->addItem(s["name"].toString());
    }

    // 恢复选中项
    int idx = ui->comboStudent->findText(current);
    if (idx >= 0) ui->comboStudent->setCurrentIndex(idx);
    ui->comboStudent->blockSignals(false);
}

// ──────────────────────────────────────────────
// 筛选条件变化
// ──────────────────────────────────────────────

void FinancialWidget::onFilterChanged()
{
    applyFilter();
}

// ──────────────────────────────────────────────
// 应用筛选条件 → 刷新表格 + 图表
// ──────────────────────────────────────────────

void FinancialWidget::applyFilter()
{
    QString selectedName = ui->comboStudent->currentText();
    QDate startDate = ui->dateStart->date();
    QDate endDate   = ui->dateEnd->date();

    // 筛选
    QList<QMap<QString, QVariant>> filtered;
    for (const auto& rec : allRecords) {
        // 学生姓名
        if (selectedName != "全部" && rec["student_name"].toString() != selectedName)
            continue;

        // 日期范围
        QDate date = QDate::fromString(rec["payment_date"].toString(), "yyyy-MM-dd");
        if (!date.isValid()) continue;
        if (date < startDate || date > endDate) continue;

        filtered.append(rec);
    }

    // 刷新表格
    ui->tableWidget->setRowCount(0);
    for (const auto& rec : filtered)
        appendTableRow(rec);

    // 刷新图表（用筛选后的数据）
    refreshPieChart(filtered);
    refreshLineChart(filtered);
}

// ──────────────────────────────────────────────
// 饼状图：按缴费类型汇总金额
// ──────────────────────────────────────────────

void FinancialWidget::refreshPieChart(const QList<QMap<QString, QVariant>>& records)
{
    pieSeries->clear();

    // 空数据 → 完全空白
    if (records.isEmpty()) {
        pieChart->legend()->hide();
        return;
    }
    pieChart->legend()->show();

    // 按 payment_type 汇总金额
    QMap<QString, double> typeSum;
    double total = 0.0;

    for (const QMap<QString, QVariant>& rec : records) {
        QString type = rec["payment_type"].toString();
        double amount = rec["amount"].toDouble();
        typeSum[type] += amount;
        total += amount;
    }

    // 颜色映射
    QStringList colors = {"#3498DB", "#E74C3C", "#2ECC71", "#F39C12", "#9B59B6"};

    // 按金额降序排列
    QStringList typeKeys = typeSum.keys();
    std::sort(typeKeys.begin(), typeKeys.end(), [&](const QString &a, const QString &b) {
        return typeSum[a] > typeSum[b];
    });

    int colorIdx = 0;
    for (const QString &type : typeKeys) {
        QPieSlice *slice = pieSeries->append(type, typeSum[type]);
        slice->setColor(QColor(colors[colorIdx % colors.size()]));
        // 图例显示 "类型  ¥金额"（标签不在饼图上显示）
        slice->setLabel(QString("%1  ¥%2")
            .arg(type).arg(typeSum[type], 0, 'f', 0));
        slice->setLabelVisible(false);
        slice->setExploded(false);
        slice->setBorderWidth(1);
        colorIdx++;
    }
}

// ──────────────────────────────────────────────
// 折线图：按日期汇总总金额
// ──────────────────────────────────────────────

void FinancialWidget::refreshLineChart(const QList<QMap<QString, QVariant>>& records)
{
    lineSeries->clear();

    // 空数据 → 完全空白（隐藏坐标轴）
    if (records.isEmpty()) {
        axisX->setVisible(false);
        axisY->setVisible(false);
        return;
    }
    axisX->setVisible(true);
    axisY->setVisible(true);

    // 按日期汇总金额（同一天可能有多个记录）
    QMap<QDate, double> dateSum;
    QDate minDate, maxDate;

    for (const QMap<QString, QVariant>& rec : records) {
        QDate date = QDate::fromString(rec["payment_date"].toString(), "yyyy-MM-dd");
        if (!date.isValid()) continue;

        double amount = rec["amount"].toDouble();
        dateSum[date] += amount;

        if (!minDate.isValid() || date < minDate) minDate = date;
        if (!maxDate.isValid() || date > maxDate) maxDate = date;
    }

    // 填充数据点
    for (auto it = dateSum.begin(); it != dateSum.end(); ++it) {
        QDateTime dt(it.key(), QTime(0, 0));
        lineSeries->append(dt.toMSecsSinceEpoch(), it.value());
    }

    // 设置 X 轴范围
    if (minDate.isValid() && maxDate.isValid()) {
        // 前后留出 1 天边距
        QDateTime minDt(minDate.addDays(-1), QTime(0, 0));
        QDateTime maxDt(maxDate.addDays(1), QTime(0, 0));
        axisX->setRange(minDt, maxDt);

        // 如果只有一天数据，手动扩展范围让折线不贴边
        if (minDate == maxDate) {
            axisX->setRange(minDt.addDays(-1), maxDt.addDays(1));
        }
    } else {
        // 无数据时显示默认范围
        QDateTime now = QDateTime::currentDateTime();
        axisX->setRange(now.addDays(-30), now.addDays(1));
    }

    // 设置 Y 轴范围
    double maxAmount = 0.0;
    for (auto it = dateSum.begin(); it != dateSum.end(); ++it) {
        if (it.value() > maxAmount) maxAmount = it.value();
    }

    if (maxAmount > 0.0) {
        axisY->setRange(0, maxAmount * 1.15);  // 顶部留 15% 余量
    } else {
        axisY->setRange(0, 100);
    }
}

// ──────────────────────────────────────────────
// 辅助方法
// ──────────────────────────────────────────────

void FinancialWidget::appendTableRow(const QMap<QString, QVariant>& rec)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    auto setCell = [&](int col, const QString& text) {
        QTableWidgetItem *item = new QTableWidgetItem(text);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row, col, item);
    };

    setCell(0, rec["student_name"].toString());
    setCell(1, rec["payment_date"].toString());
    setCell(2, QString::number(rec["amount"].toDouble(), 'f', 2));
    setCell(3, rec["payment_type"].toString());
    setCell(4, rec["notes"].toString());
}

int FinancialWidget::findRecordId(int row) const
{
    if (row < 0 || row >= ui->tableWidget->rowCount())
        return -1;

    QString name = ui->tableWidget->item(row, 0)->text();
    QString date = ui->tableWidget->item(row, 1)->text();
    double  amt  = ui->tableWidget->item(row, 2)->text().toDouble();

    for (const auto& rec : allRecords) {
        if (rec["student_name"].toString() == name &&
            rec["payment_date"].toString() == date &&
            qAbs(rec["amount"].toDouble() - amt) < 0.001) {
            return rec["id"].toInt();
        }
    }
    return -1;
}

// ──────────────────────────────────────────────
// 添加按钮
// ──────────────────────────────────────────────

void FinancialWidget::onAddClicked()
{
    AddFinancialDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString studentId = dialog.getStudentId();
        QString paymentDate = dialog.getPaymentDate();
        double amount = dialog.getAmount();
        QString paymentType = dialog.getPaymentType();
        QString notes = dialog.getNotes();

        bool success = Database::instance().addFinancialRecord(
            studentId, paymentDate, amount, paymentType, notes);

        if (success) {
            QMessageBox::information(this, "成功", "缴费记录添加成功");
            loadData();
        } else {
            QMessageBox::warning(this, "失败", "缴费记录添加失败");
        }
    }
}

// ──────────────────────────────────────────────
// 删除按钮
// ──────────────────────────────────────────────

void FinancialWidget::onDeleteClicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要删除的缴费记录");
        return;
    }

    QString name = ui->tableWidget->item(row, 0) ? ui->tableWidget->item(row, 0)->text() : "";
    QString date = ui->tableWidget->item(row, 1) ? ui->tableWidget->item(row, 1)->text() : "";

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认删除",
        QString("确定要删除 \"%1\" 在 %2 的缴费记录吗？").arg(name, date),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply != QMessageBox::Yes) return;

    int recordId = findRecordId(row);
    if (recordId < 0) {
        QMessageBox::warning(this, "错误", "无法定位该记录");
        return;
    }

    bool success = Database::instance().deleteFinancialRecord(recordId);
    if (success) {
        QMessageBox::information(this, "成功", "缴费记录删除成功");
        loadData();
    } else {
        QMessageBox::warning(this, "失败", "缴费记录删除失败");
    }
}

// ──────────────────────────────────────────────
// 修改按钮
// ──────────────────────────────────────────────

void FinancialWidget::onModifyClicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "提示", "请先选择要修改的缴费记录");
        return;
    }

    int recordId = findRecordId(row);
    if (recordId < 0) {
        QMessageBox::warning(this, "错误", "无法定位该记录");
        return;
    }

    // 弹出修改对话框
    AddFinancialDialog dialog(this);
    dialog.setWindowTitle("修改缴费记录");

    if (dialog.exec() == QDialog::Accepted) {
        QString newDate   = dialog.getPaymentDate();
        double  newAmount = dialog.getAmount();
        QString newType   = dialog.getPaymentType();
        QString newNotes  = dialog.getNotes();

        bool success = Database::instance().updateFinancialRecord(
            recordId, newDate, newAmount, newType, newNotes);

        if (success) {
            QMessageBox::information(this, "成功", "缴费记录修改成功");
            loadData();
        } else {
            QMessageBox::warning(this, "失败", "缴费记录修改失败");
        }
    }
}
