#include "spectraldata.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QJsonArray>
#include <QMessageBox>
#include <QRegularExpression>
#include <QWheelEvent>

#include "../EditContainer/editcontainer.h"
#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_spectraldata.h"

SpectralData::SpectralData(QWidget *parent) : QWidget(parent), ui(new Ui::SpectralData) {
  ui->setupUi(this);
  init();
}

SpectralData::~SpectralData() { delete ui; }

void SpectralData::init() {
  ui->tBtnRedoCrop->setVisible(false);

  ui->graphicsViewSpectral->viewport()->installEventFilter(this);

  ui->tBtnEditPeakAssistance->setObjectName("edit");
  ui->tBtnEditPeakMain->setObjectName("edit");

  ui->tBtnUploadSpectral->setObjectName("upload");
  ui->tBtnFindPeak->setObjectName("find_peak");
  ui->tBtnFindPeak->setToolTip(tr("find peak"));
  ui->tBtnFittingCurve->setObjectName("fitting_curve");
  ui->tBtnFittingCurve->setToolTip(tr("fitting curve"));

  initCalibration();
  fillCalibration();
}

void SpectralData::initCalibration() {
  toolButtonGroup = new QButtonGroup(this);
  toolButtonGroup->setExclusive(true);
  ui->tBtnPointStart->setCheckable(true);
  ui->tBtnPoint4000->setCheckable(true);
  ui->tBtnPoint2000->setCheckable(true);
  ui->tBtnPoint0->setCheckable(true);

  toolButtonGroup->addButton(ui->tBtnPointStart, 0);
  toolButtonGroup->addButton(ui->tBtnPoint4000, 1);
  toolButtonGroup->addButton(ui->tBtnPoint2000, 2);
  toolButtonGroup->addButton(ui->tBtnPoint0, 3);
  ui->tBtnPointStart->setChecked(true);
}

void SpectralData::fillCalibration() {
  m_pointData[0] = {ui->labelPointStart, QPoint(), nullptr};
  m_pointData[1] = {ui->labelPoint4000, QPoint(), nullptr};
  m_pointData[2] = {ui->labelPoint2000, QPoint(), nullptr};
  m_pointData[3] = {ui->labelPoint0, QPoint(), nullptr};

  QRegularExpression regex(R"(\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\))");
  {
    QString strPointStart = MY_GLOBAL->get<QString>(CALIBRATION_START);
    QRegularExpressionMatch match = regex.match(strPointStart);
    QPoint point;
    if (match.hasMatch()) {
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x != 0 && y != 0) {
        m_pointData[0].point = QPoint(x, y);
        PointData &data = m_pointData[0];
        QString coordText = QString("X: %1, Y: %2").arg(x).arg(y);
        data.label->setText(coordText);
        if (ui->graphicsViewSpectral->scene()) {
          if (data.item) {
            // 已经画过，删除图元
            ui->graphicsViewSpectral->scene()->removeItem(data.item);
            data.item = nullptr;
            data.label->clear();

          } else {
            // 绘制新点
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
            ellipse->setPos(x, y);
            ellipse->setBrush(Qt::red);
            ellipse->setPen(Qt::NoPen);
            ui->graphicsViewSpectral->scene()->addItem(ellipse);

            // 保存数据
            data.item = ellipse;
            data.point = QPoint(x, y);
          }
        }
      }
    }
  }
  {
    QString strPoint4000 = MY_GLOBAL->get<QString>(CALIBRATION_4000);
    QRegularExpressionMatch match = regex.match(strPoint4000);
    QPoint point;
    if (match.hasMatch()) {
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x != 0 && y != 0) {
        m_pointData[1].point = QPoint(x, y);
        PointData &data = m_pointData[1];
        QString coordText = QString("X: %1, Y: %2").arg(x).arg(y);
        data.label->setText(coordText);
        if (ui->graphicsViewSpectral->scene()) {
          if (data.item) {
            // 已经画过，删除图元
            ui->graphicsViewSpectral->scene()->removeItem(data.item);
            data.item = nullptr;
            data.label->clear();

          } else {
            // 绘制新点
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
            ellipse->setPos(x, y);
            ellipse->setBrush(Qt::red);
            ellipse->setPen(Qt::NoPen);
            ui->graphicsViewSpectral->scene()->addItem(ellipse);

            // 保存数据
            data.item = ellipse;
            data.point = QPoint(x, y);
          }
        }
      }
    }
  }
  {
    QString strPoint2000 = MY_GLOBAL->get<QString>(CALIBRATION_2000);
    QRegularExpressionMatch match = regex.match(strPoint2000);
    QPoint point;
    if (match.hasMatch()) {
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x != 0 && y != 0) {
        m_pointData[2].point = QPoint(x, y);
        PointData &data = m_pointData[2];
        QString coordText = QString("X: %1, Y: %2").arg(x).arg(y);
        data.label->setText(coordText);
        if (ui->graphicsViewSpectral->scene()) {
          if (data.item) {
            // 已经画过，删除图元
            ui->graphicsViewSpectral->scene()->removeItem(data.item);
            data.item = nullptr;
            data.label->clear();

          } else {
            // 绘制新点
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
            ellipse->setPos(x, y);
            ellipse->setBrush(Qt::red);
            ellipse->setPen(Qt::NoPen);
            ui->graphicsViewSpectral->scene()->addItem(ellipse);

            // 保存数据
            data.item = ellipse;
            data.point = QPoint(x, y);
          }
        }
      }
    }
  }
  {
    QString strPoint0 = MY_GLOBAL->get<QString>(CALIBRATION_0);
    QRegularExpressionMatch match = regex.match(strPoint0);
    QPoint point;
    if (match.hasMatch()) {
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x != 0 && y != 0) {
        m_pointData[3].point = QPoint(x, y);
        PointData &data = m_pointData[3];
        QString coordText = QString("X: %1, Y: %2").arg(x).arg(y);
        data.label->setText(coordText);
        if (ui->graphicsViewSpectral->scene()) {
          if (data.item) {
            // 已经画过，删除图元
            ui->graphicsViewSpectral->scene()->removeItem(data.item);
            data.item = nullptr;
            data.label->clear();

          } else {
            // 绘制新点
            QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
            ellipse->setPos(x, y);
            ellipse->setBrush(Qt::red);
            ellipse->setPen(Qt::NoPen);
            ui->graphicsViewSpectral->scene()->addItem(ellipse);

            // 保存数据
            data.item = ellipse;
            data.point = QPoint(x, y);
          }
        }
      }
    }
  }
}

void SpectralData::setID(const QString &id) { m_id = id; }

void SpectralData::setUUID(const QString &uuid) { m_uuid = uuid; }

void SpectralData::setSpectralData(const QJsonObject &data) {
  QJsonObject objSpectralData = data[SPECTRAL_DATA].toObject();
  ui->lineEditPeakMain->setText(objSpectralData.value(PEAK_MAIN).toString());
  ui->lineEditSpectralPicturePath->setText(objSpectralData.value(SPECTRAL_PICTURE).toString());

  QString url_get_img = QString("%1%2/%3/%4")
                            .arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_LOAD_IMG), m_id,
                                 ui->lineEditSpectralPicturePath->text());
  MY_HTTP->getImage(
      url_get_img,
      [=](QPixmap pix) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pix);
        scene->setSceneRect(pix.rect());

        ui->graphicsViewSpectral->setScene(scene);
        // ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        ui->graphicsViewSpectral->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        ui->graphicsViewSpectral->setDragMode(QGraphicsView::ScrollHandDrag);
        emit sendSpectral(pix);
      },
      [=](QString err) { qDebug() << "Download Error:" << err; });
}

QJsonObject SpectralData::getSpectralData() {
  QJsonObject objSpectralData;
  objSpectralData.insert(PEAK_MAIN, ui->lineEditPeakMain->text());
  objSpectralData.insert(PEAK_ASSISTANCE, ui->lineEditPeakAssistance->text());
  objSpectralData.insert(SPECTRAL_PICTURE, ui->lineEditSpectralPicturePath->text());
  return objSpectralData;
}

bool SpectralData::drawPoint(const QPointF &scenePos, const int &pixelX, const int &pixelY,
                             QAbstractButton *checkedBtn) {
  int id = toolButtonGroup->id(checkedBtn);
  if (!m_pointData.contains(id)) {
    qWarning() << "Unknown tool button ID:" << id;
    return true;
  }

  PointData &data = m_pointData[id];
  QString coordinate = QString("(%1, %2)").arg(pixelX).arg(pixelY);
  switch (id) {
    case 0:
      MY_GLOBAL->set<QString>(CALIBRATION_START, coordinate);
      break;
    case 1:
      MY_GLOBAL->set<QString>(CALIBRATION_4000, coordinate);
      break;
    case 2:
      MY_GLOBAL->set<QString>(CALIBRATION_2000, coordinate);
      break;
    case 3:
      MY_GLOBAL->set<QString>(CALIBRATION_0, coordinate);
      break;
    default:
      break;
  }

  if (data.item) {
    // 已经画过，删除图元
    ui->graphicsViewSpectral->scene()->removeItem(data.item);
    data.item = nullptr;
    data.label->clear();

    // 重新选中当前按钮
    checkedBtn->setChecked(true);
  } else {
    // 绘制新点
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
    ellipse->setPos(scenePos);
    ellipse->setBrush(Qt::red);
    ellipse->setPen(Qt::NoPen);
    ui->graphicsViewSpectral->scene()->addItem(ellipse);

    // 保存数据
    data.item = ellipse;
    data.point = QPoint(pixelX, pixelY);
    QString coordText = QString("X: %1, Y: %2").arg(pixelX).arg(pixelY);
    data.label->setText(coordText);

    // 当前按钮取消选中
    checkedBtn->setChecked(false);

    // 设置下一个按钮为选中状态
    QAbstractButton *nextBtn = toolButtonGroup->button(id + 1);
    if (nextBtn) {
      nextBtn->setChecked(true);
    }
  }
  return true;
}

bool SpectralData::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->graphicsViewSpectral->viewport() && event->type() == QEvent::Wheel) {
    return handleGraphicsViewSpectral(event);
  }

  if (watched == ui->graphicsViewSpectral->viewport() && event->type() == QEvent::MouseButtonPress) {
    if (ui->graphicsViewSpectral->scene() == nullptr) {
      return true;
    }

    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

    if (mouseEvent->button() == Qt::RightButton) {
      QPointF scenePos = ui->graphicsViewSpectral->mapToScene(mouseEvent->pos());
      int pixelX = static_cast<int>(scenePos.x());
      int pixelY = static_cast<int>(scenePos.y());

      QAbstractButton *checkedBtn = toolButtonGroup->checkedButton();
      if (!checkedBtn) {
        return true;
      }

      drawPoint(scenePos, pixelX, pixelY, checkedBtn);

      return true;
    }
  }

  return QWidget::eventFilter(watched, event);  // 其他事件交由默认处理
}
bool SpectralData::handleGraphicsViewSpectral(QEvent *event) {
  if (ui->graphicsViewSpectral->scene() == nullptr) {
    return true;
  }

  QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
  const double scaleFactor = 1.15;

  if (wheelEvent->angleDelta().y() > 0) {
    ui->graphicsViewSpectral->scale(scaleFactor, scaleFactor);  // 放大
  } else {
    ui->graphicsViewSpectral->scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // 缩小
  }
  return true;  // 已处理事件
}

void SpectralData::drawCropOnImage(const QPixmap &pixmap, const QJsonObject &res) {
  if (ui->graphicsViewSpectral->scene()) {
    delete ui->graphicsViewSpectral->scene();
    ui->graphicsViewSpectral->setScene(nullptr);
  }
  QGraphicsScene *scene = new QGraphicsScene(this);
  scene->addPixmap(pixmap);

  auto getPoint = [&](const QString &key) -> QPointF {
    QJsonArray arr = res[key].toArray();
    return QPointF(arr[0].toDouble(), arr[1].toDouble());
  };

  QPointF A = getPoint("A");
  QPointF B = getPoint("B");
  QPointF C = getPoint("C");
  QPointF D = getPoint("D");
  m_points = {{"A", A}, {"B", B}, {"C", C}, {"D", D}};

  QPen pen(Qt::green, 1);

  scene->addLine(QLineF(A, B), pen);  // A-B
  scene->addLine(QLineF(A, C), pen);  // A-C
  scene->addLine(QLineF(D, C), pen);  // D-C
  scene->addLine(QLineF(D, B), pen);  // D-B

  ui->graphicsViewSpectral->setScene(scene);
  ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  ui->graphicsViewSpectral->setRenderHint(QPainter::Antialiasing);
}

void SpectralData::on_tBtnRedoCrop_clicked() {
  // draw line
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_CV_FIND_CROP));
  QJsonObject obj;
  obj.insert("UUID", m_uuid);
  QJsonObject res = MY_HTTP->post_sync(url, obj);
  drawCropOnImage(m_pixRamanSpectral, res["points"].toObject());
}

void SpectralData::on_tBtnFittingCurve_clicked() {
  fillCalibration();
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_CV_FIND_CURVE));
  QJsonObject obj;
  obj.insert("UUID", m_uuid);
  obj.insert("ID", m_id);
  QString fullPath = ui->lineEditSpectralPicturePath->text();
  QString fileName = QFileInfo(fullPath).fileName();
  obj.insert("file", fileName);

  QMap<int, QString> keyNames;
  keyNames[0] = CALIBRATION_START;
  keyNames[1] = CALIBRATION_4000;
  keyNames[2] = CALIBRATION_2000;
  keyNames[3] = CALIBRATION_0;

  for (auto it = m_pointData.begin(); it != m_pointData.end(); ++it) {
    int id = it.key();
    const PointData &data = it.value();

    QString key = keyNames.value(id, QString::number(id));

    if (data.item) {
      QJsonObject pointObj;
      pointObj.insert("x", data.point.x());
      pointObj.insert("y", data.point.y());
      obj.insert(key, pointObj);
      QString coordinate = QString("(%1, %2)").arg(data.point.x()).arg(data.point.y());
      MY_GLOBAL->set<QString>(key, coordinate);
      SETTING_CONFIG_SET(GROUP_CALIBRATION, key, coordinate);
    } else {
      QMessageBox::warning(this, tr("warning"), tr("%1 is empty!").arg(key));
      return;
    }
  }
  QJsonObject res = MY_HTTP->post_sync(url, obj);
  QJsonArray points = res["points"].toArray();
  int pointCount = res["point_count"].toInt();
  for (const QJsonValue &val : points) {
    QJsonArray arr = val.toArray();
    if (arr.size() != 2) continue;

    int x = arr[0].toInt();
    int y = arr[1].toInt();

    // 用红色小圆点标出
    QGraphicsEllipseItem *dot = new QGraphicsEllipseItem(-1, -1, 2, 2);
    dot->setBrush(Qt::red);
    dot->setPen(Qt::NoPen);
    dot->setPos(x, y);
    ui->graphicsViewSpectral->scene()->addItem(dot);
  }
}

void SpectralData::on_tBtnFindPeak_clicked() {
  QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_CV_FIND_PEAK));
  QJsonObject obj;
  obj.insert("UUID", m_uuid);
  obj.insert("ID", m_id);
  QMap<int, QString> keyNames;
  keyNames[0] = CALIBRATION_START;
  keyNames[1] = CALIBRATION_4000;
  keyNames[2] = CALIBRATION_2000;
  keyNames[3] = CALIBRATION_0;

  for (auto it = m_pointData.begin(); it != m_pointData.end(); ++it) {
    int id = it.key();
    const PointData &data = it.value();

    QString key = keyNames.value(id, QString::number(id));

    if (data.item) {
      QJsonObject pointObj;
      pointObj.insert("x", data.point.x());
      pointObj.insert("y", data.point.y());
      obj.insert(key, pointObj);
    } else {
      QMessageBox::warning(this, "warning", tr("%1 is empty!").arg(key));
      return;
    }
  }
  QJsonObject res = MY_HTTP->post_sync(url, obj);

  if (!res.contains("peak_points") || !res["peak_points"].isArray()) {
    qDebug() << "无效返回：未找到 peak_points";
    return;
  }

  QJsonArray peaks = res["peak_points"].toArray();

  // 绘制蓝色圆圈表示峰点
  for (const auto &val : peaks) {
    if (!val.isArray()) continue;
    QJsonArray point = val.toArray();
    if (point.size() != 2) continue;

    int x = point[0].toInt();
    int y = point[1].toInt();

    // 创建图元
    QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-3, -3, 6, 6);
    ellipse->setPos(x, y);
    ellipse->setBrush(Qt::transparent);
    ellipse->setPen(QPen(Qt::blue, 1.5));
    ui->graphicsViewSpectral->scene()->addItem(ellipse);
  }

  qDebug() << "峰点绘制完成，数量:" << peaks.size();
  QStringList list;
  for (const auto &val : res["peak_points_real"].toArray()) {
    if (!val.isArray()) continue;
    QJsonArray point = val.toArray();
    if (point.size() != 2) continue;

    int x = qRound(point[0].toDouble());
    int y = qRound(point[1].toDouble());
    list.append(QString("(%1,%2)").arg(x).arg(y));
  }
  ui->lineEditPeakAssistance->setText(list.join(";"));
}

void SpectralData::on_tBtnUploadSpectral_clicked() {
  QString path =
      QFileDialog::getOpenFileName(this, tr("Choose Picture"), QString(), "Picture (*.png *.jpg *.jpeg *.bmp *.gif)");

  if (path.isEmpty()) {
    return;
  }
  ui->lineEditSpectralPicturePath->setText(path);
  m_pixRamanSpectral.load(path);
  if (m_pixRamanSpectral.isNull()) {
    QMessageBox::warning(this, tr("Error"), tr("Failed to load image."));
    return;
  }

  // send img
  QString url_post_img =
      QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_UPLOAD_IMG));

  QJsonObject objInfo;
  objInfo.insert("UUID", m_uuid);
  MY_HTTP->postImage(
      url_post_img, path, objInfo,
      [=](QJsonObject obj) {
        qDebug() << "Upload OK:" << obj;
        QFileInfo info(path);
        ui->lineEditSpectralPicturePath->setText(QString("original/%1").arg(info.fileName()));
      },
      [](QString err) { qDebug() << "Upload Error:" << err; });

  QGraphicsScene *scene = new QGraphicsScene(this);
  scene->addPixmap(m_pixRamanSpectral);
  scene->setSceneRect(m_pixRamanSpectral.rect());

  ui->graphicsViewSpectral->setScene(scene);
  ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  ui->graphicsViewSpectral->setRenderHint(QPainter::Antialiasing);
  ui->graphicsViewSpectral->setDragMode(QGraphicsView::ScrollHandDrag);
  emit sendSpectral(m_pixRamanSpectral);
  fillCalibration();
}

void SpectralData::on_tBtnEditPeakMain_clicked() {
  EditContainer edit(EditContainer::TYPE::PEAK);
  edit.setData(ui->lineEditPeakMain->text());
  edit.exec();
  ui->lineEditPeakMain->setText(edit.getData());
}

void SpectralData::on_tBtnEditPeakAssistance_clicked() {
  EditContainer edit(EditContainer::TYPE::PEAK);
  edit.setData(ui->lineEditPeakAssistance->text());
  edit.exec();
  ui->lineEditPeakAssistance->setText(edit.getData());
}
