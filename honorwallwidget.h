#ifndef HONORWALLWIDGET_H
#define HONORWALLWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QList>
#include <QMap>
#include <QVariant>

namespace Ui {
class HonorwallWidget;
}

struct HonorItem {
    int dbId = -1;           // 数据库中 id，-1 表示空位
    QByteArray imageData;    // 图片二进制数据
    QString description;     // 描述
};

class HonorwallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HonorwallWidget(QWidget *parent = nullptr);
    ~HonorwallWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_btnAdd_clicked();
    void on_btnModify_clicked();
    void on_btnDelete_clicked();

private:
    void loadFromDatabase();
    void updateImageDisplay();
    void selectSlot(int index);
    void clearSelection();
    QLabel* labelAt(int index) const;

    Ui::HonorwallWidget *ui;
    QList<HonorItem> m_items;   // 固定 3 个元素
    int m_selectedIndex = -1;   // -1 = 未选中
};

#endif // HONORWALLWIDGET_H
