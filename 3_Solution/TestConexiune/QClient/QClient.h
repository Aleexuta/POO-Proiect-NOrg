#pragma once

#include <QtWidgets/QMainWindow>
#include <nlohmann/json.hpp>
#include "ui_QClient.h"
#include <olc_net.h>
#include "IUser.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <qmessagebox.h>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QtPrintSupport/qprinterinfo.h>
#include <QCloseEvent>
#include "Find_Form.h"
#include "Replace_Form.h"
#include <QColorDialog>
#include <QFontDialog>
#include <QCloseEvent>
enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerAcceptRegister,
    ServerDenyRegister,
    ServerAcceptLogin,
    ServerDenyLogin,
    ServerDeny,
	ServerRegister,
    ServerLogin,
    CreateNode,
    OpenNode,
    SaveNode,
    LoadNodes
};

class QClient : public QMainWindow, public olc::net::client_interface<CustomMsgTypes>
{
    Q_OBJECT

private:
    static QClient* instance;
     QClient(QWidget *parent = Q_NULLPTR);
    ~QClient();
    void closeEvent(QCloseEvent *event)override;
    void checkSave();
    void save();
    void newFile();
    void openFile();
    
private:
    Ui::QClientClass ui;
    IUser* user;
    QString m_path;
    bool m_changed;

public:
    static QClient* getInstance();
    void deleteInstance();
    void sendRegisterMessage(std::string j);

private slots:
    
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionSave_triggered();
    void on_actionCopy_triggered();
    void on_textEdit_textChanged();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionZoom_in_triggered();
    void on_actionZoom_out_triggered();
    void on_actionSelect_All_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionBold_triggered();
    void on_actionItalic_triggered();
    void on_actionStrikeout_triggered();
    void on_actionUnderline_triggered();
    void on_actionColor_triggered();
    void on_actionFont_triggered();
    
};
