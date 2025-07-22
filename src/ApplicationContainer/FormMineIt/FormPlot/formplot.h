#ifndef FORMPLOT_H
#define FORMPLOT_H

#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QWidget>

namespace Ui {
class FormPlot;
}
class MyChartView;

class FormPlot : public QWidget
{
    Q_OBJECT
public:
    enum STACK_INDEX { PLOT, PICTURE };

public:
    explicit FormPlot(QWidget *parent = nullptr);
    ~FormPlot();

public slots:
    void onItPlot(const QJsonObject &data);
    void onItSpectral(const QPixmap &pix);

protected:
    void wheelEvent(QWheelEvent *event) override;
private slots:
    void on_tBtnXaxisInvert_clicked();
    void on_tBtnOriginalImage_clicked();
    void on_tBtnAutoScale_clicked();
    void on_tBtnCrop_clicked();
    void on_tBtnPeakAssistance_clicked();

private:
    void init();
    void initToolButtons();
    void initChart();
    void updateSeries();
    void updateSpectralPixmap();

private:
    Ui::FormPlot *ui;
    QChart *m_chart;
    MyChartView *m_chartView = nullptr;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QMap<int, int> m_rawMainData;
    QMap<int, int> m_rawAssistanceData;
    bool m_isXAxisInverted = false;
    bool m_isOriginalPicture = false;
    bool m_isPeakAssistance = false;
    bool m_isCrop = false;
    bool m_isAutoScale = false;

    QPixmap m_pixSpectral;
    QGraphicsScene *m_sceneSpectral = nullptr;
    QGraphicsPixmapItem *m_itemSpectral = nullptr;
};

#endif // FORMPLOT_H
