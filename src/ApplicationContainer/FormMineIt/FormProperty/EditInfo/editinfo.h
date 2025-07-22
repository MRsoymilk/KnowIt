#ifndef EDITINFO_H
#define EDITINFO_H

#include <QButtonGroup>
#include <QGraphicsSimpleTextItem>
#include <QJsonObject>
#include <QLabel>
#include <QWidget>

class AnnotationsAndTags;
class BasicInformation;
class Originate;
class SampleInformation;
class SpectralData;
class SpectralAcquisitionConditions;

namespace Ui {
class EditInfo;
}

class EditInfo : public QWidget {
  Q_OBJECT
 public:
 public:
  explicit EditInfo(const QJsonObject &data = {}, QWidget *parent = nullptr);
  ~EditInfo();
  void setData(const QJsonObject &data);
  QJsonObject getData();

 signals:
  void sendStructure(const QPixmap &pix);
  void sendSpectral(const QPixmap &pix);
 public slots:
  void onStructure(const QPixmap &pix);
  void onSpectral(const QPixmap &pix);

 private:
  void init();

 private:
  Ui::EditInfo *ui;

  QString m_uuid;
  QString m_id;
  AnnotationsAndTags *m_AnnotationsAndTags;
  BasicInformation *m_BasicInformation;
  Originate *m_Originate;
  SampleInformation *m_SampleInformation;
  SpectralData *m_SpectralData;
  SpectralAcquisitionConditions *m_SpectralAcquisitionConditions;
};

#endif  // EDITINFO_H
