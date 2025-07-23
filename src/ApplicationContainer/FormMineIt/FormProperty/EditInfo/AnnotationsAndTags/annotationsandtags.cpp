#include "annotationsandtags.h"

#include "g_define.h"
#include "ui_annotationsandtags.h"

AnnotationsAndTags::AnnotationsAndTags(QWidget *parent) : QWidget(parent), ui(new Ui::AnnotationsAndTags) {
  ui->setupUi(this);
}

AnnotationsAndTags::~AnnotationsAndTags() { delete ui; }

void AnnotationsAndTags::setAnnotationsAndTags(const QJsonObject &data) {
  QJsonObject objAnnotationsAndTags = data[ANNOTATIONS_AND_TAGS].toObject();
  ui->lineEditDataVersion->setText(objAnnotationsAndTags.value(DATA_VERSION).toString());
  ui->lineEditLastModified->setText(objAnnotationsAndTags.value(LAST_MODIFIED).toString());
}
QJsonObject AnnotationsAndTags::getAnnotationsAndTags() {
  QJsonObject objAnnotationsAndTags;
  objAnnotationsAndTags.insert(DATA_VERSION, ui->lineEditDataVersion->text());
  objAnnotationsAndTags.insert(LAST_MODIFIED, ui->lineEditLastModified->text());
  return objAnnotationsAndTags;
}
