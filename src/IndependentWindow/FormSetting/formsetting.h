#ifndef FORMSETTING_H
#define FORMSETTING_H

#include <QWidget>

namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = nullptr);
    ~FormSetting();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::FormSetting *ui;
};

#endif // FORMSETTING_H
