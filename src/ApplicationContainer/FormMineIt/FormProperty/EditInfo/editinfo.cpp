#include "editinfo.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QMainWindow>
#include <QWheelEvent>

#include "AnnotationsAndTags/annotationsandtags.h"
#include "BasicInformation/basicinformation.h"
#include "Originate/originate.h"
#include "SampleInformation/sampleinformation.h"
#include "SpectralAcquisitionConditions/spectralacquisitionconditions.h"
#include "SpectralData/spectraldata.h"
#include "g_define.h"
#include "ui_editinfo.h"

EditInfo::EditInfo(const QJsonObject &data, QWidget *parent) : QWidget(parent), ui(new Ui::EditInfo) {
  ui->setupUi(this);
  init();
  if (!data.isEmpty()) {
    setData(data);
  }
}

EditInfo::~EditInfo() { delete ui; }

void EditInfo::init() {
  m_uuid = QUuid::createUuid().toString();
  m_AnnotationsAndTags = new AnnotationsAndTags;

  m_BasicInformation = new BasicInformation;
  m_BasicInformation->setUUID(m_uuid);

  m_Originate = new Originate;
  m_SampleInformation = new SampleInformation;

  m_SpectralData = new SpectralData;
  m_SpectralData->setUUID(m_uuid);

  m_SpectralAcquisitionConditions = new SpectralAcquisitionConditions;

  ui->gLayAnnotationsAndTags->addWidget(m_AnnotationsAndTags);
  ui->gLayBasicInformation->addWidget(m_BasicInformation);
  ui->gLayOriginate->addWidget(m_Originate);
  ui->gLaySampleInformation->addWidget(m_SampleInformation);
  ui->gLaySpectralData->addWidget(m_SpectralData);
  ui->gLaySpectralAcquisitionConditions->addWidget(m_SpectralAcquisitionConditions);

  connect(m_SpectralData, &SpectralData::sendSpectral, this, &EditInfo::onSpectral);
  connect(m_BasicInformation, &BasicInformation::sendStructure, this, &EditInfo::onStructure);
}

void EditInfo::setData(const QJsonObject &data) {
  m_id = data[ID].toString();
  m_uuid = QUuid::createUuid().toString();
  m_BasicInformation->setID(m_id);
  m_BasicInformation->setUUID(m_uuid);
  m_BasicInformation->setBasicInformation(data);
  m_SpectralData->setID(m_id);
  m_SpectralData->setUUID(m_uuid);
  m_SpectralData->setSpectralData(data);
  m_SpectralAcquisitionConditions->setSpectralAcquisitionConditions(data);
  m_SampleInformation->setSampleInformation(data);
  m_AnnotationsAndTags->setAnnotationsAndTags(data);
  m_Originate->setOriginate(data);
}

QJsonObject EditInfo::getData() {
  QJsonObject obj;
  obj.insert("UUID", m_uuid);
  obj.insert(BASIC_INFORMATION, m_BasicInformation->getBasicInformation());
  obj.insert(SPECTRAL_DATA, m_SpectralData->getSpectralData());
  obj.insert(SPECTRAL_ACQUISITION_CONDITIONS, m_SpectralAcquisitionConditions->getSpectralAcquisitionConditions());
  obj.insert(SAMPLE_INFOMATION, m_SampleInformation->getSampleInformation());
  obj.insert(ANNOTATIONS_AND_TAGS, m_AnnotationsAndTags->getAnnotationsAndTags());
  obj.insert(ORIGINATE, m_Originate->getOriginate());
  return obj;
}

void EditInfo::onStructure(const QPixmap &pix) { emit sendStructure(pix); }

void EditInfo::onSpectral(const QPixmap &pix) { emit sendSpectral(pix); }
