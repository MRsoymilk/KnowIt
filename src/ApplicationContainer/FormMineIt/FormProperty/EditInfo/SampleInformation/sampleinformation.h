#ifndef SAMPLEINFORMATION_H
#define SAMPLEINFORMATION_H

#include <QWidget>

namespace Ui {
class SampleInformation;
}

class SampleInformation : public QWidget
{
    Q_OBJECT

public:
    explicit SampleInformation(QWidget *parent = nullptr);
    ~SampleInformation();

private:
    Ui::SampleInformation *ui;
};

#endif // SAMPLEINFORMATION_H
