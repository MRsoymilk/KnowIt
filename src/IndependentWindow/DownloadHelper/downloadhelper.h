#ifndef DOWNLOADHELPER_H
#define DOWNLOADHELPER_H

#include <QWidget>

namespace Ui {
class DownloadHelper;
}

class DownloadHelper : public QWidget {
  Q_OBJECT

 public:
  explicit DownloadHelper(QWidget *parent = nullptr);
  ~DownloadHelper();

 protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  bool eventFilter(QObject *watched, QEvent *event) override;

 private slots:
  void on_tBtnChooseDir_clicked();

 private:
  Ui::DownloadHelper *ui;

 private:
  void init();
  void display(const QPixmap &pix);
};

#endif  // DOWNLOADHELPER_H
