# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 6.10.3
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QMainWindow, QPushButton,
    QSizePolicy, QStackedWidget, QStatusBar, QToolButton,
    QVBoxLayout, QWidget)
import img_rc

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(662, 487)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.horizontalLayout = QHBoxLayout(self.centralwidget)
        self.horizontalLayout.setSpacing(0)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.widget = QWidget(self.centralwidget)
        self.widget.setObjectName(u"widget")
        self.widget.setMinimumSize(QSize(80, 0))
        self.verticalLayout = QVBoxLayout(self.widget)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(0, 0, 1, 0)
        self.btnStudent = QToolButton(self.widget)
        self.btnStudent.setObjectName(u"btnStudent")
        self.btnStudent.setMinimumSize(QSize(64, 72))
        self.btnStudent.setMaximumSize(QSize(100, 72))
        icon = QIcon()
        icon.addFile(u":/img/xueyuanxinxi.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.btnStudent.setIcon(icon)
        self.btnStudent.setIconSize(QSize(40, 40))
        self.btnStudent.setCheckable(True)
        self.btnStudent.setChecked(False)
        self.btnStudent.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        self.verticalLayout.addWidget(self.btnStudent)

        self.btnShedule = QToolButton(self.widget)
        self.btnShedule.setObjectName(u"btnShedule")
        self.btnShedule.setMinimumSize(QSize(64, 72))
        self.btnShedule.setMaximumSize(QSize(100, 72))
        icon1 = QIcon()
        icon1.addFile(u":/img/kechengguanli.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.btnShedule.setIcon(icon1)
        self.btnShedule.setIconSize(QSize(40, 40))
        self.btnShedule.setCheckable(True)
        self.btnShedule.setChecked(False)
        self.btnShedule.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        self.verticalLayout.addWidget(self.btnShedule)

        self.btnFinance = QToolButton(self.widget)
        self.btnFinance.setObjectName(u"btnFinance")
        self.btnFinance.setMinimumSize(QSize(64, 72))
        self.btnFinance.setMaximumSize(QSize(100, 72))
        icon2 = QIcon()
        icon2.addFile(u":/img/caiwuguanli.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.btnFinance.setIcon(icon2)
        self.btnFinance.setIconSize(QSize(40, 40))
        self.btnFinance.setCheckable(True)
        self.btnFinance.setChecked(False)
        self.btnFinance.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        self.verticalLayout.addWidget(self.btnFinance)

        self.btnHonor = QToolButton(self.widget)
        self.btnHonor.setObjectName(u"btnHonor")
        self.btnHonor.setMinimumSize(QSize(64, 72))
        self.btnHonor.setMaximumSize(QSize(100, 72))
        icon3 = QIcon()
        icon3.addFile(u":/img/rongyuqiang.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.btnHonor.setIcon(icon3)
        self.btnHonor.setIconSize(QSize(40, 40))
        self.btnHonor.setCheckable(True)
        self.btnHonor.setChecked(False)
        self.btnHonor.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        self.verticalLayout.addWidget(self.btnHonor)

        self.btnSystemSetting = QToolButton(self.widget)
        self.btnSystemSetting.setObjectName(u"btnSystemSetting")
        self.btnSystemSetting.setMinimumSize(QSize(64, 72))
        self.btnSystemSetting.setMaximumSize(QSize(100, 72))
        icon4 = QIcon()
        icon4.addFile(u":/img/xitongshezhi.png", QSize(), QIcon.Mode.Normal, QIcon.State.Off)
        self.btnSystemSetting.setIcon(icon4)
        self.btnSystemSetting.setIconSize(QSize(40, 40))
        self.btnSystemSetting.setCheckable(True)
        self.btnSystemSetting.setChecked(False)
        self.btnSystemSetting.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)

        self.verticalLayout.addWidget(self.btnSystemSetting)


        self.horizontalLayout.addWidget(self.widget)

        self.stackedWidget = QStackedWidget(self.centralwidget)
        self.stackedWidget.setObjectName(u"stackedWidget")
        self.pageStudent = QWidget()
        self.pageStudent.setObjectName(u"pageStudent")
        self.pushButton = QPushButton(self.pageStudent)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setGeometry(QRect(180, 120, 100, 32))
        self.stackedWidget.addWidget(self.pageStudent)
        self.pageShedule = QWidget()
        self.pageShedule.setObjectName(u"pageShedule")
        self.pushButton_2 = QPushButton(self.pageShedule)
        self.pushButton_2.setObjectName(u"pushButton_2")
        self.pushButton_2.setGeometry(QRect(150, 110, 100, 32))
        self.stackedWidget.addWidget(self.pageShedule)
        self.pageFinance = QWidget()
        self.pageFinance.setObjectName(u"pageFinance")
        self.pushButton_3 = QPushButton(self.pageFinance)
        self.pushButton_3.setObjectName(u"pushButton_3")
        self.pushButton_3.setGeometry(QRect(200, 130, 100, 32))
        self.stackedWidget.addWidget(self.pageFinance)
        self.pageHonor = QWidget()
        self.pageHonor.setObjectName(u"pageHonor")
        self.pushButton_4 = QPushButton(self.pageHonor)
        self.pushButton_4.setObjectName(u"pushButton_4")
        self.pushButton_4.setGeometry(QRect(220, 160, 100, 32))
        self.stackedWidget.addWidget(self.pageHonor)
        self.pageSystemSetting = QWidget()
        self.pageSystemSetting.setObjectName(u"pageSystemSetting")
        self.pushButton_5 = QPushButton(self.pageSystemSetting)
        self.pushButton_5.setObjectName(u"pushButton_5")
        self.pushButton_5.setGeometry(QRect(160, 170, 100, 32))
        self.stackedWidget.addWidget(self.pageSystemSetting)

        self.horizontalLayout.addWidget(self.stackedWidget)

        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)

        self.stackedWidget.setCurrentIndex(4)


        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.btnStudent.setText(QCoreApplication.translate("MainWindow", u"\u5b66\u751f\u4fe1\u606f", None))
        self.btnShedule.setText(QCoreApplication.translate("MainWindow", u"\u8bfe\u7a0b\u5b89\u6392", None))
        self.btnFinance.setText(QCoreApplication.translate("MainWindow", u"\u8d22\u52a1", None))
        self.btnHonor.setText(QCoreApplication.translate("MainWindow", u"\u8363\u8a89\u5899", None))
        self.btnSystemSetting.setText(QCoreApplication.translate("MainWindow", u"\u7cfb\u7edf\u8bbe\u7f6e", None))
        self.pushButton.setText(QCoreApplication.translate("MainWindow", u"1", None))
        self.pushButton_2.setText(QCoreApplication.translate("MainWindow", u"2", None))
        self.pushButton_3.setText(QCoreApplication.translate("MainWindow", u"3", None))
        self.pushButton_4.setText(QCoreApplication.translate("MainWindow", u"4", None))
        self.pushButton_5.setText(QCoreApplication.translate("MainWindow", u"5", None))
    # retranslateUi

