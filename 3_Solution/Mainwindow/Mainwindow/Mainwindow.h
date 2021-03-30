#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <qmessagebox.h>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QtPrintSupport/qprinterinfo.h>

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::MainwindowClass ui;
    QString currentFile = "";
private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionFont_triggered();
};
