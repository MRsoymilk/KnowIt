#ifndef AUTOUPDATE_H
#define AUTOUPDATE_H

#include <QJsonObject>
#include <QWidget>

namespace Ui {
class AutoUpdate;
}

class AutoUpdate : public QWidget {
  Q_OBJECT

 public:
  explicit AutoUpdate(QWidget *parent = nullptr);
  ~AutoUpdate();

 public slots:
  bool isNewVersion();

 protected:
  void showEvent(QShowEvent *event) override;

 private slots:
  void on_btnUpdate_clicked();
  void on_radioButtonExeOnly_clicked();
  void on_radioButtonFull_clicked();

 private:
  QJsonObject checkUpdate();

 private:
  Ui::AutoUpdate *ui;
  QString m_url;
  QJsonObject m_objUpdate;
};

#endif  // AUTOUPDATE_H
