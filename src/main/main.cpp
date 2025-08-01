#include <QApplication>
#include <QMessageBox>
#include <QScreen>

#include "AutoUpdate/autoupdate.h"
#include "SplashScreen/splashscreen.h"
#include "g_define.h"
#include "mainwindow.h"
#include "myglobal.h"
#include "myhttp.h"
#include "version.h"

void initConfig();
QString initConnect();
void initUpdate();

int main(int argc, char *argv[]) {
  LOG_INFO("version: {}", APP_VERSION);
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/res/icons/KnowIt.png"));

  SplashScreen splash;
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  int x = screenGeometry.center().x() - splash.width() / 2;
  int y = screenGeometry.center().y() - splash.height() / 2;
  splash.move(x, y);
  splash.show();
  splash.show();

  splash.setStatusText(QObject::tr("init config"));
  initConfig();
  splash.setProgress(1.0 / 4.0);

  splash.setStatusText(QObject::tr("init connect"));
  splash.setStatusText(initConnect());
  splash.setProgress(2.0 / 4.0);

  splash.setStatusText(QObject::tr("init update"));
  initUpdate();
  splash.setProgress(3.0 / 4.0);

  splash.setStatusText(QObject::tr("init GUI"));
  MainWindow w;
  w.showMaximized();
  splash.setProgress(4.0 / 4.0);

  splash.close();
  return a.exec();
}

void initConfig() {
  MY_GLOBAL->set<QString>(URL_SERVER, SETTING_CONFIG_GET(GROUP_SERVER, URL_SERVER, DFT_URL_SERVER));
  MY_GLOBAL->set<QString>(PATH_PING, SETTING_CONFIG_GET(GROUP_SERVER, PATH_PING, DFT_PATH_PING));
  MY_GLOBAL->set<QString>(PATH_UPLOAD_IMG, SETTING_CONFIG_GET(GROUP_SERVER, PATH_UPLOAD_IMG, DFT_PATH_UPLOAD_IMG));
  MY_GLOBAL->set<QString>(PATH_LOAD_IMG, SETTING_CONFIG_GET(GROUP_SERVER, PATH_LOAD_IMG, DFT_PATH_LOAD_IMG));
  MY_GLOBAL->set<QString>(PATH_DATASET_CHECK,
                          SETTING_CONFIG_GET(GROUP_SERVER, PATH_DATASET_CHECK, DFT_PATH_DATASET_CHECK));
  MY_GLOBAL->set<QString>(PATH_DATASET_AUTO_INCREASE_ID, SETTING_CONFIG_GET(GROUP_SERVER, PATH_DATASET_AUTO_INCREASE_ID,
                                                                            DFT_PATH_DATASET_AUTO_INCREASE_ID));
  MY_GLOBAL->set<QString>(PATH_DATASET_SET, SETTING_CONFIG_GET(GROUP_SERVER, PATH_DATASET_SET, DFT_PATH_DATASET_SET));
  MY_GLOBAL->set<QString>(PATH_DATASET_GET, SETTING_CONFIG_GET(GROUP_SERVER, PATH_DATASET_GET, DFT_PATH_DATASET_GET));
  MY_GLOBAL->set<QString>(PATH_DATASET_DEL, SETTING_CONFIG_GET(GROUP_SERVER, PATH_DATASET_DEL, DFT_PATH_DATASET_DEL));
  MY_GLOBAL->set<QString>(PATH_HTML_ANALYSIS,
                          SETTING_CONFIG_GET(GROUP_SERVER, PATH_HTML_ANALYSIS, DFT_PATH_HTML_ANALYSIS));
  MY_GLOBAL->set<QString>(PATH_CV_FIND_CROP,
                          SETTING_CONFIG_GET(GROUP_SERVER, PATH_CV_FIND_CROP, DFT_PATH_CV_FIND_CROP));
  MY_GLOBAL->set<QString>(PATH_CV_FIND_CURVE,
                          SETTING_CONFIG_GET(GROUP_SERVER, PATH_CV_FIND_CURVE, DFT_PATH_CV_FIND_CURVE));
  MY_GLOBAL->set<QString>(PATH_CV_FIND_PEAK,
                          SETTING_CONFIG_GET(GROUP_SERVER, PATH_CV_FIND_PEAK, DFT_PATH_CV_FIND_PEAK));
  MY_GLOBAL->set<QString>(CALIBRATION_START,
                          SETTING_CONFIG_GET(GROUP_CALIBRATION, CALIBRATION_START, DFT_CALIBRATION_START));
  MY_GLOBAL->set<QString>(CALIBRATION_4000,
                          SETTING_CONFIG_GET(GROUP_CALIBRATION, CALIBRATION_4000, DFT_CALIBRATION_4000));
  MY_GLOBAL->set<QString>(CALIBRATION_2000,
                          SETTING_CONFIG_GET(GROUP_CALIBRATION, CALIBRATION_2000, DFT_CALIBRATION_2000));
  MY_GLOBAL->set<QString>(CALIBRATION_0, SETTING_CONFIG_GET(GROUP_CALIBRATION, CALIBRATION_0, DFT_CALIBRATION_0));
  MY_GLOBAL->set<int>(
      CFG_PROPERTY_STRUCTURE_CROP,
      SETTING_CONFIG_GET(CFG_GROUP_PROPERTY, CFG_PROPERTY_STRUCTURE_CROP, DFT_CFG_PROPERTY_STRUCTURE_CROP).toInt());

  QMap<QString, QStringList> m_MajorMinor;
  QStringList list = SETTING_CONFIG_GROUP_KEYS(CFG_GROUP_CHEMICAL_CATEGORY);
  for (auto key : list) {
    m_MajorMinor[key] = SETTING_CONFIG_GET(CFG_GROUP_CHEMICAL_CATEGORY, key).split(DELIMITER);
  }
  MY_GLOBAL->set<QMap<QString, QStringList>>(CHEMICAL_CATEGORY_MAJOR_MINOR, m_MajorMinor);
  MY_GLOBAL->set<QString>(URL_AUTO_UPDATE,
                          SETTING_CONFIG_GET(CFG_GROUP_AUTO_UPDATE, URL_AUTO_UPDATE, DFT_URL_AUTO_UPDATE));
  MY_GLOBAL->set<QString>(CFG_IS_AUTO_UPDATE,
                          SETTING_CONFIG_GET(CFG_GROUP_AUTO_UPDATE, CFG_IS_AUTO_UPDATE, VAL_DISABLE));
}

QString initConnect() {
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_PING));

  while (true) {
    QJsonObject res = MY_HTTP->get_sync(url);

    if (res.contains("pong")) {
      QString msg = QString("pong: %1").arg(res["pong"].toString());
      return msg;
    }

    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Connection failed");
    msgBox.setText("Unable to connect to the server, please check the network or server status.");
    msgBox.setInformativeText("Do you want to retry the connection?");
    msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Retry);

    int ret = msgBox.exec();
    if (ret == QMessageBox::Cancel) {
      return QString();
    }
  }
}

void initUpdate() {
  if (MY_GLOBAL->get<QString>(CFG_IS_AUTO_UPDATE) == VAL_ENABLE) {
    AutoUpdate *update = new AutoUpdate;
    if (update->isNewVersion()) {
      update->show();
    }
  }
}
