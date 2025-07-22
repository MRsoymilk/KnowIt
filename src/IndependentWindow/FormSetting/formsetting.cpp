#include "formsetting.h"
#include "ui_formsetting.h"

#include "ServerSetting/serversetting.h"

FormSetting::FormSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormSetting)
{
    ui->setupUi(this);
    init();
}

FormSetting::~FormSetting()
{
    delete ui;
}

void FormSetting::init()
{
    m_serverSetting = new ServerSetting;
    ui->vLay->addWidget(m_serverSetting);
}

void FormSetting::on_buttonBox_accepted()
{
    this->close();
}

void FormSetting::on_buttonBox_rejected()
{
    this->close();
}
