#ifndef MYGLOBAL_H
#define MYGLOBAL_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QVariant>

class MyGlobal : public QObject {
  Q_OBJECT

 public:
  static MyGlobal *instance();

  template <typename T>
  void set(const QString &key, const T &value) {
    m_data[key] = QVariant::fromValue(value);
  }

  template <typename T>
  T get(const QString &key, const T &defaultValue = T()) const {
    return m_data.value(key, defaultValue).template value<T>();
  }

  void setValue(const QString &key, const QVariant &value);
  QVariant getValue(const QString &key, const QVariant &defaultValue = QVariant()) const;

 private:
  explicit MyGlobal(QObject *parent = nullptr);
  MyGlobal(const MyGlobal &) = delete;
  MyGlobal &operator=(const MyGlobal &) = delete;

  QMap<QString, QVariant> m_data;
};

#define MY_GLOBAL MyGlobal::instance()

#endif  // MYGLOBAL_H

/*
// set
MyGlobal::instance()->set<QString>("username", "milksoy");
MyGlobal::instance()->set<int>("age", 28);
MyGlobal::instance()->set<bool>("loggedIn", true);

// get
QString name = MyGlobal::instance()->get<QString>("username");
int age = MyGlobal::instance()->get<int>("age", 0);
bool status = MyGlobal::instance()->get<bool>("loggedIn", false);
*/
