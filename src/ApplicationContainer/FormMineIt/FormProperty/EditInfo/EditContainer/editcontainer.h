#ifndef EDITCONTAINER_H
#define EDITCONTAINER_H

#include <QDialog>

namespace Ui {
class EditContainer;
}

class EditContainer : public QDialog {
  Q_OBJECT
 public:
  enum TYPE { PEAK, SINGLE_LINE };

 public:
  explicit EditContainer(TYPE type, QWidget *parent = nullptr);
  ~EditContainer();
  void setData(const QString &data);
  const QString getData();
 private slots:
  void on_tBtnAdd_clicked();
  void on_buttonBox_accepted();

 private:
  Ui::EditContainer *ui;
  TYPE m_type;
};

#endif  // EDITCONTAINER_H
