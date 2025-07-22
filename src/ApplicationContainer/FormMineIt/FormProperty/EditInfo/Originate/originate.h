#ifndef ORIGINATE_H
#define ORIGINATE_H

#include <QWidget>

namespace Ui {
class Originate;
}

class Originate : public QWidget
{
    Q_OBJECT

public:
    explicit Originate(QWidget *parent = nullptr);
    ~Originate();

private:
    Ui::Originate *ui;
};

#endif // ORIGINATE_H
