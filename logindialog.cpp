#include "logindialog.h"
#include "ui_logindialog.h"
#include "database.h"
#include "setting.h"

#include <QFile>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // 从配置读取缓存
    if (Setting::instance().isLoginCacheEnabled()) {
        ui->editUser->setText(Setting::instance().getLastLoginUser());
        ui->chkRemember->setChecked(true);
    }

    // 加载样式表
    QFile file(":/style.qss");
    if (file.open(QFile::ReadOnly)) {
        setStyleSheet(QString::fromUtf8(file.readAll()));
        file.close();
    }

    // 回车键触发登录
    connect(ui->editPwd, &QLineEdit::returnPressed,
            this, &LoginDialog::on_btnLogin_clicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_btnLogin_clicked()
{
    QString username = ui->editUser->text().trimmed();
    QString password = ui->editPwd->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入用户名"));
        ui->editUser->setFocus();
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入密码"));
        ui->editPwd->setFocus();
        return;
    }

    if (Database::instance().authenticateUser(username, password)) {
        // 保存缓存
        Setting::instance().setLoginCacheEnabled(ui->chkRemember->isChecked());
        Setting::instance().setLastLoginUser(username);

        accept();  // 关闭对话框，返回 Accepted
    } else {
        QMessageBox::critical(this, tr("登录失败"), tr("用户名或密码错误"));
        ui->editPwd->selectAll();
        ui->editPwd->setFocus();
    }
}
