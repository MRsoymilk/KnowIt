#include "spectralacquisitionconditions.h"
#include "g_define.h"
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

void SpectralAcquisitionConditions::setSpectralAcquisitionConditions(const QJsonObject &data)
{
    QJsonObject objSpectralAcquisitionConditions = data[SPECTRAL_ACQUISITION_CONDITIONS].toObject();
    ui->lineEditInstrumentModel->setText(
        objSpectralAcquisitionConditions.value(INSTRUMENT_MODEL).toString());
    ui->lineEditLaserWavelength->setText(
        objSpectralAcquisitionConditions.value(LASER_WAVELENGTH).toString());
    ui->lineEditLaserPower->setText(objSpectralAcquisitionConditions.value(LASER_POWER).toString());
    ui->lineEditCalculusTimes->setText(
        objSpectralAcquisitionConditions.value(CALCULUS_TIMES).toString());
    ui->lineEditMeasuringEnvironment->setText(
        objSpectralAcquisitionConditions.value(MEASURING_ENVIRONMENT).toString());
    ui->lineEditMeasuringTemperature->setText(
        objSpectralAcquisitionConditions.value(MEASURING_TEMPERATURE).toString());
}

QJsonObject SpectralAcquisitionConditions::getSpectralAcquisitionConditions()
{
    QJsonObject objSpectralAcquisitionConditions;
    objSpectralAcquisitionConditions.insert(INSTRUMENT_MODEL, ui->lineEditInstrumentModel->text());
    objSpectralAcquisitionConditions.insert(LASER_WAVELENGTH, ui->lineEditLaserWavelength->text());
    objSpectralAcquisitionConditions.insert(LASER_POWER, ui->lineEditLaserPower->text());
    objSpectralAcquisitionConditions.insert(CALCULUS_TIMES, ui->lineEditCalculusTimes->text());
    objSpectralAcquisitionConditions.insert(MEASURING_ENVIRONMENT,
                                            ui->lineEditMeasuringEnvironment->text());
    objSpectralAcquisitionConditions.insert(MEASURING_TEMPERATURE,
                                            ui->lineEditMeasuringTemperature->text());
    return objSpectralAcquisitionConditions;
}
