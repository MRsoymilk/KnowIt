#ifndef EDITURL_H
#define EDITURL_H

#include <QWidget>

namespace Ui {
class EditURL;
}

class EditURL : public QWidget {
  Q_OBJECT

 public:
  explicit EditURL(QWidget *parent = nullptr);
  ~EditURL();
  bool check();

  // signals:
  //     void backEditID();
  //     void editURLFinished();

  // private slots:
  //     void on_btnPrevious_clicked();
  //     void on_btnNext_clicked();

 private:
  Ui::EditURL *ui;
};

#endif  // EDITURL_H
