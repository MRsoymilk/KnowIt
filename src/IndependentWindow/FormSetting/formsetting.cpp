#include "formsetting.h"

#include "ServerSetting/serversetting.h"
#include "SpectralSetting/spectralsetting.h"
#include "UpdateSetting/updatesetting.h"
#include "ui_formsetting.h"

FormSetting::FormSetting(QWidget *parent) : QWidget(parent), ui(new Ui::FormSetting) {
  ui->setupUi(this);
  init();
}

FormSetting::~FormSetting() { delete ui; }

void FormSetting::init() {
  m_serverSetting = new ServerSetting;
  ui->vLay->addWidget(m_serverSetting);
  m_spectralSetting = new SpectralSetting;
  ui->vLay->addWidget(m_spectralSetting);
  m_updateSetting = new UpdateSetting;
  ui->vLay->addWidget(m_updateSetting);
}

void FormSetting::on_buttonBox_accepted() { this->close(); }

void FormSetting::on_buttonBox_rejected() { this->close(); }
