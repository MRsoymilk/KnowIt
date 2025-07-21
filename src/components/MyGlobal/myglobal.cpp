#include "myglobal.h"

MyGlobal::MyGlobal(QObject *parent)
    : QObject(parent)
{}

MyGlobal *MyGlobal::instance()
{
    static MyGlobal store;
    return &store;
}

void MyGlobal::setValue(const QString &key, const QVariant &value)
{
    m_data[key] = value;
}

QVariant MyGlobal::getValue(const QString &key, const QVariant &defaultValue) const
{
    return m_data.value(key, defaultValue);
}
