#include "myhttp.h"

#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QPixmap>

MyHttp::MyHttp(QObject *parent) : QObject(parent) { m_manager = new QNetworkAccessManager(this); }

MyHttp *MyHttp::instance() {
  static MyHttp instance;
  return &instance;
}

QJsonObject MyHttp::post_sync(const QString &url, const QJsonObject &json) {
  QUrl qurl(url);
  QNetworkRequest request(qurl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QByteArray data = QJsonDocument(json).toJson();
  QNetworkReply *reply = m_manager->post(request, data);

  QEventLoop loop;
  connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  QJsonObject result;
  if (reply->error() != QNetworkReply::NoError) {
    qWarning() << "[MyHttp] HTTP Error:" << reply->errorString();
  } else {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &err);
    if (err.error == QJsonParseError::NoError && doc.isObject()) {
      result = doc.object();
    } else {
      qWarning() << "[MyHttp] JSON parse error:" << err.errorString();
    }
  }

  reply->deleteLater();
  return result;
}

void MyHttp::post_async(const QString &url, const QJsonObject &json, std::function<void(QJsonObject)> onSuccess,
                        std::function<void(QString)> onError) {
  QUrl qurl(url);
  QNetworkRequest request(qurl);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

  QByteArray data = QJsonDocument(json).toJson();
  QNetworkReply *reply = m_manager->post(request, data);

  connect(reply, &QNetworkReply::finished, this, [=]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      if (onError) onError(reply->errorString());
      return;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
      if (onError) onError("JSON Parse Error: " + err.errorString());
      return;
    }

    if (onSuccess) onSuccess(doc.object());
  });
}

void MyHttp::postImage(const QString &url, const QString &imagePath, const QJsonObject &json,
                       std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError) {
  QFile *file = new QFile(imagePath);
  if (!file->open(QIODevice::ReadOnly)) {
    onError("无法打开图片文件");
    file->deleteLater();
    return;
  }

  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

  // 1. 添加图片部分
  QHttpPart imagePart;
  imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                      QVariant("form-data; name=\"file\"; filename=\"" + QFileInfo(*file).fileName() + "\""));
  imagePart.setHeader(QNetworkRequest::ContentTypeHeader,
                      QVariant("image/png"));  // 或根据扩展名设置
  imagePart.setBodyDevice(file);
  file->setParent(multiPart);  // 自动释放
  multiPart->append(imagePart);

  // 2. 添加 JSON 字段（以表单方式嵌入每个字段）
  for (const QString &key : json.keys()) {
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"" + key + "\""));
    textPart.setBody(json.value(key).toString().toUtf8());
    multiPart->append(textPart);
  }

  QUrl qurl(url);
  QNetworkRequest request(qurl);
  QNetworkReply *reply = m_manager->post(request, multiPart);
  multiPart->setParent(reply);  // 自动删除

  connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() {
    reply->deleteLater();
    if (reply->error()) {
      onError(reply->errorString());
      return;
    }

    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
      onError("响应不是有效的 JSON");
      return;
    }

    onSuccess(doc.object());
  });
}

void MyHttp::getImage(const QString &url, std::function<void(QPixmap)> onSuccess,
                      std::function<void(QString)> onError) {
  QUrl qurl(url);
  QNetworkRequest request(qurl);
  QNetworkReply *reply = m_manager->get(request);

  connect(reply, &QNetworkReply::finished, this, [reply, onSuccess, onError]() {
    reply->deleteLater();
    if (reply->error()) {
      onError(reply->errorString());
      return;
    }

    QByteArray data = reply->readAll();
    QPixmap pixmap;
    if (!pixmap.loadFromData(data)) {
      onError("无法解析图片数据");
      return;
    }

    onSuccess(pixmap);
  });
}

QJsonObject MyHttp::get_sync(const QString &url) {
  QUrl qurl(url);
  QNetworkRequest request(qurl);
  QNetworkReply *reply = m_manager->get(request);

  QEventLoop loop;
  connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  QJsonObject result;

  if (reply->error() == QNetworkReply::NoError) {
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isObject()) {
      result = doc.object();
    }
  } else {
    qWarning() << "[MyHttp] GET Error:" << reply->errorString();
  }

  reply->deleteLater();
  return result;
}

void MyHttp::get_async(const QString &url, std::function<void(QJsonObject)> onSuccess,
                       std::function<void(QString)> onError) {
  QUrl qurl(url);
  QNetworkRequest request(qurl);
  QNetworkReply *reply = m_manager->get(request);

  connect(reply, &QNetworkReply::finished, [reply, onSuccess, onError]() {
    if (reply->error() == QNetworkReply::NoError) {
      QByteArray data = reply->readAll();
      QJsonDocument doc = QJsonDocument::fromJson(data);
      if (doc.isObject()) {
        onSuccess(doc.object());
      } else {
        onError("Invalid JSON format");
      }
    } else {
      onError(reply->errorString());
    }
    reply->deleteLater();
  });
}
