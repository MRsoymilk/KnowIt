#ifndef EDITINFO_H
#define EDITINFO_H

#include <QButtonGroup>
#include <QGraphicsSimpleTextItem>
#include <QJsonObject>
#include <QLabel>
#include <QWidget>

namespace Ui {
class EditInfo;
}

class EditInfo : public QWidget
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
    explicit EditInfo(const QJsonObject &data = {}, QWidget *parent = nullptr);
    ~EditInfo();
    void setData(const QJsonObject &data);
    QJsonObject getData();

signals:
    void sendStructure(const QPixmap &pix);
    void sendSpectral(const QPixmap &pix);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private slots:
    void on_tBtnName_en_Edit_clicked();
    void on_tBtnName_zh_Edit_clicked();
    void on_tBtnUploadSpectral_clicked();
    void on_tBtnSourceEdit_clicked();
    void on_tBtnRedoCrop_clicked();
    void on_tBtnFindPeak_clicked();
    void on_tBtnFittingCurve_clicked();
    void on_tBtnEditPeakMain_clicked();
    void on_tBtnEditPeakAssistance_clicked();
    void on_tBtnUploadStructure_clicked();

private:
    void init();
    QJsonObject getBasicInformation();
    QJsonObject getSpectralData();
    QJsonObject getSpectralAcquisitionConditions();
    QJsonObject getSampleInformation();
    QJsonObject getAnnotationsAndTags();
    QJsonObject getOriginate();
    void setBasicInformation(const QJsonObject &data);
    void setSpectralData(const QJsonObject &data);
    void setSpectralAcquisitionConditions(const QJsonObject &data);
    void setSampleInformation(const QJsonObject &data);
    void setAnnotationsAndTags(const QJsonObject &data);
    void setOriginate(const QJsonObject &data);

private:
    Ui::EditInfo *ui;
    void drawCropOnImage(const QPixmap &pixmap, const QJsonObject &res);
    QPixmap m_pixRamanSpectral;
    QMap<QString, QPointF> m_points;
    QVector<QPointF> m_clickedPoints;
    QSet<QString> m_selectedPoints;
    QMap<QString, QGraphicsEllipseItem *> m_storePoints;
    QButtonGroup *toolButtonGroup;
    QMap<int, PointData> m_pointData;
    QString m_uuid;
    bool handleGraphicsViewSpectral(QEvent *event);
    bool handleGraphicsViewStructure(QEvent *event);
    QString m_id;
};

#endif // EDITINFO_H
