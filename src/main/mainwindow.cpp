#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "FormSetting/formsetting.h"
#include "formapplicationmenu.h"
#include "formmineit.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{

    m_AppMineIt = new FormMineIt;
    ui->gLayApplicationContainer->addWidget(m_AppMineIt);
    m_Menu = new FormApplicationMenu;
    ui->gLayApplicationMenu->addWidget(m_Menu);

    ui->tBtnSetting->setToolTip(tr("settings"));
    ui->tBtnSetting->setObjectName("settings");
    ui->tBtnSetting->setIcon(QIcon(":/res/icons/black/settings.png"));
    ui->tBtnSetting->setIconSize(QSize(24, 24));
    ui->tBtnSetting->setCheckable(true);
    ui->tBtnSetting->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}


void MainWindow::on_tBtnSetting_clicked()
{
    FormSetting *setting = new FormSetting;
    setting->show();
}
