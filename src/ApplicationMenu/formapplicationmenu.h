#ifndef FORMAPPLICATIONMENU_H
#define FORMAPPLICATIONMENU_H

#include <QWidget>

namespace Ui {
class FormApplicationMenu;
}

class FormApplicationMenu : public QWidget {
  Q_OBJECT

 public:
  explicit FormApplicationMenu(QWidget *parent = nullptr);
  ~FormApplicationMenu();

 private:
  void init();

 private slots:
  void on_tBtnMineIt_clicked();

 private:
  Ui::FormApplicationMenu *ui;
};

#endif  // FORMAPPLICATIONMENU_H
