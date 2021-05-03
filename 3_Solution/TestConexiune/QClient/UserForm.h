#pragma once
#ifndef USERFORM_H
#define USERFORM_H
//#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qdialog.h>
#include "ui_UserForm.h"
class UserForm :
    public QDialog
{
    Q_OBJECT
private:
    static UserForm* instance;
    UserForm(QWidget* parent = 0);
    ~UserForm();
public:
    static UserForm* getInstance();
    void deleteInstance();
    void changePassword(std::string oldpass, std::string newpass);
public slots:
    void on_logoutButton_clicked();
    void on_deleteAccButton_clicked();
    void on_changePassButton_clicked();
private:

    Ui::UserForm* ui;
};
#endif