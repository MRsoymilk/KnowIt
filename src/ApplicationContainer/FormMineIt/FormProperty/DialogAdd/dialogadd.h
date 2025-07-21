#ifndef DIALOGADD_H
#define DIALOGADD_H

#include <QDialog>

class EditID;
class EditURL;
class EditInfo;

namespace Ui {
class DialogAdd;
}

class DialogAdd : public QDialog
{
    Q_OBJECT
public:
    enum class EDIT { ID = 0, URL, INFO };

public:
    explicit DialogAdd(QWidget *parent = nullptr);
    ~DialogAdd();
signals:

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();

private:
    void init();

private:
    Ui::DialogAdd *ui;
    EditID *m_editID;
    EditURL *m_editURL;
    EditInfo *m_editInfo;
    QVector<QWidget *> m_editWidgets;
};

#endif // DIALOGADD_H
