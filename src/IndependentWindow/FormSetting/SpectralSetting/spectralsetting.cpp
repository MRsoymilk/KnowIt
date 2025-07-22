#include "spectralsetting.h"
#include "g_define.h"
#include "myglobal.h"
#include "ui_spectralsetting.h"

SpectralSetting::SpectralSetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpectralSetting)
{
    ui->setupUi(this);
    init();
}

SpectralSetting::~SpectralSetting()
{
    delete ui;
}

void SpectralSetting::init()
{
    QRegularExpression regex(R"(\(\s*(-?\d+)\s*,\s*(-?\d+)\s*\))");
    {
        QString strPointStart = MY_GLOBAL->get<QString>(CALIBRATION_START);
        QRegularExpressionMatch match = regex.match(strPointStart);
        QPoint point;
        if (match.hasMatch()) {
            int x = match.captured(1).toInt();
            int y = match.captured(2).toInt();
            ui->spinBoxStart_x->setValue(x);
            ui->spinBoxStart_y->setValue(y);
        }
    }

    {
        QString strPoint4000 = MY_GLOBAL->get<QString>(CALIBRATION_4000);
        QRegularExpressionMatch match = regex.match(strPoint4000);
        QPoint point;
        if (match.hasMatch()) {
            int x = match.captured(1).toInt();
            int y = match.captured(2).toInt();
            ui->spinBox4000_x->setValue(x);
            ui->spinBox4000_y->setValue(y);
        }
    }

    {
        QString strPoint2000 = MY_GLOBAL->get<QString>(CALIBRATION_2000);
        QRegularExpressionMatch match = regex.match(strPoint2000);
        QPoint point;
        if (match.hasMatch()) {
            int x = match.captured(1).toInt();
            int y = match.captured(2).toInt();
            ui->spinBox2000_x->setValue(x);
            ui->spinBox2000_y->setValue(y);
        }
    }

    {
        QString strPoint0 = MY_GLOBAL->get<QString>(CALIBRATION_0);
        QRegularExpressionMatch match = regex.match(strPoint0);
        QPoint point;
        if (match.hasMatch()) {
            int x = match.captured(1).toInt();
            int y = match.captured(2).toInt();
            ui->spinBox0_x->setValue(x);
            ui->spinBox0_y->setValue(y);
        }
    }
}
