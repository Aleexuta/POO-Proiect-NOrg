#pragma once
#ifndef LOGINFORM_H
#define LOGINFORM_H
//#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qdialog.h>
#include <regex>
#include <iostream>
#include "ui_LoginForm.h"

class LoginForm :
    public QDialog
{
    Q_OBJECT
private:
    LoginForm(QWidget* parent = 0);
    ~LoginForm();
    static LoginForm* instance;
private://private function 


public:
    static LoginForm* getInstance();
    void deleteInstance();
public slots:
    void on_LoginButton_clicked();
    void on_closeButton_clicked();
private:
    Ui::Dialog* ui;
};

#endif