#include "annotationsandtags.h"
#include "g_define.h"
#include "ui_annotationsandtags.h"

AnnotationsAndTags::AnnotationsAndTags(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AnnotationsAndTags)
{
    ui->setupUi(this);
}

AnnotationsAndTags::~AnnotationsAndTags()
{
    delete ui;
}

void AnnotationsAndTags::setAnnotationsAndTags(const QJsonObject &data)
{
    QJsonObject objAnnotationsAndTags = data[ANNOTATIONS_AND_TAGS].toObject();
    ui->lineEditCategory->setText(objAnnotationsAndTags.value(CATEGORY).toString());
    ui->lineEditApplicationArea->setText(objAnnotationsAndTags.value(APPLICATION_AREA).toString());
    ui->lineEditDataVersion->setText(objAnnotationsAndTags.value(DATA_VERSION).toString());
    ui->lineEditLastModified->setText(objAnnotationsAndTags.value(LAST_MODIFIED).toString());
}
QJsonObject AnnotationsAndTags::getAnnotationsAndTags()
{
    QJsonObject objAnnotationsAndTags;
    objAnnotationsAndTags.insert(CATEGORY, ui->lineEditCategory->text());
    objAnnotationsAndTags.insert(APPLICATION_AREA, ui->lineEditApplicationArea->text());
    objAnnotationsAndTags.insert(DATA_VERSION, ui->lineEditDataVersion->text());
    objAnnotationsAndTags.insert(LAST_MODIFIED, ui->lineEditLastModified->text());
    return objAnnotationsAndTags;
}
