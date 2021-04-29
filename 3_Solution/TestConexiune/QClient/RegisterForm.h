#pragma once
#ifndef REGISTERFORM_H
#define REGISTERFORM_H
//#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qdialog.h>
#include <regex>
#include <iostream>
#include "ui_RegisterForm.h"

class RegisterForm :
    public QDialog
{
    Q_OBJECT
private:
    RegisterForm(QWidget* parent = 0);
    ~RegisterForm();
    static RegisterForm* instance;
private://private function 
    

public:
    static RegisterForm* getInstance();
    void deleteInstance();
    void freeText();
public slots:
    void on_RegisterButton_clicked();
    void on_CloseButton_clicked();
private:
    Ui::RegisterForm* ui;
};

#endif