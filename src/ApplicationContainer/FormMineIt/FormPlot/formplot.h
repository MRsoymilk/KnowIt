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
private slots:
    void on_tBtnXaxisInvert_clicked();
    void on_tBtnOriginalImage_clicked();
    void on_tBtnAutoScale_clicked();
    void on_tBtnCrop_clicked();

private:
    void init();
    void initToolButtons();
    void initChart();

private:
    Ui::FormPlot *ui;
    QChart *m_chart;
    MyChartView *m_chartView = nullptr;
    QLineSeries *m_series;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;

    QMap<int, int> m_rawData;       // 保存原始 Peak(Main) 数据
    bool m_isXAxisInverted = false; // 当前方向标志
    bool m_isOriginalPicture = false;
    void updateSeries();
    void updateSpectralPixmap();
    QPixmap m_pixSpectral;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // FORMPLOT_H
