#ifndef SPECTRALDATA_H
#define SPECTRALDATA_H

#include <QButtonGroup>
#include <QGraphicsEllipseItem>
#include <QLabel>
#include <QWidget>

namespace Ui {
class SpectralData;
}

class SpectralData : public QWidget
{
    Q_OBJECT
public:
    struct PointData
    {
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

private:
    Ui::SpectralData *ui;
    QString m_id, m_uuid;

    // QObject interface
    void init();
    QButtonGroup *toolButtonGroup;

    QMap<int, PointData> m_pointData;

    bool handleGraphicsViewSpectral(QEvent *event);
    QPixmap m_pixRamanSpectral;

    void drawCropOnImage(const QPixmap &pixmap, const QJsonObject &res);
    QMap<QString, QPointF> m_points;

    void initCalibration();
    void fillCalibration();
    bool drawPoint(const QPointF &scenePos,
                   const int &pixelX,
                   const int &pixelY,
                   QAbstractButton *checkedBtn);

public:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void on_tBtnRedoCrop_clicked();
    void on_tBtnFittingCurve_clicked();
    void on_tBtnFindPeak_clicked();
    void on_tBtnUploadSpectral_clicked();
    void on_tBtnEditPeakMain_clicked();
    void on_tBtnEditPeakAssistance_clicked();
};

#endif // SPECTRALDATA_H
