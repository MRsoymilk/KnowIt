#ifndef MYSETTING_H
#define MYSETTING_H

#include <QSettings>
#include <QString>

class MySetting {
 public:
  enum class SETTING { CONFIG };

 public:
  static MySetting &getInstance();
  ~MySetting();
  void setValue(SETTING s, const QString &group, const QString &key, const QString &val);
  QString getValue(SETTING s, const QString &group, const QString &key, const QString &val_dft = "");
  QStringList getGroups(SETTING s);
  QStringList getGroupKeys(SETTING s, const QString &group);
  void sync(SETTING s);
  void removeKey(SETTING s, const QString &group, const QString &key);

 private:
  MySetting();

 private:
  QMap<SETTING, QSettings *> m_settings;
};

#define MY_SETTING MySetting::getInstance()

#endif  // MYSETTING_H
