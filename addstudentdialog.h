#ifndef ADDSTUDENTDIALOG_H
#define ADDSTUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

class AddStudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddStudentDialog(QWidget *parent = nullptr);
    ~AddStudentDialog();

    QString getId() const;
    QString getName() const;
    QString getGender() const;
    QString getBirthday() const;
    QString getJoinDate() const;
    QString getStudyGoal() const;
    QString getProgress() const;
    QByteArray getPhotoData() const;

private slots:
    void onSelectPhoto();
    void onConfirm();
    void onCancel();

private:
    QLineEdit *leId;
    QLineEdit *leName;
    QComboBox *cbGender;
    QDateEdit *deBirthday;
    QDateEdit *deJoinDate;
    QLineEdit *leStudyGoal;
    QLineEdit *leProgress;
    QLabel *lblPhoto;
    QPushButton *btnSelectPhoto;
    QPushButton *btnConfirm;
    QPushButton *btnCancel;

    QByteArray photoData;
};

#endif // ADDSTUDENTDIALOG_H