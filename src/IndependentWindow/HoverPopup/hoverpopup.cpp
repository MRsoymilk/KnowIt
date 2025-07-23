#include "hoverpopup.h"

#include "g_define.h"
#include "ui_hoverpopup.h"

HoverPopup::HoverPopup(QWidget *parent) : QWidget(parent), ui(new Ui::HoverPopup) {
  ui->setupUi(this);
  init();
}

HoverPopup::~HoverPopup() { delete ui; }

void HoverPopup::setData(const QString &key, const QString &value) {
  ui->labelName->setText(key);

  QStringList items = value.split(DELIMITER, Qt::SkipEmptyParts);
  m_model->setStringList(items);
}

void HoverPopup::init() {
  m_model = new QStringListModel(this);
  ui->listViewValue->setModel(m_model);
}

void HoverPopup::leaveEvent(QEvent *event) { this->close(); }
