#include "updatesetting.h"

#include "AutoUpdate/autoupdate.h"
#include "ui_updatesetting.h"

UpdateSetting::UpdateSetting(QWidget *parent) : QWidget(parent), ui(new Ui::UpdateSetting) { ui->setupUi(this); }

UpdateSetting::~UpdateSetting() { delete ui; }

void UpdateSetting::on_btnStartUpdate_clicked() {
  AutoUpdate *update = new AutoUpdate;
  update->show();
}
