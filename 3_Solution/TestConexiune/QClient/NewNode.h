#pragma once
#ifndef NEWNODE_H
#define NEWNODE_H
//#include <QtWidgets/QMainWindow>
#include <qmainwindow.h>
#include <qdialog.h>
#include "ui_NewNode.h"
class NewNode :
    public QDialog
{
    Q_OBJECT
public:
    NewNode(int tip,QWidget* parent = 0);
    ~NewNode();
public slots:
    void on_OKButton_clicked();
    void on_colorSet_clicked();
private:
    QColor m_color;
    Ui::NewNode* ui;
    int m_tip;
};

#endif