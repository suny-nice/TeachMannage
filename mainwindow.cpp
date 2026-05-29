#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"
#include "studentwidget.h"
#include "financialwidget.h"
#include <QFile>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , buttonGroup(new QButtonGroup(this))
{
    Database::instance().openDatabase();

    ui->setupUi(this);

    loadStyleSheet();
    
    ui->pageStudent->loadStudentData();
    ui->pageFinance->loadData();

    buttonGroup->addButton(ui->btnStudent, 0);
    buttonGroup->addButton(ui->btnShedule, 1);
    buttonGroup->addButton(ui->btnFinance, 2);
    buttonGroup->addButton(ui->btnHonor, 3);
    buttonGroup->addButton(ui->btnSystemSetting, 4);

    connect(buttonGroup, &QButtonGroup::idClicked, ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    buttonGroup->button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
