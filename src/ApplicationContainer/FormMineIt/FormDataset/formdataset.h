#ifndef FORMDATASET_H
#define FORMDATASET_H

#include <QJsonArray>
#include <QJsonValue>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QWidget>
namespace Ui {
class FormDataset;
}

class FormDataset : public QWidget {
  Q_OBJECT

 public:
  explicit FormDataset(QWidget *parent = nullptr);
  ~FormDataset();
  void retranslateUI();
 signals:
  void itInfo(const QJsonObject &data);
  void itPlot(const QJsonObject &data);
  void itStructure(const QPixmap &pix);
  void itSpectral(const QPixmap &pix);

 public slots:
  void onSelectedDataDelete();
  void onUpdateData();

 private:
  void init();
  void clearData();

 private slots:
  void on_tableView_clicked(const QModelIndex &index);

  void on_tBtnRefresh_clicked();

 private:
  Ui::FormDataset *ui;
  QStandardItemModel *m_model;
};

#endif  // FORMDATASET_H
