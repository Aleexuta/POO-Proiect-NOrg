#include "Mainwindow.h"

Mainwindow::Mainwindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void Mainwindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui.plainTextEdit->setPlainText(QString());
}

void Mainwindow::on_actionOpen_triggered()
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
    ui.plainTextEdit->setPlainText(text);
    file.close();

}

void Mainwindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }
    ui.plainTextEdit->print(&printer);
}

void Mainwindow::on_action_Exit_triggered()
{
    QApplication::quit();
}

void Mainwindow::on_actionSave_triggered()
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
    QString text = ui.plainTextEdit->toPlainText();
    out << text;
    file.close();
}

void Mainwindow::on_actionCopy_triggered()
{
    ui.plainTextEdit->copy();
}

void Mainwindow::on_actionCut_triggered()
{
    ui.plainTextEdit->cut();
}

void Mainwindow::on_actionPaste_triggered()
{
    ui.plainTextEdit->paste();
}

void Mainwindow::on_actionUndo_triggered()
{
    ui.plainTextEdit->undo();
}

void Mainwindow::on_actionRedo_triggered()
{
    ui.plainTextEdit->redo();
}

void Mainwindow::on_actionFont_triggered()
{
}


