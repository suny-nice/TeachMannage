#include "setting.h"
#include "ui_setting.h"
#include "database.h"

#include <QFileDialog>
#include <QMessageBox>

Setting::Setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Setting)
    , m_settings("config/app.ini", QSettings::IniFormat)
{
    ui->setupUi(this);
    loadSettings();
}

Setting::~Setting()
{
    delete ui;
}

Setting& Setting::instance()
{
    static Setting instance;
    return instance;
}

// === 配置读写 ===
QString Setting::getDatabasePath() const
{
    return m_settings.value("General/dbPath").toString();
}

void Setting::setDatabasePath(const QString& path)
{
    m_settings.setValue("General/dbPath", path);
}

bool Setting::isLoginCacheEnabled() const
{
    return m_settings.value("Login/loginCache", false).toBool();
}

void Setting::setLoginCacheEnabled(bool enabled)
{
    m_settings.setValue("Login/loginCache", enabled);
}

QString Setting::getLastLoginUser() const
{
    return m_settings.value("Login/lastLoginUser").toString();
}

void Setting::setLastLoginUser(const QString& username)
{
    m_settings.setValue("Login/lastLoginUser", username);
}

// === 界面 ===
void Setting::loadSettings()
{
    QString dbPath = getDatabasePath();
    if (dbPath.isEmpty()) {
        dbPath = Database::instance().getDatabasePath();
    }
    ui->editDbPath->setText(dbPath);
    ui->chkLoginCache->setChecked(isLoginCacheEnabled());
    ui->editOldPwd->clear();
    ui->editNewPwd->clear();
    ui->editConfirmPwd->clear();
}

void Setting::on_btnBrowse_clicked()
{
    QString path = QFileDialog::getOpenFileName(
        this, tr("选择数据库文件"),
        ui->editDbPath->text(),
        tr("数据库文件 (*.db)"));
    if (!path.isEmpty()) {
        ui->editDbPath->setText(path);
    }
}

void Setting::on_btnSave_clicked()
{
    QString oldPath = getDatabasePath();
    QString newPath = ui->editDbPath->text();
    bool dbChanged = (oldPath != newPath);

    setDatabasePath(newPath);
    setLoginCacheEnabled(ui->chkLoginCache->isChecked());

    // 如果填写了密码信息，同时修改密码
    QString oldPwd = ui->editOldPwd->text();
    QString newPwd = ui->editNewPwd->text();
    QString confirmPwd = ui->editConfirmPwd->text();

    if (!oldPwd.isEmpty() || !newPwd.isEmpty() || !confirmPwd.isEmpty()) {
        if (oldPwd.isEmpty() || newPwd.isEmpty() || confirmPwd.isEmpty()) {
            QMessageBox::warning(this, tr("提示"), tr("请填写完整的密码信息"));
            return;
        }
        if (newPwd != confirmPwd) {
            QMessageBox::warning(this, tr("提示"), tr("两次输入的新密码不一致"));
            return;
        }

        QString username = getLastLoginUser();
        if (username.isEmpty()) username = "admin";

        if (!Database::instance().changePassword(username, oldPwd, newPwd)) {
            QMessageBox::critical(this, tr("错误"), tr("当前密码错误"));
            return;
        }
    }

    // 清空密码框
    ui->editOldPwd->clear();
    ui->editNewPwd->clear();
    ui->editConfirmPwd->clear();

    if (dbChanged) {
        QMessageBox::information(this, tr("提示"),
            tr("数据库路径已修改，重启程序后生效。"));
    } else {
        QMessageBox::information(this, tr("提示"), tr("设置已保存。"));
    }
}
