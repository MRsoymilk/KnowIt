#ifndef SPECTRALSETTING_H
#define SPECTRALSETTING_H

#include <QWidget>

namespace Ui {
class SpectralSetting;
}

class SpectralSetting : public QWidget
{
    Q_OBJECT

public:
    explicit SpectralSetting(QWidget *parent = nullptr);
    ~SpectralSetting();

private:
    Ui::SpectralSetting *ui;
    void init();
};

#endif // SPECTRALSETTING_H
