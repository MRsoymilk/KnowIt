#ifndef PEAKINFO_H
#define PEAKINFO_H

#include <QWidget>

namespace Ui {
class PeakInfo;
}

class PeakInfo : public QWidget
{
    Q_OBJECT

public:
    explicit PeakInfo(QWidget *parent = nullptr,
                      const double &wavelength = 0.0,
                      const double &intensity = 0.0);
    ~PeakInfo();
    double getWavelength();
    double getIntensity();
private slots:
    void on_tBtnDelete_clicked();

private:
    Ui::PeakInfo *ui;
};

#endif // PEAKINFO_H
