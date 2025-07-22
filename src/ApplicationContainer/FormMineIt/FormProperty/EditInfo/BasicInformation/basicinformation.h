#ifndef BASICINFORMATION_H
#define BASICINFORMATION_H

#include <QWidget>

namespace Ui {
class BasicInformation;
}

class BasicInformation : public QWidget
{
    Q_OBJECT

public:
    explicit BasicInformation(QWidget *parent = nullptr);
    ~BasicInformation();

private:
    Ui::BasicInformation *ui;
};

#endif // BASICINFORMATION_H
