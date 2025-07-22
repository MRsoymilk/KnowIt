#include "formapplicationmenu.h"

#include "ui_formapplicationmenu.h"

FormApplicationMenu::FormApplicationMenu(QWidget *parent) : QWidget(parent), ui(new Ui::FormApplicationMenu) {
  ui->setupUi(this);
  init();
}

FormApplicationMenu::~FormApplicationMenu() { delete ui; }

void FormApplicationMenu::init() {
  ui->tBtnMineIt->setToolTip("MineIt");
  ui->tBtnMineIt->setObjectName("MineIt");
  ui->tBtnMineIt->setIcon(QIcon(":/res/icons/MineIt.png"));
  ui->tBtnMineIt->setIconSize(QSize(64, 64));
  ui->tBtnMineIt->setCheckable(true);
  ui->tBtnMineIt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  ui->tBtnMineIt->setChecked(true);
}

void FormApplicationMenu::on_tBtnMineIt_clicked() {}
