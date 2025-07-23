#ifndef HOVERPOPUP_H
#define HOVERPOPUP_H

#include <QStringListModel>
#include <QWidget>

namespace Ui {
class HoverPopup;
}

class HoverPopup : public QWidget {
  Q_OBJECT

 public:
  explicit HoverPopup(QWidget *parent = nullptr);
  ~HoverPopup();
  void setData(const QString &key, const QString &value);
  void init();

 protected:
  void leaveEvent(QEvent *event) override;

 private:
  Ui::HoverPopup *ui;
  QStringListModel *m_model;
};

#endif  // HOVERPOPUP_H
