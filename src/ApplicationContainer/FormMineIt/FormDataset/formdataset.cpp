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
  m_model->setHorizontalHeaderLabels(QStringList() << tr("ID") << tr("Compound Name(en)") << tr("Compound Name(zh)")
                                                   << tr("Structure Picture"));
  ui->tableView->setModel(m_model);
  QHeaderView *header = ui->tableView->horizontalHeader();
  header->setSectionResizeMode(QHeaderView::Interactive);

  ui->tableView->setColumnWidth(0, width() / 4);
  ui->tableView->setColumnWidth(1, width() / 3);
  ui->tableView->setColumnWidth(2, width() / 3);
  ui->tableView->setColumnWidth(3, width() / 3);

  ui->tableView->verticalHeader()->setVisible(false);
  ui->tableView->setIconSize(QSize(128, 128));
  ui->tableView->verticalHeader()->setDefaultSectionSize(96);
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
    QJsonObject obj = array.at(i).toObject();
    QString id = obj[ID].toString();
    QStandardItem *itemID = new QStandardItem(id);
    m_model->setItem(i, 0, itemID);

    QString name_en = obj[BASIC_INFORMATION].toObject()[COMPOUND_NAME_EN].toString();
    QStandardItem *itemName_en = new QStandardItem(name_en);
    m_model->setItem(i, 1, itemName_en);

    QString name_zh = obj[BASIC_INFORMATION].toObject()[COMPOUND_NAME_ZH].toString();
    QStandardItem *itemName_zh = new QStandardItem(name_zh);
    m_model->setItem(i, 2, itemName_zh);

    int row = i;
    QString url_get_img_structure =
        QString("%1%2/%3/%4")
            .arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_LOAD_IMG), obj[ID].toString(),
                 obj[BASIC_INFORMATION].toObject()[STRUCTURE_PICTURE].toString());

    MY_HTTP->getImage(
        url_get_img_structure,
        [=](QPixmap pix) {
          QStandardItem *imgItem = new QStandardItem();
          imgItem->setIcon(QIcon(pix.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
          imgItem->setTextAlignment(Qt::AlignCenter);
          m_model->setItem(row, 3, imgItem);
        },
        [=](QString err) { LOG_WARN("Failed to load structure image: {}", err); });
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
