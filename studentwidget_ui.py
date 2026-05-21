# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'studentwidget.ui'
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
from PySide6.QtWidgets import (QApplication, QHBoxLayout, QHeaderView, QPushButton,
    QSizePolicy, QSpacerItem, QTableWidget, QTableWidgetItem,
    QVBoxLayout, QWidget)

class Ui_StudentWidget(object):
    def setupUi(self, StudentWidget):
        if not StudentWidget.objectName():
            StudentWidget.setObjectName(u"StudentWidget")
        StudentWidget.resize(602, 487)
        self.horizontalLayout = QHBoxLayout(StudentWidget)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.horizontalLayout.setContentsMargins(0, 0, 0, 0)
        self.tableWidget = QTableWidget(StudentWidget)
        if (self.tableWidget.columnCount() < 8):
            self.tableWidget.setColumnCount(8)
        __qtablewidgetitem = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(0, __qtablewidgetitem)
        __qtablewidgetitem1 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(1, __qtablewidgetitem1)
        __qtablewidgetitem2 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(2, __qtablewidgetitem2)
        __qtablewidgetitem3 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(3, __qtablewidgetitem3)
        __qtablewidgetitem4 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(4, __qtablewidgetitem4)
        __qtablewidgetitem5 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(5, __qtablewidgetitem5)
        __qtablewidgetitem6 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(6, __qtablewidgetitem6)
        __qtablewidgetitem7 = QTableWidgetItem()
        self.tableWidget.setHorizontalHeaderItem(7, __qtablewidgetitem7)
        if (self.tableWidget.rowCount() < 1):
            self.tableWidget.setRowCount(1)
        self.tableWidget.setObjectName(u"tableWidget")
        self.tableWidget.setMinimumSize(QSize(0, 0))
        self.tableWidget.setBaseSize(QSize(0, 0))
        self.tableWidget.horizontalHeader().setDefaultSectionSize(100)
        self.tableWidget.horizontalHeader().setStretchLastSection(False)

        self.horizontalLayout.addWidget(self.tableWidget)

        self.verticalLayout = QVBoxLayout()
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.verticalLayout.setContentsMargins(1, -1, 5, -1)
        self.verticalSpacer = QSpacerItem(20, 20, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer)

        self.pushButton = QPushButton(StudentWidget)
        self.pushButton.setObjectName(u"pushButton")
        self.pushButton.setMinimumSize(QSize(20, 30))

        self.verticalLayout.addWidget(self.pushButton)

        self.verticalSpacer_2 = QSpacerItem(20, 30, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer_2)

        self.btnDeleteItem = QPushButton(StudentWidget)
        self.btnDeleteItem.setObjectName(u"btnDeleteItem")

        self.verticalLayout.addWidget(self.btnDeleteItem)

        self.btnDeleteList = QPushButton(StudentWidget)
        self.btnDeleteList.setObjectName(u"btnDeleteList")

        self.verticalLayout.addWidget(self.btnDeleteList)

        self.verticalSpacer_3 = QSpacerItem(20, 140, QSizePolicy.Policy.Minimum, QSizePolicy.Policy.Expanding)

        self.verticalLayout.addItem(self.verticalSpacer_3)


        self.horizontalLayout.addLayout(self.verticalLayout)


        self.retranslateUi(StudentWidget)

        QMetaObject.connectSlotsByName(StudentWidget)
    # setupUi

    def retranslateUi(self, StudentWidget):
        StudentWidget.setWindowTitle(QCoreApplication.translate("StudentWidget", u"Form", None))
        ___qtablewidgetitem = self.tableWidget.horizontalHeaderItem(0)
        ___qtablewidgetitem.setText(QCoreApplication.translate("StudentWidget", u"\u7f16\u53f7", None))
        ___qtablewidgetitem1 = self.tableWidget.horizontalHeaderItem(1)
        ___qtablewidgetitem1.setText(QCoreApplication.translate("StudentWidget", u"\u59d3\u540d", None))
        ___qtablewidgetitem2 = self.tableWidget.horizontalHeaderItem(2)
        ___qtablewidgetitem2.setText(QCoreApplication.translate("StudentWidget", u"\u6027\u522b", None))
        ___qtablewidgetitem3 = self.tableWidget.horizontalHeaderItem(3)
        ___qtablewidgetitem3.setText(QCoreApplication.translate("StudentWidget", u"\u751f\u65e5", None))
        ___qtablewidgetitem4 = self.tableWidget.horizontalHeaderItem(4)
        ___qtablewidgetitem4.setText(QCoreApplication.translate("StudentWidget", u"\u52a0\u5165\u65f6\u95f4", None))
        ___qtablewidgetitem5 = self.tableWidget.horizontalHeaderItem(5)
        ___qtablewidgetitem5.setText(QCoreApplication.translate("StudentWidget", u"\u5b66\u4e60\u76ee\u6807", None))
        ___qtablewidgetitem6 = self.tableWidget.horizontalHeaderItem(6)
        ___qtablewidgetitem6.setText(QCoreApplication.translate("StudentWidget", u"\u5f53\u524d\u8fdb\u5ea6", None))
        ___qtablewidgetitem7 = self.tableWidget.horizontalHeaderItem(7)
        ___qtablewidgetitem7.setText(QCoreApplication.translate("StudentWidget", u"\u7167\u7247", None))
        self.pushButton.setText(QCoreApplication.translate("StudentWidget", u"\u6dfb\u52a0", None))
        self.btnDeleteItem.setText(QCoreApplication.translate("StudentWidget", u"\u5220\u9664\u9879", None))
        self.btnDeleteList.setText(QCoreApplication.translate("StudentWidget", u"\u5220\u9664\u884c", None))
    # retranslateUi

