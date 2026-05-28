#include "honorwallwidget.h"
#include "ui_honorwallwidget.h"
#include "database.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>

HonorwallWidget::HonorwallWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HonorwallWidget)
{
    ui->setupUi(this);

    // 初始化三个空位
    m_items = { HonorItem(), HonorItem(), HonorItem() };

    // 安装事件过滤器，监听图片标签点击
    ui->imageLabel1->installEventFilter(this);
    ui->imageLabel2->installEventFilter(this);
    ui->imageLabel3->installEventFilter(this);

    // 启用鼠标跟踪（可选，用于悬停效果）
    ui->imageLabel1->setCursor(Qt::PointingHandCursor);
    ui->imageLabel2->setCursor(Qt::PointingHandCursor);
    ui->imageLabel3->setCursor(Qt::PointingHandCursor);

    loadFromDatabase();
}

HonorwallWidget::~HonorwallWidget()
{
    delete ui;
}

bool HonorwallWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        for (int i = 0; i < 3; ++i) {
            if (obj == labelAt(i)) {
                selectSlot(i);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

QLabel* HonorwallWidget::labelAt(int index) const
{
    switch (index) {
        case 0: return ui->imageLabel1;
        case 1: return ui->imageLabel2;
        case 2: return ui->imageLabel3;
        default: return nullptr;
    }
}

void HonorwallWidget::selectSlot(int index)
{
    if (index < 0 || index >= 3) return;

    // 清除之前的高亮
    clearSelection();

    m_selectedIndex = index;

    // 高亮选中的标签：加粗边框（通过样式表）
    QString style = "QLabel {"
                    "  border: 3px solid #1890ff;"
                    "  background-color: #e6f7ff;"
                    "}";
    labelAt(index)->setStyleSheet(style);
}

void HonorwallWidget::clearSelection()
{
    if (m_selectedIndex >= 0) {
        // 还原边框样式
        labelAt(m_selectedIndex)->setStyleSheet(
            "QLabel { border: 1px solid palette(mid); }");
    }
    m_selectedIndex = -1;
}

void HonorwallWidget::loadFromDatabase()
{
    // 重置
    for (auto &item : m_items) {
        item = HonorItem();
    }

    auto records = Database::instance().getAllHonorWallRecords();
    int count = qMin(records.size(), 3);
    for (int i = 0; i < count; ++i) {
        m_items[i].dbId = records[i]["id"].toInt();
        m_items[i].imageData = records[i]["image_data"].toByteArray();
        m_items[i].description = records[i]["description"].toString();
    }

    updateImageDisplay();
}

void HonorwallWidget::updateImageDisplay()
{
    for (int i = 0; i < 3; ++i) {
        QLabel *label = labelAt(i);

        if (!m_items[i].imageData.isEmpty()) {
            QPixmap pix;
            pix.loadFromData(m_items[i].imageData);
            if (!pix.isNull()) {
                label->setPixmap(pix);
                label->setText(QString());
            } else {
                label->setText(tr("加载失败"));
                label->setPixmap(QPixmap());
            }
        } else {
            label->setPixmap(QPixmap());
            label->setText(QString());
        }

        // 还原选择状态边框
        if (i == m_selectedIndex) {
            label->setStyleSheet(
                "QLabel { border: 3px solid #1890ff; background-color: #e6f7ff; }");
        } else {
            label->setStyleSheet(
                "QLabel { border: 1px solid palette(mid); }");
        }
    }
}

void HonorwallWidget::on_btnAdd_clicked()
{
    // 找到第一个空位
    int emptySlot = -1;
    for (int i = 0; i < 3; ++i) {
        if (m_items[i].dbId == -1) {
            emptySlot = i;
            break;
        }
    }

    if (emptySlot == -1) {
        QMessageBox::information(this, tr("提示"), tr("最多只能添加 3 张图片"));
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择图片"),
        QString(),
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty()) return;

    // 读取文件
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("错误"), tr("无法打开图片文件"));
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    // 存入数据库
    QString desc = QFileInfo(filePath).fileName();
    if (Database::instance().addHonorWallRecord(data, desc)) {
        loadFromDatabase();
        clearSelection();
    } else {
        QMessageBox::warning(this, tr("错误"), tr("保存到数据库失败"));
    }
}

void HonorwallWidget::on_btnModify_clicked()
{
    if (m_selectedIndex < 0 || m_items[m_selectedIndex].dbId == -1) {
        QMessageBox::information(this, tr("提示"),
                                 tr("请先点击选中要修改的图片"));
        return;
    }

    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择新图片"),
        QString(),
        tr("图片文件 (*.png *.jpg *.jpeg *.bmp)"));

    if (filePath.isEmpty()) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, tr("错误"), tr("无法打开图片文件"));
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    int idx = m_selectedIndex;
    QString desc = QFileInfo(filePath).fileName();

    if (Database::instance().updateHonorWallRecord(m_items[idx].dbId, data, desc)) {
        loadFromDatabase();
        selectSlot(idx);  // 保持选中
    } else {
        QMessageBox::warning(this, tr("错误"), tr("更新数据库失败"));
    }
}

void HonorwallWidget::on_btnDelete_clicked()
{
    if (m_selectedIndex < 0 || m_items[m_selectedIndex].dbId == -1) {
        QMessageBox::information(this, tr("提示"),
                                 tr("请先点击选中要删除的图片"));
        return;
    }

    int ret = QMessageBox::question(this, tr("确认删除"),
                                     tr("确定要删除这张荣誉图片吗？"));
    if (ret != QMessageBox::Yes) return;

    int idx = m_selectedIndex;
    if (Database::instance().deleteHonorWallRecord(m_items[idx].dbId)) {
        loadFromDatabase();
    } else {
        QMessageBox::warning(this, tr("错误"), tr("删除失败"));
    }
}
