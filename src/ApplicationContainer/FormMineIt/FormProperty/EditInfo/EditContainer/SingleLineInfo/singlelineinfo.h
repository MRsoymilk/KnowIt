#ifndef SINGLELINEINFO_H
#define SINGLELINEINFO_H

#include <QWidget>

namespace Ui {
class SingleLineInfo;
}

class SingleLineInfo : public QWidget {
  Q_OBJECT

 public:
  explicit SingleLineInfo(const QString &txt = "", QWidget *parent = nullptr);
  ~SingleLineInfo();
  QString getInfo();
 private slots:
  void on_tBtnDelete_clicked();

 private:
  Ui::SingleLineInfo *ui;
};

#endif  // SINGLELINEINFO_H
