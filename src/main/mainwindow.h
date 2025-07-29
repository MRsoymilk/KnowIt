#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FormMineIt;
class FormApplicationMenu;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_tBtnSetting_clicked();
  void menuLanguageSelect(QAction *selectedAction);
  void on_actionDocument_triggered();
  void on_actionAbout_triggered();
  void on_actionDownloadHelper_triggered();

 private:
  void init();
  void initTheme();
  void setTheme(const QString &theme);
  void menuThemeSelect(QAction *selectedTheme);
  void initMsgBar();
  void initLanguage();
  void setLanguage(const QString &language);

 private:
  Ui::MainWindow *ui;
  QString m_theme;
  FormMineIt *m_AppMineIt;
  FormApplicationMenu *m_Menu;
};
#endif  // MAINWINDOW_H
