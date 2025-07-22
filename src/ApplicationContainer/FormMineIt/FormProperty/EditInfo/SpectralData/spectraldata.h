#ifndef SPECTRALDATA_H
#define SPECTRALDATA_H

#include <QWidget>

namespace Ui {
class SpectralData;
}

class SpectralData : public QWidget
{
    Q_OBJECT

public:
    explicit SpectralData(QWidget *parent = nullptr);
    ~SpectralData();

private:
    Ui::SpectralData *ui;
};

#endif // SPECTRALDATA_H
