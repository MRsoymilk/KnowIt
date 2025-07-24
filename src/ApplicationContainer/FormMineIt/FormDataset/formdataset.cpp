#include "formdataset.h"

#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_formdataset.h"

FormDataset::FormDataset(QWidget *parent) : QWidget(parent), ui(new Ui::FormDataset) {
  ui->setupUi(this);
  init();
}

FormDataset::~FormDataset() { delete ui; }

void FormDataset::retranslateUI() {
  ui->retranslateUi(this);
  ui->tBtnRefresh->setToolTip(tr("refresh"));
}

void FormDataset::init() {
  m_model = new QStandardItemModel(this);
  m_model->setHorizontalHeaderLabels(QStringList() << tr("ID"));
  ui->tableView->setModel(m_model);
  ui->tableView->horizontalHeader()->setStretchLastSection(true);
  ui->tableView->verticalHeader()->setVisible(false);
  ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  onUpdateData();

  ui->tBtnRefresh->setObjectName("refresh");
}

void FormDataset::on_tableView_clicked(const QModelIndex &index) {
  if (!index.isValid()) return;

  QModelIndex idIndex = index.sibling(index.row(), 0);
  QString id = idIndex.data().toString();

  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_GET));
  QJsonObject obj{{ID, id}};
  QJsonObject res = MY_HTTP->post_sync(url, obj);
  LOG_INFO("{}", res);
  QJsonObject objInfo = res["data"].toObject();
  QJsonObject objSpectralData = objInfo[SPECTRAL_DATA].toObject();
  QJsonObject objBasicInfo = objInfo[BASIC_INFORMATION].toObject();

  emit itInfo(objInfo);
  emit itPlot(objSpectralData);

  QString url_get_img_spectral = QString("%1%2/%3/%4")
                                     .arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_LOAD_IMG),
                                          objInfo[ID].toString(), objSpectralData[SPECTRAL_PICTURE].toString());
  MY_HTTP->getImage(
      url_get_img_spectral, [=](QPixmap pix) { emit itSpectral(pix); },
      [=](QString err) {
        qDebug() << "Download Error:" << err;
        QPixmap pix;
        emit itSpectral(pix);
      });

  QString url_get_img_structure = QString("%1%2/%3/%4")
                                      .arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_LOAD_IMG),
                                           objInfo[ID].toString(), objBasicInfo[STRUCTURE_PICTURE].toString());
  MY_HTTP->getImage(
      url_get_img_structure, [=](QPixmap pix) { emit itStructure(pix); },
      [=](QString err) {
        qDebug() << "Download Error:" << err;
        QPixmap pix;
        emit itStructure(pix);
      });
}

void FormDataset::clearData() { m_model->removeRows(0, m_model->rowCount()); }

void FormDataset::onUpdateData() {
  clearData();
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_GET));
  QJsonObject obj{{"dummy", true}};
  QJsonObject res = MY_HTTP->post_sync(url, obj);
  LOG_INFO("{}", res);

  QJsonArray array = res["data"].toArray();
  for (int i = 0; i < array.size(); ++i) {
    QString name = array[i].toString();
    QStandardItem *item = new QStandardItem(name);
    m_model->setItem(i, 0, item);
  }
}

void FormDataset::onSelectedDataDelete() {
  QModelIndex index = ui->tableView->currentIndex();
  if (!index.isValid()) {
    LOG_WARN("No item selected");
    return;
  }

  QModelIndex idIndex = index.sibling(index.row(), 0);
  QString id = idIndex.data().toString();

  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_DEL));

  QJsonObject obj;
  obj.insert(ID, id);

  QJsonObject res = MY_HTTP->post_sync(url, obj);
  LOG_INFO("Delete result: {}", res);
  onUpdateData();
}

void FormDataset::on_tBtnRefresh_clicked() { onUpdateData(); }
