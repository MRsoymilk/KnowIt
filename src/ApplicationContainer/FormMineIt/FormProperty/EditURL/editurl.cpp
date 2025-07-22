#include "editurl.h"

#include <QJsonObject>
#include <QMessageBox>

#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_editurl.h"

EditURL::EditURL(QWidget *parent) : QWidget(parent), ui(new Ui::EditURL) { ui->setupUi(this); }

EditURL::~EditURL() { delete ui; }

bool EditURL::check() {
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_HTML_ANALYSIS));

  QString urlRamanSpectralView = ui->lineEditURL_RamanSpectralView->text();
  // QJsonObject objCheckRaman;
  // objCheckRaman.insert(KEY_URL_RAMAN_SPECTRAL_VIEW, urlRamanSpectralView);
  // QJsonObject resRaman = MY_HTTP->post_sync(url, objCheckRaman);

  QString urlCompoundView = ui->lineEditURL_CompoundView->text();
  // QJsonObject objCheckCompound;
  // objCheckCompound.insert(KEY_URL_COMPOUND_VIEW, urlCompoundView);
  // QJsonObject resCompound = MY_HTTP->post_sync(url, objCheckCompound);

  if (urlRamanSpectralView.isEmpty() || urlCompoundView.isEmpty()) {
    QMessageBox::warning(this, "warning", "URL is empty!");
  }

  return true;
}
