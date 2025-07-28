#include "formapplicationmenu.h"

#include "ui_formapplicationmenu.h"

FormApplicationMenu::FormApplicationMenu(QWidget *parent) : QWidget(parent), ui(new Ui::FormApplicationMenu) {
  ui->setupUi(this);
  init();
}

FormApplicationMenu::~FormApplicationMenu() { delete ui; }

void FormApplicationMenu::retranslateUI() { ui->retranslateUi(this); }

void FormApplicationMenu::init() {
  ui->tBtnMineIt->setToolTip("MineIt");
  ui->tBtnMineIt->setObjectName("MineIt");
  ui->tBtnMineIt->setIcon(QIcon(":/res/icons/MineIt.png"));
  ui->tBtnMineIt->setIconSize(QSize(64, 64));
  ui->tBtnMineIt->setCheckable(true);
  ui->tBtnMineIt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  ui->tBtnMineIt->setChecked(true);

  ui->tBtnAnalyzeIt->setVisible(false);
  ui->tBtnIDExpert->setVisible(false);
  ui->tBtnQCExpert->setVisible(false);
  ui->tBtnPredictIt->setVisible(false);
  ui->tBtnSearchIt->setVisible(false);

  ui->tBtnAnalyzeIt->setObjectName("AnalyzeIt");
  ui->tBtnIDExpert->setObjectName("IDExpert");
  ui->tBtnQCExpert->setObjectName("QCExpert");
  ui->tBtnPredictIt->setObjectName("PredictIt");
  ui->tBtnSearchIt->setObjectName("SearchIt");

  // ui->pageSpectralAnalysis->hide();
  ui->toolBox->setCurrentIndex(0);
}

void FormApplicationMenu::on_tBtnMineIt_clicked() {}
