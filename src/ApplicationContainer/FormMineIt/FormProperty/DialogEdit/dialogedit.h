#ifndef DIALOGEDIT_H
#define DIALOGEDIT_H

#include <QDialog>
#include <QJsonObject>

class EditInfo;
namespace Ui {
class DialogEdit;
}

class DialogEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEdit(const QJsonObject &data = {}, QWidget *parent = nullptr);
    ~DialogEdit();

private:
    void init(const QJsonObject &data = {});

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::DialogEdit *ui;
    EditInfo *m_editInfo;
};

#endif // DIALOGEDIT_H
