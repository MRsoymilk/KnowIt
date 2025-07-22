#include "originate.h"

#include "../EditContainer/editcontainer.h"
#include "g_define.h"
#include "ui_originate.h"

Originate::Originate(QWidget *parent) : QWidget(parent), ui(new Ui::Originate) {
  ui->setupUi(this);
  init();
}

Originate::~Originate() { delete ui; }

void Originate::init() { ui->tBtnSourceEdit->setObjectName("edit"); }

void Originate::setOriginate(const QJsonObject &data) {
  QJsonObject objOriginate = data[ORIGINATE].toObject();
  ui->lineEditSource->setText(objOriginate.value(SOURCE).toString());
}

QJsonObject Originate::getOriginate() {
  QJsonObject objOriginate;
  objOriginate.insert(SOURCE, ui->lineEditSource->text());
  return objOriginate;
}

void Originate::on_tBtnSourceEdit_clicked() {
  EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
  edit.setData(ui->lineEditSource->text());
  edit.exec();
  ui->lineEditSource->setText(edit.getData());
}
