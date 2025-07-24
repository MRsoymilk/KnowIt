#ifndef UPDATESETTING_H
#define UPDATESETTING_H

#include <QWidget>

namespace Ui {
class UpdateSetting;
}

class UpdateSetting : public QWidget {
  Q_OBJECT

 public:
  explicit UpdateSetting(QWidget *parent = nullptr);
  ~UpdateSetting();
  void init();

 private slots:
  void on_btnStartUpdate_clicked();

  void on_checkBoxAutoUpdate_clicked();

 private:
  Ui::UpdateSetting *ui;
};

#endif  // UPDATESETTING_H
