#include "originate.h"
#include "ui_originate.h"

Originate::Originate(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Originate)
{
    ui->setupUi(this);
}

Originate::~Originate()
{
    delete ui;
}
