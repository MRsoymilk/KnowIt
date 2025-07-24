#include "updatesetting.h"

#include "AutoUpdate/autoupdate.h"
#include "g_define.h"
#include "myglobal.h"
#include "ui_updatesetting.h"

UpdateSetting::UpdateSetting(QWidget *parent) : QWidget(parent), ui(new Ui::UpdateSetting) {
  ui->setupUi(this);
  init();
}

UpdateSetting::~UpdateSetting() { delete ui; }

void UpdateSetting::init() {
  if (MY_GLOBAL->get<QString>(CFG_IS_AUTO_UPDATE) == VAL_ENABLE) {
    ui->checkBoxAutoUpdate->setChecked(true);
  } else {
    ui->checkBoxAutoUpdate->setChecked(false);
  }
  ui->lineEditURL->setText(MY_GLOBAL->get<QString>(URL_AUTO_UPDATE));
}

void UpdateSetting::on_btnStartUpdate_clicked() {
  AutoUpdate *update = new AutoUpdate;
  update->show();
}

void UpdateSetting::on_checkBoxAutoUpdate_clicked() {
  if (ui->checkBoxAutoUpdate->isChecked()) {
    MY_GLOBAL->set<QString>(CFG_IS_AUTO_UPDATE, VAL_ENABLE);
    SETTING_CONFIG_SET(CFG_GROUP_AUTO_UPDATE, CFG_IS_AUTO_UPDATE, VAL_ENABLE);
  } else {
    MY_GLOBAL->set<QString>(CFG_IS_AUTO_UPDATE, VAL_DISABLE);
    SETTING_CONFIG_SET(CFG_GROUP_AUTO_UPDATE, CFG_IS_AUTO_UPDATE, VAL_DISABLE);
  }
}
