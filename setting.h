#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

    // 单例 — 仅用于配置读写，不管理 UI
    static Setting& instance();

    QString getDatabasePath() const;
    void setDatabasePath(const QString& path);
    bool isLoginCacheEnabled() const;
    void setLoginCacheEnabled(bool enabled);
    QString getLastLoginUser() const;
    void setLastLoginUser(const QString& username);

private slots:
    void on_btnBrowse_clicked();
    void on_btnSave_clicked();

private:
    void loadSettings();

    Ui::Setting *ui;
    QSettings m_settings;
};

#endif // SETTING_H
