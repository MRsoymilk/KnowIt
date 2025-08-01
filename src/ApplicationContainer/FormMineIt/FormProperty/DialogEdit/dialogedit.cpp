#include "dialogedit.h"

#include "../EditInfo/editinfo.h"
#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_dialogedit.h"

DialogEdit::DialogEdit(const QJsonObject &data, QWidget *parent) : QDialog(parent), ui(new Ui::DialogEdit) {
  ui->setupUi(this);
  init(data);
}

DialogEdit::~DialogEdit() { delete ui; }

void DialogEdit::init(const QJsonObject &data) {
  setWindowState(Qt::WindowMaximized);
  m_editInfo = new EditInfo(data);
  ui->labelID->setText(data[ID].toString());
  ui->gLayEdit->addWidget(m_editInfo);
}

void DialogEdit::on_buttonBox_accepted() {
  QJsonObject obj = m_editInfo->getData();
  obj.insert(ID, ui->labelID->text());
  QString url = QString("%1/%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_SET));
  QJsonObject res = MY_HTTP->post_sync(url, obj);

  {
    QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_GET));
    QJsonObject obj{{ID, ui->labelID->text()}};
    QJsonObject res = MY_HTTP->post_sync(url, obj);
    emit updateEditInfo(res["data"].toObject());
  }
  this->close();
}

void DialogEdit::on_buttonBox_rejected() { this->close(); }
