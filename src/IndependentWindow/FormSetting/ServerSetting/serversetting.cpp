#include "serversetting.h"

#include "g_define.h"
#include "myglobal.h"
#include "ui_serversetting.h"

ServerSetting::ServerSetting(QWidget *parent) : QWidget(parent), ui(new Ui::ServerSetting) {
  ui->setupUi(this);
  init();
}

ServerSetting::~ServerSetting() { delete ui; }

void ServerSetting::init() {
  ui->lineEditUrl->setText(MY_GLOBAL->get<QString>(URL_SERVER));
  ui->lineEditPathPing->setText(MY_GLOBAL->get<QString>(PATH_PING));
  ui->lineEditPathUploadImg->setText(MY_GLOBAL->get<QString>(PATH_UPLOAD_IMG));
  ui->lineEditPathLoadImg->setText(MY_GLOBAL->get<QString>(PATH_LOAD_IMG));
  ui->lineEditPathDatasetCheck->setText(MY_GLOBAL->get<QString>(PATH_DATASET_CHECK));
  ui->lineEditPathDatasetSet->setText(MY_GLOBAL->get<QString>(PATH_DATASET_SET));
  ui->lineEditPathDatasetGet->setText(MY_GLOBAL->get<QString>(PATH_DATASET_GET));
  ui->lineEditPathDatasetDel->setText(MY_GLOBAL->get<QString>(PATH_DATASET_DEL));
  ui->lineEditPathHtmlAnalysis->setText(MY_GLOBAL->get<QString>(PATH_HTML_ANALYSIS));
  ui->lineEditPathCvFindCrop->setText(MY_GLOBAL->get<QString>(PATH_CV_FIND_CROP));
  ui->lineEditPathCvFindPeak->setText(MY_GLOBAL->get<QString>(PATH_CV_FIND_PEAK));
  ui->lineEditPathCvFindCurve->setText(MY_GLOBAL->get<QString>(PATH_CV_FIND_CURVE));
}
