#include "editid.h"

#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_editid.h"

EditID::EditID(QWidget *parent) : QWidget(parent), ui(new Ui::EditID) {
  ui->setupUi(this);
  init();
}

EditID::~EditID() { delete ui; }

bool EditID::check() {
  updateID();
  QJsonObject objCheck;
  objCheck.insert(ID, m_id);
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_CHECK));
  QJsonObject res = MY_HTTP->post_sync(url, objCheck);
  if (res["status"].toBool()) {
    MY_GLOBAL->set<QString>(ID, m_id);
    return true;
  } else {
    MSG_WARN(tr("ID %1 already exists, please re-enter!").arg(m_id));
    return false;
  }
}

int EditID::getAutoIncreateID() {
  QJsonObject objGet;
  objGet.insert("type", ui->comboBoxSpectralType->currentText());
  QString url =
      QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_AUTO_INCREASE_ID));
  QJsonObject res = MY_HTTP->post_sync(url, objGet);
  if (res["status"].toBool()) {
    return res["increate_id"].toInt();
  } else {
    return 0;
  }
}

void EditID::init() {
  ui->comboBoxSpectralType->addItems({"RAM", "NIR", "MIR", "UV"});
  int id = getAutoIncreateID();
  if (id == 0) {
    SHOW_AUTO_CLOSE_MSGBOX(this, TITLE_WARNING,
                           tr("Failed to obtain the auto-increment ID, please enter it manually!"));
  } else {
    ui->spinBoxSpecificNumber->setValue(id);
  }
}

void EditID::updateID() {
  m_id = QString("%1_%2").arg(ui->comboBoxSpectralType->currentText(),
                              QString("%1").arg(ui->spinBoxSpecificNumber->value(), 4, 10, QChar('0')));
}
