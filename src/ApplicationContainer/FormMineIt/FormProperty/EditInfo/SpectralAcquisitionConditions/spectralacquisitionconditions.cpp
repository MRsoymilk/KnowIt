#include "spectralacquisitionconditions.h"
#include "ui_spectralacquisitionconditions.h"

SpectralAcquisitionConditions::SpectralAcquisitionConditions(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpectralAcquisitionConditions)
{
    ui->setupUi(this);
}

SpectralAcquisitionConditions::~SpectralAcquisitionConditions()
{
    delete ui;
}
