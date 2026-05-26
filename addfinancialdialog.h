#ifndef ADDFINANCIALDIALOG_H
#define ADDFINANCIALDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
#include <QPushButton>

class AddFinancialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFinancialDialog(QWidget *parent = nullptr);
    ~AddFinancialDialog();

    QString getStudentId() const;
    QString getPaymentDate() const;
    double getAmount() const;
    QString getPaymentType() const;
    QString getNotes() const;

private slots:
    void onConfirm();
    void onCancel();

private:
    void loadStudents();

    QComboBox *cbStudent;
    QDateEdit *dePaymentDate;
    QLineEdit *leAmount;
    QComboBox *cbPaymentType;
    QLineEdit *leNotes;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;
};

#endif // ADDFINANCIALDIALOG_H
