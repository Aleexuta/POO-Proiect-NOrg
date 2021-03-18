#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_proiect1.h"

class Mainwindow : public QMainWindow
{
    Q_OBJECT

public:
    Mainwindow(QWidget *parent = Q_NULLPTR);

private:
    Ui::proiect1Class ui;
};
