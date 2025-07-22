#ifndef SPECTRALACQUISITIONCONDITIONS_H
#define SPECTRALACQUISITIONCONDITIONS_H

#include <QWidget>

namespace Ui {
class SpectralAcquisitionConditions;
}

class SpectralAcquisitionConditions : public QWidget {
  Q_OBJECT

 public:
  explicit SpectralAcquisitionConditions(QWidget *parent = nullptr);
  ~SpectralAcquisitionConditions();

  void setSpectralAcquisitionConditions(const QJsonObject &data);

  QJsonObject getSpectralAcquisitionConditions();

 private:
  Ui::SpectralAcquisitionConditions *ui;
};

#endif  // SPECTRALACQUISITIONCONDITIONS_H
