#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>

namespace Ui {
class SplashScreen;
}

class SplashScreen : public QWidget {
  Q_OBJECT

 public:
  explicit SplashScreen(QWidget *parent = nullptr);
  ~SplashScreen();
  void setStatusText(const QString &text);

 private:
  Ui::SplashScreen *ui;
};

#endif  // SPLASHSCREEN_H
