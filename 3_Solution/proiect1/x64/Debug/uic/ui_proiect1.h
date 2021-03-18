/********************************************************************************
** Form generated from reading UI file 'proiect1.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROIECT1_H
#define UI_PROIECT1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_proiect1Class
{
public:
    QAction *actionExit;
    QAction *actionSave;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    QTreeView *treeView;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *proiect1Class)
    {
        if (proiect1Class->objectName().isEmpty())
            proiect1Class->setObjectName(QString::fromUtf8("proiect1Class"));
        proiect1Class->resize(753, 506);
        actionExit = new QAction(proiect1Class);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionSave = new QAction(proiect1Class);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        centralWidget = new QWidget(proiect1Class);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        treeView = new QTreeView(splitter);
        treeView->setObjectName(QString::fromUtf8("treeView"));
        treeView->setMinimumSize(QSize(50, 0));
        treeView->setMaximumSize(QSize(250, 16777215));
        splitter->addWidget(treeView);
        plainTextEdit = new QPlainTextEdit(splitter);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        splitter->addWidget(plainTextEdit);

        horizontalLayout->addWidget(splitter);

        proiect1Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(proiect1Class);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 753, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        proiect1Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(proiect1Class);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        proiect1Class->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(proiect1Class);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        proiect1Class->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);

        retranslateUi(proiect1Class);

        QMetaObject::connectSlotsByName(proiect1Class);
    } // setupUi

    void retranslateUi(QMainWindow *proiect1Class)
    {
        proiect1Class->setWindowTitle(QCoreApplication::translate("proiect1Class", "proiect1", nullptr));
        actionExit->setText(QCoreApplication::translate("proiect1Class", "Exit", nullptr));
        actionSave->setText(QCoreApplication::translate("proiect1Class", "Save", nullptr));
        menuFile->setTitle(QCoreApplication::translate("proiect1Class", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class proiect1Class: public Ui_proiect1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROIECT1_H
