#include "peakinfo.h"
#include "ui_peakinfo.h"

PeakInfo::PeakInfo(QWidget *parent, const double &wavelength, const double &intensity)
    : QWidget(parent)
    , ui(new Ui::PeakInfo)
{
    ui->setupUi(this);
    ui->doubleSpinBoxWavelength->setValue(wavelength);
    ui->doubleSpinBoxIntensity->setValue(intensity);
    ui->tBtnDelete->setObjectName("delete");
}

PeakInfo::~PeakInfo()
{
    delete ui;
}

double PeakInfo::getWavelength()
{
    return ui->doubleSpinBoxWavelength->value();
}

double PeakInfo::getIntensity()
{
    return ui->doubleSpinBoxIntensity->value();
}

void PeakInfo::on_tBtnDelete_clicked()
{
    this->close();
}
