/********************************************************************************
** Form generated from reading UI file 'my_tool.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MY_TOOL_H
#define UI_MY_TOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_my_toolClass
{
public:
    QWidget *centralWidget;
    QPushButton *sniffStartButton;
    QPushButton *OpenCameraButtom;
    QComboBox *wangkaBox;
    QPushButton *cutOutPicButton;
    QTableWidget *tableWidget;
    QPushButton *OpenWangKaButton;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *my_toolClass)
    {
        if (my_toolClass->objectName().isEmpty())
            my_toolClass->setObjectName(QStringLiteral("my_toolClass"));
        my_toolClass->resize(1226, 738);
        my_toolClass->setMinimumSize(QSize(200, 200));
        my_toolClass->setMaximumSize(QSize(16777215, 16777215));
        centralWidget = new QWidget(my_toolClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sniffStartButton = new QPushButton(centralWidget);
        sniffStartButton->setObjectName(QStringLiteral("sniffStartButton"));
        sniffStartButton->setGeometry(QRect(510, 10, 93, 28));
        OpenCameraButtom = new QPushButton(centralWidget);
        OpenCameraButtom->setObjectName(QStringLiteral("OpenCameraButtom"));
        OpenCameraButtom->setGeometry(QRect(770, 10, 93, 28));
        wangkaBox = new QComboBox(centralWidget);
        wangkaBox->setObjectName(QStringLiteral("wangkaBox"));
        wangkaBox->setGeometry(QRect(20, 10, 341, 22));
        cutOutPicButton = new QPushButton(centralWidget);
        cutOutPicButton->setObjectName(QStringLiteral("cutOutPicButton"));
        cutOutPicButton->setGeometry(QRect(650, 10, 93, 28));
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(20, 60, 411, 611));
        tableWidget->horizontalHeader()->setDefaultSectionSize(195);
        OpenWangKaButton = new QPushButton(centralWidget);
        OpenWangKaButton->setObjectName(QStringLiteral("OpenWangKaButton"));
        OpenWangKaButton->setGeometry(QRect(390, 10, 75, 23));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(460, 60, 721, 611));
        my_toolClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(my_toolClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1226, 23));
        my_toolClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(my_toolClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        my_toolClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(my_toolClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        my_toolClass->setStatusBar(statusBar);

        retranslateUi(my_toolClass);

        QMetaObject::connectSlotsByName(my_toolClass);
    } // setupUi

    void retranslateUi(QMainWindow *my_toolClass)
    {
        my_toolClass->setWindowTitle(QApplication::translate("my_toolClass", "my_tool", 0));
        sniffStartButton->setText(QApplication::translate("my_toolClass", "arp\346\234\254\347\275\221\346\256\265\345\271\277\346\222\255", 0));
        OpenCameraButtom->setText(QApplication::translate("my_toolClass", "\346\211\223\345\274\200\346\221\204\345\203\217\345\244\264", 0));
        cutOutPicButton->setText(QApplication::translate("my_toolClass", "\346\210\252\345\217\226\345\233\276\347\211\207", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("my_toolClass", "IP", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("my_toolClass", "MAC", 0));
        OpenWangKaButton->setText(QApplication::translate("my_toolClass", "\346\211\223\345\274\200\347\275\221\345\215\241", 0));
    } // retranslateUi

};

namespace Ui {
    class my_toolClass: public Ui_my_toolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MY_TOOL_H
