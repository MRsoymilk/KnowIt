#include "spectraldata.h"
#include "ui_spectraldata.h"

SpectralData::SpectralData(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpectralData)
{
    ui->setupUi(this);
}

SpectralData::~SpectralData()
{
    delete ui;
}
