#include "singlelineinfo.h"

#include "ui_singlelineinfo.h"

SingleLineInfo::SingleLineInfo(const QString &txt, QWidget *parent) : QWidget(parent), ui(new Ui::SingleLineInfo) {
  ui->setupUi(this);
  ui->lineEdit->setText(txt);
  ui->tBtnDelete->setObjectName("delete");
}

SingleLineInfo::~SingleLineInfo() { delete ui; }

QString SingleLineInfo::getInfo() { return ui->lineEdit->text(); }

void SingleLineInfo::on_tBtnDelete_clicked() { emit requestDelete(this); }
