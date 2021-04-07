#include "QClient.h"

#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <iostream>

QClient* QClient::instance = nullptr;

QClient::QClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_changed = false;

    // facere formular
}
QClient:: ~QClient()
{

}
void QClient::closeEvent(QCloseEvent* event)
{
    event->accept();
}
void QClient::checkSave()
{
    if (!m_changed)
        return;
    QMessageBox::StandardButton value = QMessageBox::question(this, "Save file?", "You have unsaved changes.Do you want to save now?");
    if (value == QMessageBox::StandardButton::No)
        return;
    else save();
}
void QClient::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save");
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(fileName);
    QTextStream out(&file);
    QString text = ui.textEdit->toPlainText();
    out << text;
    file.close();
    m_changed = false;
}
QClient* QClient::getInstance()
{
    if (!instance)
        instance = new QClient();
    return instance;
}

void QClient::deleteInstance()
{
    if (instance)
        delete instance;
}

void QClient::sendRegisterMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerRegister;
    char * vect=const_cast<char*>(j.c_str());
    for(int i=0;i<j.size();i++)
        msg << vect[i];

    Send(msg);
}

void QClient::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui.textEdit->setText(text);
    file.close();
    m_changed = false;
}

void QClient::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }
    ui.textEdit->print(&printer);
}

void QClient::on_actionExit_triggered()
{
    checkSave();
    QApplication::quit();
}

void QClient::on_actionSave_triggered()
{
    save();
}

void QClient::on_actionCopy_triggered()
{
    ui.textEdit->copy();
}

void QClient::on_textEdit_textChanged()
{
    m_changed = true;
}

void QClient::on_actionCut_triggered()
{
    ui.textEdit->cut();
}

void QClient::on_actionPaste_triggered()
{
    ui.textEdit->paste();
}

void QClient::on_actionUndo_triggered()
{
    ui.textEdit->undo();
}

void QClient::on_actionRedo_triggered()
{
    ui.textEdit->redo();
}

void QClient::on_actionZoom_in_triggered()
{
    ui.textEdit->zoomIn(1);
}

void QClient::on_actionZoom_out_triggered()
{
    ui.textEdit->zoomOut(1);
}

void QClient::on_actionNew_triggered()
{
    checkSave();
    currentFile.clear();
    ui.textEdit->setText(QString());
}