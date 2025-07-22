#ifndef SPECTRALSETTING_H
#define SPECTRALSETTING_H

#include <QWidget>

namespace Ui {
class SpectralSetting;
}

class SpectralSetting : public QWidget {
  Q_OBJECT

 public:
  explicit SpectralSetting(QWidget *parent = nullptr);
  ~SpectralSetting();
  void init();

 private:
  Ui::SpectralSetting *ui;
};

#endif  // SPECTRALSETTING_H
