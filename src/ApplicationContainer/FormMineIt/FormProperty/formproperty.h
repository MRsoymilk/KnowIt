#ifndef FORMPROPERTY_H
#define FORMPROPERTY_H

#include <QJsonObject>
#include <QPushButton>
#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class FormProperty;
}

class FormProperty : public QWidget
{
    Q_OBJECT

public:
    explicit FormProperty(QWidget *parent = nullptr);
    ~FormProperty();
signals:
    void selectedDataDelete();
    // void selectedDataEdit();
    void dataAdd();

public slots:
    void onItInfo(const QJsonObject &data);
    void onItStructure(const QPixmap &pix);

private slots:
    void on_btnAdd_clicked();
    void on_btnEdit_clicked();
    void on_btnDelete_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void init();
    void appendProperty(const QString &name, const QString &value);
    void initPropertySelect();
    void setStructure(const QString &path);
    void initProperty();
    void setBasicInformation(const QJsonObject &data);
    void setSpectralData(const QJsonObject &data);
    void setSpectralAcquisitionConditions(const QJsonObject &data);
    void setSampleInformation(const QJsonObject &data);
    void setAnnotationsAndTags(const QJsonObject &data);
    void setOriginate(const QJsonObject &data);
    void clearInfo();
    void showAllProperties();

private:
    Ui::FormProperty *ui;
    QStandardItemModel *m_model;

    QList<QPair<QString, QStringList>> m_propertyCategories; // 分类及字段（有顺序）
    QMap<QString, QStringList> m_propertyMap;                // 分类查找（便于筛选）
    QList<QPushButton *> m_categoryButtons;                  // 按钮引用（用于切换状态）
    QJsonObject m_info;
    QMap<QString, QString> m_categoryToJsonKey;
};

#endif // FORMPROPERTY_H
