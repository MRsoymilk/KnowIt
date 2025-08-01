#ifndef EDITID_H
#define EDITID_H

#include <QWidget>

namespace Ui {
class EditID;
}

class EditID : public QWidget {
  Q_OBJECT

 public:
  explicit EditID(QWidget *parent = nullptr);
  ~EditID();
  bool check();

 private:
  void init();
  void updateID();

  // private slots:
  //  void on_comboBoxMajor_currentTextChanged(const QString &major);

 private:
  Ui::EditID *ui;
  QString m_id;
  // QMap<QString, QStringList> m_mapMajorMinor;
  int getAutoIncreateID();
};

#endif  // EDITID_H
