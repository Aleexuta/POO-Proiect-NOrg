#pragma once
#ifndef FIRSTFORM_H
#define FIRSTFORM_H
#include <QtWidgets/QMainWindow>
#include <qdialog.h>
#include "ui_FirstForm.h"
class FirstForm :
    public QDialog
{
    Q_OBJECT
public:
    FirstForm(QWidget* parent = 0);
    ~FirstForm();
 public slots:
    void on_pushButton_clicked();
private:
    Ui::FirstForm* ui;
};

#endif