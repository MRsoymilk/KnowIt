#include "splashscreen.h"

#include <QGraphicsDropShadowEffect>
#include <QPixmap>

#include "ui_splashscreen.h"

SplashScreen::SplashScreen(QWidget *parent) : QWidget(parent), ui(new Ui::SplashScreen) {
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
  setAttribute(Qt::WA_TranslucentBackground);

  QPixmap logo(":/res/splash/knowit_logo.png");

  ui->labelLogo->setPixmap(logo.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

  auto *effect = new QGraphicsDropShadowEffect(this);
  effect->setOffset(0, 5);
  effect->setBlurRadius(20);
  effect->setColor(QColor(0, 0, 0, 160));
  this->setGraphicsEffect(effect);

  ui->labelStatus->setStyleSheet("QLabel { background-color: white; color: black; }");
  ui->labelStatus->setText("starting...");

  ui->labelSoftwareInfo->setStyleSheet("QLabel { background-color: white; color: black; }");

  ui->progressBar->setValue(0);
}

SplashScreen::~SplashScreen() { delete ui; }

void SplashScreen::setStatusText(const QString &text) {
  ui->labelStatus->setText(text);
  QApplication::processEvents();
}

void SplashScreen::setProgress(const double &progress) { ui->progressBar->setValue(progress * 100); }
