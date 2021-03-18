#include "Mainwindow.h"

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
    
}
