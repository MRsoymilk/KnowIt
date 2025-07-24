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

QJsonObject AutoUpdate::onCheckUpdate() {
  m_url = MY_GLOBAL->get<QString>(URL_AUTO_UPDATE);
  QString url_json = QString("%1/%2").arg(m_url, "update.json");
  m_objUpdate = MY_HTTP->get_sync(url_json);
  return m_objUpdate;
}

void AutoUpdate::showEvent(QShowEvent *event) {
  QJsonObject res = onCheckUpdate();

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
  if (ui->radioButtonExeOnly->isChecked()) {
  } else if (ui->radioButtonFull->isChecked()) {
  }
  MY_HTTP->downloadBinary(
      ui->lineEditURL->text(),

      // 成功下载回调
      [=](QByteArray data) {
        QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        QString fileName = m_objUpdate["file"].toString();
        QString fullPath = QDir(tempDir).filePath(fileName);

        QFile file(fullPath);
        if (file.open(QIODevice::WriteOnly)) {
          file.write(data);
          file.close();
          qDebug() << "✅ 下载完成并保存成功：" << fullPath;

          // 自动执行下载的文件
          QProcess::startDetached(fullPath);
        } else {
          ui->textBrowserDescription->append("❌ 无法保存文件：" + fullPath);
        }
      },

      // 错误处理
      [=](QString err) { ui->textBrowserDescription->append("❌ 下载失败: " + err); },

      // 进度显示
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
