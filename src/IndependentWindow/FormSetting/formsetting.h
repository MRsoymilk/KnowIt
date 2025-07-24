#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>

class ServerSetting;
class SpectralSetting;
class UpdateSetting;

namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget {
  Q_OBJECT

 public:
  explicit FormSetting(QWidget *parent = nullptr);
  ~FormSetting();

 private slots:
  void on_buttonBox_accepted();
  void on_buttonBox_rejected();

 private:
  void init();

 private:
  Ui::FormSetting *ui;
  ServerSetting *m_serverSetting;
  SpectralSetting *m_spectralSetting;
  UpdateSetting *m_updateSetting;
};

#endif  // FORMSETTING_H
