#ifndef SPECTRALDATA_H
#define SPECTRALDATA_H

#include <QButtonGroup>
#include <QGraphicsEllipseItem>
#include <QLabel>
#include <QWidget>

namespace Ui {
class SpectralData;
}

class SpectralData : public QWidget {
  Q_OBJECT

 public:
  struct PointData {
    QLabel *label = nullptr;
    QPoint point;
    QGraphicsEllipseItem *item = nullptr;
  };

 public:
  explicit SpectralData(QWidget *parent = nullptr);
  ~SpectralData();
  void setID(const QString &id);
  void setUUID(const QString &uuid);
  void setSpectralData(const QJsonObject &data);
  QJsonObject getSpectralData();

 signals:
  void sendSpectral(const QPixmap &pix);

 protected:
  bool eventFilter(QObject *watched, QEvent *event) override;

 private:
  void init();
  void initCalibration();
  void fillCalibration();
  bool handleGraphicsViewSpectral(QEvent *event);
  void drawCropOnImage(const QPixmap &pixmap, const QJsonObject &res);
  bool drawPoint(const QPointF &scenePos, const int &pixelX, const int &pixelY, QAbstractButton *checkedBtn);

 private slots:
  void on_tBtnRedoCrop_clicked();
  void on_tBtnFittingCurve_clicked();
  void on_tBtnFindPeak_clicked();
  void on_tBtnUploadSpectral_clicked();
  void on_tBtnEditPeakMain_clicked();
  void on_tBtnEditPeakAssistance_clicked();

 private:
  Ui::SpectralData *ui;
  QString m_id, m_uuid;
  QButtonGroup *toolButtonGroup;
  QMap<int, PointData> m_pointData;
  QPixmap m_pixRamanSpectral;
  QMap<QString, QPointF> m_points;
};

#endif  // SPECTRALDATA_H
