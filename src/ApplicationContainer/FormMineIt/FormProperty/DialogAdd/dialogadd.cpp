#include "dialogadd.h"

#include <QScreen>

#include "../EditID/editid.h"
#include "../EditInfo/editinfo.h"
#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_dialogadd.h"

DialogAdd::DialogAdd(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAdd) {
  ui->setupUi(this);
  init();
}

DialogAdd::~DialogAdd() { delete ui; }

void DialogAdd::closeEvent(QCloseEvent *event) {
  if (!m_committed && !ui->labelID->text().isEmpty()) {
    QString id = ui->labelID->text();
    QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_DEL));
    QJsonObject obj;
    obj.insert(ID, id);
    QJsonObject res = MY_HTTP->post_sync(url, obj);
    LOG_INFO("Delete result: {}", res);
  }
}

void DialogAdd::showEvent(QShowEvent *event) { m_committed = false; }

void DialogAdd::init() {
  setWindowState(Qt::WindowMaximized);

  m_editID = new EditID;
  m_editInfo = new EditInfo;

  ui->stackedWidget->addWidget(m_editID);
  ui->stackedWidget->addWidget(m_editInfo);

  m_editWidgets = {m_editID, m_editInfo};

  ui->stackedWidget->setCurrentWidget(m_editID);

  ui->labelStatus->setText("1/" + QString::number(m_editWidgets.size()));
}

void DialogAdd::on_btnNext_clicked() {
  int index = ui->stackedWidget->currentIndex();

  if (auto edit = qobject_cast<EditID *>(m_editWidgets[index])) {
    if (edit->check()) {
      ui->labelID->setText(MY_GLOBAL->get<QString>(ID));
    } else {
      return;
    }
  }
  else if (auto edit = qobject_cast<EditInfo *>(m_editWidgets[index])) {
    QJsonObject obj = edit->getData();
    obj.insert(ID, ui->labelID->text());
    QString url = QString("%1/%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_DATASET_SET));
    QJsonObject res = MY_HTTP->post_sync(url, obj);
    m_committed = true;
    this->close();
    return;
  }

  if (index < m_editWidgets.size() - 1) {
    index++;
    ui->stackedWidget->setCurrentIndex(index);
    ui->labelStatus->setText(QString("%1/" + QString::number(m_editWidgets.size())).arg(index + 1));
  }
}

void DialogAdd::on_btnPrevious_clicked() {
  int index = ui->stackedWidget->currentIndex();
  if (index > 0) {
    index--;
    ui->stackedWidget->setCurrentIndex(index);
    ui->labelStatus->setText(QString("%1/" + QString::number(m_editWidgets.size())).arg(index + 1));
  }
}
