#include "formproperty.h"

#include <QScrollBar>
#include <QWheelEvent>

#include "DialogAdd/dialogadd.h"
#include "DialogEdit/dialogedit.h"
#include "HoverPopup/hoverpopup.h"
#include "g_define.h"
#include "myglobal.h"
#include "ui_formproperty.h"

FormProperty::FormProperty(QWidget *parent) : QWidget(parent), ui(new Ui::FormProperty) {
  ui->setupUi(this);
  init();
}

FormProperty::~FormProperty() { delete ui; }

void FormProperty::retranslateUI() {
  ui->retranslateUi(this);
  initProperty();
  initPropertySelect();
}

void FormProperty::clearInfo() { m_model->removeRows(0, m_model->rowCount()); }

void FormProperty::showAllProperties() {
  clearInfo();

  if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

  for (const auto &pair : m_propertyCategories) {
    QString category = pair.first;
    const QList<QString> &fields = pair.second;

    if (category == tr("All Properties")) continue;

    QString jsonKey = m_UI2JsonKey.value(category, "");
    if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) continue;

    QJsonObject obj = m_info[jsonKey].toObject();

    for (const QString &field : fields) {
      if (field == tr("ID")) continue;
      QString fieldKey = m_UI2JsonKey.value(field, "");
      appendProperty(field, obj[fieldKey].toString());
    }
  }
}

void FormProperty::onItInfo(const QJsonObject &data) {
  clearInfo();

  m_info = data;

  // 单独处理 ID
  if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

  for (const auto &pair : m_propertyCategories) {
    QString category = pair.first;
    const QList<QString> &fields = pair.second;

    QString jsonKey = m_UI2JsonKey.value(category, "");
    if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) {
      continue;
    }

    QJsonObject obj = m_info[jsonKey].toObject();

    for (const QString &field : fields) {
      if (field == tr("ID")) {
        continue;
      }
      QString fieldKey = m_UI2JsonKey.value(field, "");
      appendProperty(field, obj[fieldKey].toString());
    }
  }
}

void FormProperty::updateCroppedStructurePixmap(int margin) {
  if (m_structurePixmap.isNull()) {
    ui->labelStructure->setText(tr("Structure"));
    return;
  }

  int w = m_structurePixmap.width();
  int h = m_structurePixmap.height();

  // 限制 margin 不越界
  int left = qBound(0, margin, w / 2);
  int top = qBound(0, margin, h / 2);
  int right = w - left;
  int bottom = h - top;

  QRect cropRect(left, top, right - left, bottom - top);
  QPixmap cropped = m_structurePixmap.copy(cropRect);

  QPixmap scaled = cropped.scaled(ui->labelStructure->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
  ui->labelStructure->setPixmap(scaled);
}

void FormProperty::onItStructure(const QPixmap &pix) {
  m_structurePixmap = pix;  // 保存原图
  updateCroppedStructurePixmap(ui->spinBoxCrop->value());
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
  for (QPushButton *btn : m_categoryButtons) {
    ui->hLayBtnProperty->removeWidget(btn);
    btn->deleteLater();
  }
  m_categoryButtons.clear();

  QStringList categoryNames;
  for (const auto &pair : m_propertyCategories) {
    categoryNames << pair.first;
  }

  for (const QString &name : categoryNames) {
    QPushButton *btn = new QPushButton(name, ui->scrollAreaWidgetContents);
    btn->setCheckable(true);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->hLayBtnProperty->addWidget(btn);
    m_categoryButtons << btn;

    connect(btn, &QPushButton::clicked, this, [=]() {
      for (QPushButton *otherBtn : m_categoryButtons) {
        otherBtn->setChecked(otherBtn == btn);
      }

      clearInfo();

      if (name == tr("All Properties")) {
        if (m_info.contains("ID")) appendProperty(tr("ID"), m_info["ID"].toString());

        for (const auto &pair : m_propertyCategories) {
          QString category = pair.first;
          const QList<QString> &fields = pair.second;

          if (category == tr("All Properties")) {
            continue;
          }

          QString jsonKey = m_UI2JsonKey.value(category, "");
          if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) {
            continue;
          }

          QJsonObject obj = m_info[jsonKey].toObject();
          for (const QString &field : fields) {
            if (field == tr("ID")) {
              continue;
            }
            QString fieldKey = m_UI2JsonKey.value(field, "");
            appendProperty(field, obj[fieldKey].toString());
          }
        }
      } else {
        QString jsonKey = m_UI2JsonKey.value(name, "");
        if (jsonKey.isEmpty() || !m_info.contains(jsonKey)) {
          return;
        }

        QJsonObject obj = m_info[jsonKey].toObject();
        for (const QString &field : m_propertyMap[name]) {
          if (field == tr("ID")) {
            continue;
          }
          QString fieldKey = m_UI2JsonKey.value(field, "");
          appendProperty(field, obj[fieldKey].toString());
        }
      }
    });
  }

  if (!m_categoryButtons.isEmpty()) {
    m_categoryButtons.first()->click();
  }
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
              tr("Category"),
              tr("Application Area"),
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
              tr("Produced From"),
              tr("Storage"),
              tr("Derivative State"),
          },
      },
      {
          tr("Annotations And Tags"),
          {
              tr("Data Version"),
              tr("Last Modified"),
          },
      },
      {
          tr("Originate"),
          {
              tr("Resource"),
          },
      },
  };

  m_propertyMap.clear();
  for (const auto &pair : m_propertyCategories) {
    m_propertyMap[pair.first] = pair.second;
  }

  // category -> JSON key
  m_UI2JsonKey = {
      {tr("Annotations And Tags"), ANNOTATIONS_AND_TAGS},
      {tr("Basic Information"), BASIC_INFORMATION},
      {tr("Originate"), ORIGINATE},
      {tr("Spectral Data"), SPECTRAL_DATA},
      {tr("Spectral Acquisition Conditions"), SPECTRAL_ACQUISITION_CONDITIONS},
      {tr("Sample Information"), SAMPLE_INFOMATION},

      {tr("ID"), ID},
      {tr("Compound Name(en)"), COMPOUND_NAME_EN},
      {tr("Compound Name(zh)"), COMPOUND_NAME_ZH},
      {tr("Molecular Formula"), MOLECULAR_FORMULA},
      {tr("Molecular Weight"), MOLECULAR_WEIGHT},
      {tr("CAS Number"), CAS_NUMBER},
      {tr("Compound Type"), COMPOUND_TYPE},
      {tr("Structure Picture"), STRUCTURE_PICTURE},
      {tr("State"), STATE},
      {tr("Peak(Main)"), PEAK_MAIN},
      {tr("Peak(Assistance)"), PEAK_ASSISTANCE},
      {tr("Spectral Picture"), SPECTRAL_PICTURE},
      {tr("Instrument Model"), INSTRUMENT_MODEL},
      {tr("Laser Wavelength"), LASER_WAVELENGTH},
      {tr("Laser Power"), LASER_POWER},
      {tr("Calculus Times"), CALCULUS_TIMES},
      {tr("Measuring Environment"), MEASURING_ENVIRONMENT},
      {tr("Measuring Temperature"), MEASURING_TEMPERATURE},
      {tr("Produced From"), PRODUCED_FROM},
      {tr("Storage"), STORAGE},
      {tr("Derivative State"), DERIVATIVE_STATE},
      {tr("Category"), CATEGORY},
      {tr("Application Area"), APPLICATION_AREA},
      {tr("Data Version"), DATA_VERSION},
      {tr("Last Modified"), LAST_MODIFIED},
      {tr("Resource"), RESOURCE},
  };
}

void FormProperty::init() {
  initProperty();

  m_model = new QStandardItemModel;
  m_model->setHorizontalHeaderLabels({tr("Name"), tr("Value")});
  ui->tableViewProperty->verticalHeader()->setVisible(false);
  ui->tableViewProperty->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->tableViewProperty->setModel(m_model);

  initPropertySelect();

  ui->btnAdd->setObjectName("add");
  ui->btnDelete->setObjectName("delete");
  ui->btnEdit->setObjectName("edit");

  ui->btnAdd->setCheckable(true);
  ui->btnEdit->setCheckable(true);
  ui->scrollArea->viewport()->installEventFilter(this);

  ui->spinBoxCrop->setValue(MY_GLOBAL->get<int>(CFG_PROPERTY_STRUCTURE_CROP));
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

void FormProperty::on_spinBoxCrop_valueChanged(int pixel) {
  MY_GLOBAL->set<int>(CFG_PROPERTY_STRUCTURE_CROP, pixel);
  SETTING_CONFIG_SET(CFG_GROUP_PROPERTY, CFG_PROPERTY_STRUCTURE_CROP, QString::number(pixel));
  updateCroppedStructurePixmap(pixel);
}

void FormProperty::on_tableViewProperty_clicked(const QModelIndex &index) {
  int row = index.row();

  QString name = m_model->data(m_model->index(row, 0)).toString();
  QString value = m_model->data(m_model->index(row, 1)).toString();
  QStringList values = value.split(DELIMITER, Qt::SkipEmptyParts);

  HoverPopup *popup = new HoverPopup();
  popup->setData(name, value);

  QPoint globalPos = QCursor::pos();
  popup->move(globalPos + QPoint(10, 10));
  popup->show();
}
