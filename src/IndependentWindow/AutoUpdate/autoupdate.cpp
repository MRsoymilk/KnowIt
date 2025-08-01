#include "autoupdate.h"

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QStandardPaths>

#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_autoupdate.h"
#include "version.h"

AutoUpdate::AutoUpdate(QWidget *parent) : QWidget(parent), ui(new Ui::AutoUpdate) { ui->setupUi(this); }

AutoUpdate::~AutoUpdate() { delete ui; }

QJsonObject AutoUpdate::checkUpdate() {
  m_url = MY_GLOBAL->get<QString>(URL_AUTO_UPDATE);
  QString url_json = QString("%1/%2").arg(m_url, "update.json");
  m_objUpdate = MY_HTTP->get_sync(url_json);
  return m_objUpdate;
}

bool AutoUpdate::isNewVersion() {
  checkUpdate();

  QString currentVersion = APP_VERSION;
  QString newVersion = m_objUpdate["version"].toString();

  QStringList curList = currentVersion.split('.');
  QStringList newList = newVersion.split('.');

  int maxCount = qMax(curList.size(), newList.size());

  while (curList.size() < maxCount) curList << "0";
  while (newList.size() < maxCount) newList << "0";

  for (int i = 0; i < maxCount; ++i) {
    int curPart = curList[i].toInt();
    int newPart = newList[i].toInt();

    if (newPart > curPart)
      return true;
    else if (newPart < curPart)
      return false;
  }

  return false;
}

void AutoUpdate::showEvent(QShowEvent *event) {
  QJsonObject res = checkUpdate();

  ui->radioButtonExeOnly->setChecked(true);
  ui->progressBar->setVisible(false);

  ui->lineEditURL->setText(QString("%1/%2").arg(res["url"].toString(), "latest/KnowIt.exe"));
  ui->lineEditTargetVersion->setText(res["version"].toString());
  ui->lineEditCurrentVersion->setText(APP_VERSION);
  ui->labelTimestamp->setText(res["timestamp"].toString());
  ui->textBrowserDescription->setText(res["description"].toString());
}

void AutoUpdate::on_btnUpdate_clicked() {
  ui->progressBar->setVisible(true);
  QString to_downloaded;
  if (ui->radioButtonExeOnly->isChecked()) {
    to_downloaded = "KnowIt.exe";
  } else if (ui->radioButtonFull->isChecked()) {
    to_downloaded = m_objUpdate["file"].toString();
  }

  MY_HTTP->downloadBinary(
      ui->lineEditURL->text(),
      [=](QByteArray data) {
        QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString fullPath = QDir(tempDir).filePath(to_downloaded);

        QFile file(fullPath);
        if (file.open(QIODevice::WriteOnly)) {
          file.write(data);
          file.close();
          ui->textBrowserDescription->append("✅ 下载完成并保存成功：" + fullPath);

          if (ui->radioButtonFull->isChecked()) {
            QProcess::startDetached(fullPath);
          } else {
            QString currentExePath = QCoreApplication::applicationFilePath();
            QString currentExeName = QFileInfo(currentExePath).fileName();

            QString updaterPath = QDir(tempDir).filePath("updater.bat");

            QFile updater(updaterPath);
            if (updater.open(QIODevice::WriteOnly | QIODevice::Text)) {
              QTextStream out(&updater);
              out << "@echo off\n";
              out << "timeout /t 2 /nobreak > nul\n";
              out << "move /Y \"" << fullPath.replace("/", "\\") << "\" \"" << currentExePath.replace("/", "\\")
                  << "\"\n";
              out << "start \"\" \"" << currentExePath.replace("/", "\\") << "\"\n";
              out << "del \"%~f0\"\n";
              updater.close();

              QProcess::startDetached("cmd.exe", {"/C", updaterPath});
              QApplication::exit(0);
            } else {
              MSG_WARN(tr("Unable to write update script!"));
            }
          }
        } else {
          ui->textBrowserDescription->append("❌ 无法保存文件：" + fullPath);
        }
      },
      [=](QString err) { ui->textBrowserDescription->append("❌ 下载失败: " + err); },
      [=](qint64 received, qint64 total) {
        if (total > 0) {
          int percent = static_cast<int>((double(received) / total) * 100);
          ui->progressBar->setValue(percent);
        }
      });
}

void AutoUpdate::on_radioButtonExeOnly_clicked() {
  ui->lineEditURL->setText(QString("%1/%2").arg(m_url, "latest/KnowIt.exe"));
}

void AutoUpdate::on_radioButtonFull_clicked() {
  ui->lineEditURL->setText(QString("%1/%2").arg(m_url, m_objUpdate["file"].toString()));
}
