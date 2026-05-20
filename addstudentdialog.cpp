#include "addstudentdialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>

AddStudentDialog::AddStudentDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("添加学生信息");
    setFixedSize(500, 420);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(15);

    QGroupBox *groupInfo = new QGroupBox("基本信息");
    QGridLayout *gridLayout = new QGridLayout(groupInfo);
    gridLayout->setSpacing(8);
    gridLayout->setContentsMargins(10, 10, 10, 10);

    gridLayout->addWidget(new QLabel("编号："), 0, 0);
    leId = new QLineEdit();
    gridLayout->addWidget(leId, 0, 1);

    gridLayout->addWidget(new QLabel("姓名："), 1, 0);
    leName = new QLineEdit();
    gridLayout->addWidget(leName, 1, 1);

    gridLayout->addWidget(new QLabel("性别："), 2, 0);
    cbGender = new QComboBox();
    cbGender->addItems({"男", "女"});
    gridLayout->addWidget(cbGender, 2, 1);

    gridLayout->addWidget(new QLabel("出生日期："), 3, 0);
    deBirthday = new QDateEdit(QDate::currentDate());
    deBirthday->setDisplayFormat("yyyy-MM-dd");
    gridLayout->addWidget(deBirthday, 3, 1);

    gridLayout->addWidget(new QLabel("入学日期："), 4, 0);
    deJoinDate = new QDateEdit(QDate::currentDate());
    deJoinDate->setDisplayFormat("yyyy-MM-dd");
    gridLayout->addWidget(deJoinDate, 4, 1);

    gridLayout->addWidget(new QLabel("学习目标："), 5, 0);
    leStudyGoal = new QLineEdit();
    gridLayout->addWidget(leStudyGoal, 5, 1);

    gridLayout->addWidget(new QLabel("当前进度："), 6, 0);
    leProgress = new QLineEdit();
    leProgress->setText("0%");
    gridLayout->addWidget(leProgress, 6, 1);

    contentLayout->addWidget(groupInfo);

    QGroupBox *groupPhoto = new QGroupBox("照片上传");
    QVBoxLayout *photoLayout = new QVBoxLayout(groupPhoto);
    photoLayout->setSpacing(10);
    photoLayout->setContentsMargins(10, 10, 10, 10);

    lblPhoto = new QLabel();
    lblPhoto->setFixedSize(150, 180);
    lblPhoto->setStyleSheet("border: 1px solid gray;");
    lblPhoto->setAlignment(Qt::AlignCenter);
    lblPhoto->setText("照片预览");
    photoLayout->addWidget(lblPhoto);

    btnSelectPhoto = new QPushButton("选择照片");
    photoLayout->addWidget(btnSelectPhoto);

    contentLayout->addWidget(groupPhoto);

    mainLayout->addLayout(contentLayout);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    
    btnCancel = new QPushButton("取消");
    btnCancel->setMinimumWidth(80);
    btnLayout->addWidget(btnCancel);
    
    btnConfirm = new QPushButton("确认");
    btnConfirm->setMinimumWidth(80);
    btnLayout->addWidget(btnConfirm);
    
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(btnSelectPhoto, &QPushButton::clicked, this, &AddStudentDialog::onSelectPhoto);
    connect(btnConfirm, &QPushButton::clicked, this, &AddStudentDialog::onConfirm);
    connect(btnCancel, &QPushButton::clicked, this, &AddStudentDialog::onCancel);
}

AddStudentDialog::~AddStudentDialog()
{
}

void AddStudentDialog::onSelectPhoto()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择照片", "", "图片文件 (*.jpg *.jpeg *.png *.bmp)");
    if (!fileName.isEmpty()) {
        QPixmap pixmap(fileName);
        if (!pixmap.isNull()) {
            QPixmap scaledPixmap = pixmap.scaled(lblPhoto->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            lblPhoto->setPixmap(scaledPixmap);

            QBuffer buffer(&photoData);
            buffer.open(QIODevice::WriteOnly);
            pixmap.save(&buffer, "JPG");
        }
    }
}

void AddStudentDialog::onConfirm()
{
    if (leId->text().isEmpty() || leName->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "编号和姓名不能为空");
        return;
    }
    accept();
}

void AddStudentDialog::onCancel()
{
    reject();
}

QString AddStudentDialog::getId() const
{
    return leId->text();
}

QString AddStudentDialog::getName() const
{
    return leName->text();
}

QString AddStudentDialog::getGender() const
{
    return cbGender->currentText();
}

QString AddStudentDialog::getBirthday() const
{
    return deBirthday->date().toString("yyyy-MM-dd");
}

QString AddStudentDialog::getJoinDate() const
{
    return deJoinDate->date().toString("yyyy-MM-dd");
}

QString AddStudentDialog::getStudyGoal() const
{
    return leStudyGoal->text();
}

QString AddStudentDialog::getProgress() const
{
    return leProgress->text();
}

QByteArray AddStudentDialog::getPhotoData() const
{
    return photoData;
}