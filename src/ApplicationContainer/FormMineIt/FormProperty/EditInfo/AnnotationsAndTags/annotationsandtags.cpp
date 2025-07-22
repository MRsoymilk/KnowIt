#include "annotationsandtags.h"
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
