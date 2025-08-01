#ifndef DIALOGADD_H
#define DIALOGADD_H

#include <QDialog>

class EditID;
class EditInfo;

namespace Ui {
class DialogAdd;
}

class DialogAdd : public QDialog {
  Q_OBJECT
 public:
  enum class EDIT { ID = 0, URL, INFO };

 public:
  explicit DialogAdd(QWidget *parent = nullptr);
  ~DialogAdd();

 protected:
  void closeEvent(QCloseEvent *event) override;
  void showEvent(QShowEvent *event) override;

 private:
  void init();

 private slots:
  void on_btnNext_clicked();
  void on_btnPrevious_clicked();

 private:
  Ui::DialogAdd *ui;
  EditID *m_editID;
  EditInfo *m_editInfo;
  QVector<QWidget *> m_editWidgets;
  bool m_committed;
};

#endif  // DIALOGADD_H
