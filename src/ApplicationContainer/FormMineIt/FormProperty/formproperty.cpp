#include "formproperty.h"

#include <QScrollBar>
#include <QWheelEvent>

#include "DialogAdd/dialogadd.h"
#include "DialogEdit/dialogedit.h"
#include "ui_formproperty.h"
#define BASIC_INFORMATION "Basic Information"
#define SPECTRAL_DATA "Spectral Data"
#define SPECTRAL_ACQUISITION_CONDITIONS "Spectral Acquisition Conditions"
#define SAMPLE_INFOMATION "Sample Information"
#define ANNOTATIONS_AND_TAGS "Annotations and Tags"
#define ORIGINATE "Originate"
FormProperty::FormProperty(QWidget *parent) : QWidget(parent), ui(new Ui::FormProperty) {
  ui->setupUi(this);
  init();
  ui->btnAdd->setCheckable(true);
  ui->btnEdit->setCheckable(true);
  ui->scrollArea->viewport()->installEventFilter(this);
}

FormProperty::~FormProperty() { delete ui; }

void FormProperty::clearInfo() { m_model->removeRows(0, m_model->rowCount()); }

void FormProperty::showAllProperties() {
  m_model->clear();
  m_model->setHorizontalHeaderLabels({"Name", "Value"});

  if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

  for (const auto &pair : m_propertyCategories) {
    QString category = pair.first;
    const QList<QString> &fields = pair.second;

    if (category == tr("All Properties")) continue;

    QString jsonKey = m_categoryToJsonKey.value(category, "");
    if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) continue;

    QJsonObject obj = m_info[jsonKey].toObject();

    for (const QString &field : fields) {
      if (field == tr("ID")) continue;
      appendProperty(field, obj[field].toString());
    }
  }
}

void FormProperty::onItInfo(const QJsonObject &data) {
  clearInfo();

  m_info = data;

  // category -> JSON key 对应
  m_categoryToJsonKey = {
      {tr("Basic Information"), BASIC_INFORMATION},
      {tr("Spectral Data"), SPECTRAL_DATA},
      {tr("Spectral Acquisition Conditions"), SPECTRAL_ACQUISITION_CONDITIONS},
      {tr("Sample Information"), SAMPLE_INFOMATION},
      {tr("Annotations and Tags"), ANNOTATIONS_AND_TAGS},
      {tr("Originate"), ORIGINATE},
  };

  // 单独处理 ID
  if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

  for (const auto &pair : m_propertyCategories) {
    QString category = pair.first;
    const QList<QString> &fields = pair.second;

    QString jsonKey = m_categoryToJsonKey.value(category, "");
    if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) continue;

    QJsonObject obj = m_info[jsonKey].toObject();

    for (const QString &field : fields) {
      if (field == tr("ID")) continue;

      appendProperty(field, obj[field].toString());
    }
  }
}

void FormProperty::onItStructure(const QPixmap &pix) {
  if (pix.isNull()) {
    ui->labelStructure->setText(tr("Structure"));
  } else {
    ui->labelStructure->setPixmap(
        pix.scaled(ui->labelStructure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
}

bool FormProperty::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->scrollArea->viewport() && event->type() == QEvent::Wheel) {
    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);

    int delta = wheelEvent->angleDelta().y();

    QScrollBar *hBar = ui->scrollArea->horizontalScrollBar();
    int newVal = hBar->value() - delta;

    newVal = qMax(hBar->minimum(), qMin(hBar->maximum(), newVal));
    hBar->setValue(newVal);

    return true;
  }
  return QWidget::eventFilter(obj, event);
}

void FormProperty::resizeEvent(QResizeEvent *event) { QWidget::resizeEvent(event); }

void FormProperty::setStructure(const QString &path) {
  QPixmap pixmap(path);
  ui->labelStructure->setPixmap(
      pixmap.scaled(ui->labelStructure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void FormProperty::initPropertySelect() {
  QStringList categoryNames;
  for (const auto &pair : m_propertyCategories) categoryNames << pair.first;

  for (const QString &name : categoryNames) {
    QPushButton *btn = new QPushButton(name, ui->scrollAreaWidgetContents);
    btn->setCheckable(true);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->hLayBtnProperty->addWidget(btn);
    m_categoryButtons << btn;

    connect(btn, &QPushButton::clicked, this, [=]() {
      for (QPushButton *otherBtn : m_categoryButtons) otherBtn->setChecked(otherBtn == btn);

      m_model->clear();
      m_model->setHorizontalHeaderLabels({"Name", "Value"});

      if (name == tr("All Properties")) {
        if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

        for (const auto &pair : m_propertyCategories) {
          QString category = pair.first;
          const QList<QString> &fields = pair.second;

          if (category == tr("All Properties")) continue;

          QString jsonKey = m_categoryToJsonKey.value(category, "");
          if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) continue;

          QJsonObject obj = m_info[jsonKey].toObject();
          for (const QString &field : fields) {
            if (field == tr("ID")) continue;
            appendProperty(field, obj[field].toString());
          }
        }
      } else {
        QString jsonKey = m_categoryToJsonKey.value(name, "");
        if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) return;

        QJsonObject obj = m_info[jsonKey].toObject();
        for (const QString &field : m_propertyMap[name]) {
          if (field == tr("ID")) continue;
          appendProperty(field, obj[field].toString());
        }
      }
    });
  }

  if (!m_categoryButtons.isEmpty()) m_categoryButtons.first()->click();
}

void FormProperty::initProperty() {
  m_propertyCategories = {
      {
          tr("All Properties"),
          {
              tr("ID"),
          },
      },
      {
          tr("Basic Information"),
          {
              tr("Compound Name(en)"),
              tr("Compound Name(zh)"),
              tr("Molecular Formula"),
              tr("Molecular Weight"),
              tr("Compound Type"),
              tr("CAS Number"),
              tr("Structure Picture"),
              tr("State"),
          },
      },
      {
          tr("Spectral Data"),
          {
              tr("Peak(Main)"),
              tr("Peak(Assistance)"),
              tr("Spectral Picture"),
          },
      },
      {
          tr("Spectral Acquisition Conditions"),
          {
              tr("Instrument Model"),
              tr("Laser Wavelength"),
              tr("Laser Power"),
              tr("Calculus Times"),
              tr("Measuring Environment"),
              tr("Measuring Temperature"),
          },
      },
      {
          tr("Sample Information"),
          {
              tr("Origin"),
              tr("Storage"),
              tr("Derivative State"),
          },
      },
      {
          tr("Annotations and Tags"),
          {
              tr("Category"),
              tr("Application Area"),
              tr("Data Version"),
              tr("Last Modified"),
          },
      },
      {
          tr("Originate"),
          {
              tr("Source"),
          },
      },
  };

  for (const auto &pair : m_propertyCategories) {
    m_propertyMap[pair.first] = pair.second;
  }
}

void FormProperty::setBasicInformation(const QJsonObject &data) {
  QJsonObject BasicInformation = data[BASIC_INFORMATION].toObject();
}

void FormProperty::setSpectralData(const QJsonObject &data) {
  QJsonObject objSpectralData = data[SPECTRAL_DATA].toObject();
}

void FormProperty::setSpectralAcquisitionConditions(const QJsonObject &data) {
  QJsonObject objSpectralAcquisitionConditions = data[SPECTRAL_ACQUISITION_CONDITIONS].toObject();
}

void FormProperty::setSampleInformation(const QJsonObject &data) {
  QJsonObject objSampleInformation = data[SAMPLE_INFOMATION].toObject();
}

void FormProperty::setAnnotationsAndTags(const QJsonObject &data) {
  QJsonObject objAnnotationsAndTags = data[ANNOTATIONS_AND_TAGS].toObject();
}

void FormProperty::setOriginate(const QJsonObject &data) { QJsonObject objOriginate = data[ORIGINATE].toObject(); }

void FormProperty::init() {
  initProperty();
  m_model = new QStandardItemModel;
  m_model->setHorizontalHeaderLabels({"Name", "Value"});
  ui->tableViewProperty->verticalHeader()->setVisible(false);
  ui->tableViewProperty->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->tableViewProperty->setModel(m_model);

  initPropertySelect();

  ui->btnAdd->setObjectName("add");
  ui->btnDelete->setObjectName("delete");
  ui->btnEdit->setObjectName("edit");
}

void FormProperty::appendProperty(const QString &name, const QString &value) {
  if (name.isEmpty()) {
    return;
  }
  QList<QStandardItem *> rowItems;
  rowItems << new QStandardItem(name) << new QStandardItem(value);
  m_model->appendRow(rowItems);
}

void FormProperty::on_btnAdd_clicked() {
  ui->btnAdd->setChecked(true);
  DialogAdd *add = new DialogAdd;
  add->exec();
  ui->btnAdd->setChecked(false);
  emit dataAdd();
}

void FormProperty::on_btnEdit_clicked() {
  ui->btnEdit->setChecked(true);
  DialogEdit *edit = new DialogEdit(m_info);
  edit->exec();
  ui->btnEdit->setChecked(false);
}

void FormProperty::on_btnDelete_clicked() { emit selectedDataDelete(); }
