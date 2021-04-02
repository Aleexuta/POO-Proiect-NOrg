#pragma once
#ifndef FIRSTFORM_H
#define FIRSTFORM_H
//#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qdialog.h>
#include "ui_FirstForm.h"
class FirstForm :
    public QDialog
{
    Q_OBJECT
public:
    static FirstForm* getInstance();
    void deleteInstance();
private:
    static FirstForm* instance;
    
    FirstForm(QWidget* parent = 0);
    ~FirstForm();
public slots:
    void on_pushButton_clicked();
    void on_closeButton_clicked();
private:
    Ui::FirstForm* ui;
};

#endif