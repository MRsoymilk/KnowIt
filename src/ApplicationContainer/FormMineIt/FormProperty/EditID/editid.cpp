#include "editid.h"

#include <QMessageBox>

#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_editid.h"

EditID::EditID(QWidget *parent) : QWidget(parent), ui(new Ui::EditID) {
  ui->setupUi(this);
  init();
}

EditID::~EditID() { delete ui; }

bool EditID::check() {
  updateID();
  QJsonObject objCheck;
  objCheck.insert(ID, m_id);
  QJsonObject res = MY_HTTP->post_sync("http://192.168.123.233:5020/dataset/check", objCheck);
  if (res["status"].toBool()) {
    MY_GLOBAL->set<QString>(ID, m_id);
    return true;
  } else {
    QMessageBox::warning(this, tr("warning"), tr("ID %1 already exists, please re-enter!").arg(m_id));
    return false;
  }
}

void EditID::init() {
  ui->comboBoxSpectralType->addItems({"RAM", "NIR", "MIR", "UV"});
  m_mapMajorMinor = {
      {"PP", {"MON", "BAS", "IND"}},
      {"OC", {"BAS", "SOL", "GAS"}},
      {"IC", {"ADD", "MAT", "INT", "PET", "SOL"}},
      {"BA", {"ANC", "PHA", "STE", "PCA"}},
      {"CI", {"DRU", "TOX", "FES", "COA", "CAB", "FAD", "STE"}},
      {"EP", {"INS", "WGP"}},
      {"IO", {"INO", "MOC", "MIN"}},
  };

  ui->comboBoxMajor->addItems(m_mapMajorMinor.keys());
  QString defaultMajor = ui->comboBoxMajor->currentText();
  ui->comboBoxMinor->addItems(m_mapMajorMinor.value(defaultMajor));
}

void EditID::updateID() {
  m_id = QString("%1_%2-%3_%4")
             .arg(ui->comboBoxSpectralType->currentText(), ui->comboBoxMajor->currentText(),
                  ui->comboBoxMinor->currentText(),
                  QString("%1").arg(ui->spinBoxSpecificNumber->value(), 4, 10, QChar('0')));
}

void EditID::on_comboBoxMajor_currentTextChanged(const QString &major) {
  ui->comboBoxMinor->clear();
  ui->comboBoxMinor->addItems(m_mapMajorMinor.value(major));
}
