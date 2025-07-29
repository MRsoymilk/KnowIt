#include "mainwindow.h"

#include <QDesktopServices>
#include <QFile>
#include <QLabel>
#include <QTranslator>

#include "./ui_mainwindow.h"
#include "About/about.h"
#include "FormSetting/formsetting.h"
#include "formapplicationmenu.h"
#include "formmineit.h"
#include "g_define.h"
#include "version.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  init();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::init() {
  m_AppMineIt = new FormMineIt;
  ui->gLayApplicationContainer->addWidget(m_AppMineIt);
  m_Menu = new FormApplicationMenu;
  ui->gLayApplicationMenu->addWidget(m_Menu);

  ui->tBtnSetting->setToolTip(tr("settings"));
  ui->tBtnSetting->setObjectName("settings");
  ui->tBtnSetting->setIconSize(QSize(24, 24));
  ui->tBtnSetting->setCheckable(true);
  ui->tBtnSetting->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

  initLanguage();
  initTheme();
  initMsgBar();
}

void MainWindow::initLanguage() {
  QString language = SETTING_CONFIG_GET(CFG_GROUP_PROGRAM, CFG_PROGRAM_LANGUAGE, "en");
  connect(ui->menuLanguage, &QMenu::triggered, this, &MainWindow::menuLanguageSelect);
  const QList<QAction *> actions = ui->menuLanguage->actions();
  for (QAction *act : actions) {
    act->setCheckable(true);
    if (act->text() == language) {
      menuLanguageSelect(act);
    }
  }
}

void MainWindow::menuLanguageSelect(QAction *selectedAction) {
  QString language = selectedAction->text();
  setLanguage(language);
  const QList<QAction *> actions = ui->menuLanguage->actions();
  for (QAction *act : actions) {
    if (act == selectedAction) {
      act->setChecked(true);
    } else {
      act->setChecked(false);
    }
  }
}

void MainWindow::on_actionDocument_triggered() {
  QString pdfPath = QCoreApplication::applicationDirPath() + "/document/KnowIt.pdf";

  if (!QFile::exists(pdfPath)) {
    MSG_WARN(QString("Unable to find help document: %1").arg(pdfPath));
    return;
  }

  QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath));
}

void MainWindow::on_actionAbout_triggered() {
  About *about = new About;
  about->exec();
}

void MainWindow::setLanguage(const QString &language) {
  QTranslator *translator = new QTranslator(this);
  if (translator->load(QString(":/res/i18n/%1.qm").arg(language))) {
    qApp->installTranslator(translator);
    ui->retranslateUi(this);
    if (m_AppMineIt) {
      m_AppMineIt->retranslateUI();
    }
    if (m_Menu) {
      m_Menu->retranslateUI();
    }
  }
  SETTING_CONFIG_SET(CFG_GROUP_PROGRAM, CFG_PROGRAM_LANGUAGE, language);
}

void MainWindow::initMsgBar() {
  QLabel *linkLabel = new QLabel(this);
  linkLabel->setText(QString("version: %1").arg(APP_VERSION));
  linkLabel->setTextFormat(Qt::RichText);
  linkLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  linkLabel->setOpenExternalLinks(true);
  ui->statusbar->addPermanentWidget(linkLabel);
}

void MainWindow::initTheme() {
  QString theme = SETTING_CONFIG_GET(CFG_GROUP_PROGRAM, CFG_PROGRAM_THEME, "Lite");
  connect(ui->menuTheme, &QMenu::triggered, this, &MainWindow::menuThemeSelect);
  const QList<QAction *> actions = ui->menuTheme->actions();
  for (QAction *act : actions) {
    act->setCheckable(true);
    if (act->text() == theme) {
      menuThemeSelect(act);
    }
  }
}

void MainWindow::menuThemeSelect(QAction *selectedTheme) {
  QString theme = selectedTheme->text();
  setTheme(theme);
  m_theme = theme;
  const QList<QAction *> actions = ui->menuTheme->actions();
  for (QAction *act : actions) {
    if (act == selectedTheme) {
      act->setChecked(true);
    } else {
      act->setChecked(false);
    }
  }
}

void MainWindow::setTheme(const QString &theme) {
  QFile file(QString(":/res/themes/%1.qss").arg(theme));
  if (file.open(QFile::ReadOnly | QFile::Text)) {
    QString style = file.readAll();
    qApp->setStyleSheet(style);
  }

  SETTING_CONFIG_SET(CFG_GROUP_PROGRAM, CFG_PROGRAM_THEME, theme);
}

void MainWindow::on_tBtnSetting_clicked() {
  FormSetting *setting = new FormSetting;
  setting->show();
}
