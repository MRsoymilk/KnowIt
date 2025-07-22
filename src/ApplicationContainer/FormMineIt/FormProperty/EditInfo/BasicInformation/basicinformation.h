#ifndef BASICINFORMATION_H
#define BASICINFORMATION_H

#include <QWidget>

namespace Ui {
class BasicInformation;
}

class BasicInformation : public QWidget {
  Q_OBJECT

 public:
  explicit BasicInformation(QWidget *parent = nullptr);
  ~BasicInformation();
  void setID(const QString &id);
  void setUUID(const QString &uuid);
  void setBasicInformation(const QJsonObject &data);
  QJsonObject getBasicInformation();

 signals:
  void sendStructure(const QPixmap &pix);

 protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

 private:
  void init();
  bool handleGraphicsViewStructure(QEvent *event);

 private slots:
  void on_tBtnName_zh_Edit_clicked();
  void on_tBtnName_en_Edit_clicked();
  void on_tBtnUploadStructure_clicked();

 private:
  Ui::BasicInformation *ui;
  QString m_id, m_uuid;
};

#endif  // BASICINFORMATION_H
