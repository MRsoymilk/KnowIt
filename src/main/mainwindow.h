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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_tBtnSetting_clicked();

private:
    void init();

private:
    Ui::MainWindow *ui;
    FormMineIt *m_AppMineIt;
    FormApplicationMenu *m_Menu;
    void initTheme();
    void setTheme(const QString &theme);
    void menuThemeSelect(QAction *selectedTheme);
    QString m_theme;
};
#endif // MAINWINDOW_H
