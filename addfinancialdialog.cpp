#include "addfinancialdialog.h"
#include "database.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QSqlError>
#include <QDoubleValidator>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <cmath>

AddFinancialDialog::AddFinancialDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("添加缴费记录");
    setFixedSize(300, 350);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QGroupBox *groupInfo = new QGroupBox("缴费信息");
    QFormLayout *formLayout = new QFormLayout(groupInfo);
    formLayout->setSpacing(10);
    formLayout->setContentsMargins(15, 15, 15, 15);

    // 学生姓名（从 studentInfo 表获取）
    cbStudent = new QComboBox();
    cbStudent->setMinimumWidth(120);
    loadStudents();
    formLayout->addRow("学生姓名：", cbStudent);

    // 缴费日期
    dePaymentDate = new QDateEdit(QDate::currentDate());
    dePaymentDate->setDisplayFormat("yyyy-MM-dd");
    dePaymentDate->setCalendarPopup(true);
    formLayout->addRow("缴费日期：", dePaymentDate);

    // 缴费金额（输入框，可输入小数）
    leAmount = new QLineEdit();
    leAmount->setValidator(new QDoubleValidator(0.01, 999999.99, 2, this));
    formLayout->addRow("缴费金额：", leAmount);

    // 支付类型（微信、支付宝、现金、银行卡）
    cbPaymentType = new QComboBox();
    cbPaymentType->addItems({"微信", "支付宝", "现金", "银行卡"});
    formLayout->addRow("支付类型：", cbPaymentType);

    // 备注
    leNotes = new QLineEdit();
    leNotes->setPlaceholderText("可选备注信息");
    formLayout->addRow("备注：", leNotes);

    mainLayout->addWidget(groupInfo);

    // 按钮
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    btnCancel = new QPushButton("取消");
    btnCancel->setMinimumWidth(70);
    btnLayout->addWidget(btnCancel);

    btnConfirm = new QPushButton("确认");
    btnConfirm->setMinimumWidth(70);
    btnLayout->addWidget(btnConfirm);

    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(btnConfirm, &QPushButton::clicked, this, &AddFinancialDialog::onConfirm);
    connect(btnCancel, &QPushButton::clicked, this, &AddFinancialDialog::onCancel);
}

AddFinancialDialog::~AddFinancialDialog()
{
}

void AddFinancialDialog::loadStudents()
{
    // 从 studentInfo 表加载学生姓名，只显示姓名，data 存储编号
    QSqlQuery query("SELECT id, name FROM studentInfo ORDER BY id");
    if (!query.exec()) {
        qDebug() << "加载学生列表失败:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        cbStudent->addItem(name, id);
    }
}

void AddFinancialDialog::onConfirm()
{
    if (cbStudent->count() == 0) {
        QMessageBox::warning(this, "警告", "请先添加学生信息");
        return;
    }

    bool ok = false;
    double amount = leAmount->text().toDouble(&ok);
    if (!ok || amount <= 0.0 || amount > 999999.99 || std::isnan(amount)) {
        QMessageBox::warning(this, "警告", "请输入有效的缴费金额（0.01 ~ 999999.99）");
        return;
    }

    // 备注安全过滤：移除控制字符，限制长度
    QString notes = leNotes->text();
    notes.remove(QRegularExpression("[\\x00-\\x08\\x0B\\x0C\\x0E-\\x1F]"));
    if (notes.length() > 200) {
        QMessageBox::warning(this, "警告", "备注不能超过 200 个字符");
        return;
    }
    leNotes->setText(notes);

    accept();
}

void AddFinancialDialog::onCancel()
{
    reject();
}

QString AddFinancialDialog::getStudentId() const
{
    return cbStudent->currentData().toString();
}

QString AddFinancialDialog::getPaymentDate() const
{
    return dePaymentDate->date().toString("yyyy-MM-dd");
}

double AddFinancialDialog::getAmount() const
{
    return leAmount->text().toDouble();
}

QString AddFinancialDialog::getPaymentType() const
{
    return cbPaymentType->currentText();
}

QString AddFinancialDialog::getNotes() const
{
    return leNotes->text();
}
