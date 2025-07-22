#include "basicinformation.h"
#include "ui_basicinformation.h"

BasicInformation::BasicInformation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BasicInformation)
{
    ui->setupUi(this);
}

BasicInformation::~BasicInformation()
{
    delete ui;
}
