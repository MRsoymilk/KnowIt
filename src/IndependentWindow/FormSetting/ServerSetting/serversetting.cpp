#include "serversetting.h"
#include "ui_serversetting.h"

ServerSetting::ServerSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerSetting)
{
    ui->setupUi(this);
}

ServerSetting::~ServerSetting()
{
    delete ui;
}
