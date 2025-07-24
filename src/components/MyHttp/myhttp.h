#ifndef MYHTTP_H
#define MYHTTP_H

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <functional>

class MyHttp : public QObject {
  Q_OBJECT
 public:
  static MyHttp *instance();

  // 同步返回 JSON 对象（阻塞）
  QJsonObject post_sync(const QString &url, const QJsonObject &json);

  // 异步 JSON 请求：成功和失败使用回调函数处理
  void post_async(const QString &url, const QJsonObject &json, std::function<void(QJsonObject)> onSuccess,
                  std::function<void(QString)> onError);
  // 同步 GET 请求，返回 QJsonObject
  QJsonObject get_sync(const QString &url);

  // 异步 GET 请求
  void get_async(const QString &url, std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError);
  void postImage(const QString &url, const QString &imagePath, const QJsonObject &json,
                 std::function<void(QJsonObject)> onSuccess, std::function<void(QString)> onError);
  void getImage(const QString &url, std::function<void(QPixmap)> onSuccess, std::function<void(QString)> onError);

  void downloadBinary(const QString &url, std::function<void(QByteArray)> onSuccess,
                      std::function<void(QString)> onError,
                      std::function<void(qint64 bytesReceived, qint64 bytesTotal)> onProgress = nullptr);

 private:
  explicit MyHttp(QObject *parent = nullptr);
  MyHttp(const MyHttp &) = delete;
  MyHttp &operator=(const MyHttp &) = delete;

  QNetworkAccessManager *m_manager;
};

#define MY_HTTP MyHttp::instance()

#endif  // MYHTTP_H

/*
QJsonObject req;
req["username"] = "admin";
req["password"] = "123456";

MyHttp::instance()->post_async(
    QUrl("http://localhost:8000/login"),
    req,
    [](const QJsonObject &res) {
        qDebug() << "[成功] 登录返回：" << res;
    },
    [](const QString &err) {
        qWarning() << "[失败] 请求错误：" << err;
    }
);


// 上传图片
MY_HTTP->postImage(QUrl("http://localhost:8000/upload"), "/path/to/img.png",
    [](QJsonObject obj) { qDebug() << "Upload OK:" << obj; },
    [](QString err) { qDebug() << "Upload Error:" << err; });

// 下载图片
MY_HTTP->getImage(QUrl("http://localhost:8000/image.png"),
    [=](QPixmap pix) { ui->labelImage->setPixmap(pix); },
    [=](QString err) { qDebug() << "Download Error:" << err; });

MY_HTTP->downloadBinary("http://192.168.123.233:8010/latest/KnowIt.exe",
 [](QByteArray data) {
     QFile file("KnowIt_Downloaded.exe");
     if (file.open(QIODevice::WriteOnly)) {
         file.write(data);
         file.close();
         qDebug() << "✅ 下载完成并保存成功";
     }
 },
 [](QString err) {
     qWarning() << "❌ 下载失败：" << err;
 },
 [](qint64 received, qint64 total) {
     qDebug() << "📥 下载进度：" << received << "/" << total;
 });
*/
