#include "sampleinformation.h"

#include "g_define.h"
#include "ui_sampleinformation.h"

SampleInformation::SampleInformation(QWidget *parent) : QWidget(parent), ui(new Ui::SampleInformation) {
  ui->setupUi(this);
}

SampleInformation::~SampleInformation() { delete ui; }
void SampleInformation::setSampleInformation(const QJsonObject &data) {
  QJsonObject objSampleInformation = data[SAMPLE_INFOMATION].toObject();
  ui->lineEditOrigin->setText(objSampleInformation.value(PRODUCED_FROM).toString());
  ui->lineEditStorage->setText(objSampleInformation.value(STORAGE).toString());
  ui->lineEditDerivativeState->setText(objSampleInformation.value(DERIVATIVE_STATE).toString());
}

QJsonObject SampleInformation::getSampleInformation() {
  QJsonObject objSampleInformation;
  objSampleInformation.insert(PRODUCED_FROM, ui->lineEditOrigin->text());
  objSampleInformation.insert(STORAGE, ui->lineEditStorage->text());
  objSampleInformation.insert(DERIVATIVE_STATE, ui->lineEditDerivativeState->text());
  return objSampleInformation;
}
