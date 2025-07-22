#ifndef SERVERSETTING_H
#define SERVERSETTING_H

#include <QWidget>

namespace Ui {
class ServerSetting;
}

class ServerSetting : public QWidget {
  Q_OBJECT

 public:
  explicit ServerSetting(QWidget *parent = nullptr);
  ~ServerSetting();

 private:
  Ui::ServerSetting *ui;
  void init();
};

#endif  // SERVERSETTING_H
