#include "sampleinformation.h"
#include "ui_sampleinformation.h"

SampleInformation::SampleInformation(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SampleInformation)
{
    ui->setupUi(this);
}

SampleInformation::~SampleInformation()
{
    delete ui;
}
