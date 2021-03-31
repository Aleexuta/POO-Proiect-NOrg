#pragma once
#ifndef REGISTERFORM_H
#define REGISTERFORM_H
#include <QtWidgets/QMainWindow>
#include <qdialog.h>
#include "ui_RegisterForm.h"
class RegisterForm :
    public QDialog
{
    Q_OBJECT
private:
    RegisterForm(QWidget* parent = 0);
    ~RegisterForm();



    static RegisterForm* instance;
public:
    static RegisterForm* getInstance();
    void deleteInstance();
public slots:
    void on_RegisterButton_clicked();
private:
    Ui::RegisterForm* ui;
};

#endif