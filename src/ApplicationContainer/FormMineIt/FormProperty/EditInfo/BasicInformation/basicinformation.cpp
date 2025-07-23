#include "basicinformation.h"

#include <QFileDialog>
#include <QWheelEvent>

#include "../EditContainer/editcontainer.h"
#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_basicinformation.h"

BasicInformation::BasicInformation(QWidget *parent) : QWidget(parent), ui(new Ui::BasicInformation) {
  ui->setupUi(this);
  init();
}

BasicInformation::~BasicInformation() { delete ui; }

void BasicInformation::setID(const QString &id) { m_id = id; }

void BasicInformation::setUUID(const QString &uuid) { m_uuid = uuid; }

void BasicInformation::init() {
  ui->graphicsViewStructure->viewport()->installEventFilter(this);
  ui->tBtnUploadStructure->setObjectName("upload");
  ui->tBtnName_en_Edit->setObjectName("edit");
  ui->tBtnName_zh_Edit->setObjectName("edit");
  ui->tBtnApplicationAreaEdit->setObjectName("edit");
  ui->tBtnCategoryEdit->setObjectName("edit");
  ui->comboBoxState->addItems({"", tr("Liquid"), tr("Powder"), tr("Grain"), tr("Gas")});

  auto map_MajorMinor = MY_GLOBAL->get<QMap<QString, QStringList>>(CHEMICAL_CATEGORY_MAJOR_MINOR);
  ui->comboBoxCategory->addItems(map_MajorMinor.keys());
  QString defaultMajor = ui->comboBoxCategory->currentText();
  ui->comboBoxApplicationArea->clear();
  ui->comboBoxApplicationArea->addItems(map_MajorMinor.value(defaultMajor));
}

bool BasicInformation::handleGraphicsViewStructure(QEvent *event) {
  if (ui->graphicsViewStructure->scene() == nullptr) {
    return true;
  }

  QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
  const double scaleFactor = 1.15;

  if (wheelEvent->angleDelta().y() > 0) {
    ui->graphicsViewStructure->scale(scaleFactor, scaleFactor);  // 放大
  } else {
    ui->graphicsViewStructure->scale(1.0 / scaleFactor, 1.0 / scaleFactor);  // 缩小
  }
  return true;  // 已处理事件
}

bool BasicInformation::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->graphicsViewStructure->viewport() && event->type() == QEvent::Wheel) {
    return handleGraphicsViewStructure(event);
  }
  return QWidget::eventFilter(watched, event);
}

void BasicInformation::setBasicInformation(const QJsonObject &data) {
  QJsonObject BasicInformation = data[BASIC_INFORMATION].toObject();
  ui->lineEditCompoundName_en->setText(BasicInformation.value(COMPOUND_NAME_EN).toString());
  ui->lineEditCompoundName_zh->setText(BasicInformation.value(COMPOUND_NAME_ZH).toString());
  ui->lineEditMolecularFormula->setText(BasicInformation.value(MOLECULAR_FORMULA).toString());
  ui->lineEditMolecularWeight->setText(BasicInformation.value(MOLECULAR_WEIGHT).toString());
  ui->lineEditCASNumber->setText(BasicInformation.value(CAS_NUMBER).toString());
  ui->lineEditCompoundType->setText(BasicInformation.value(COMPOUND_TYPE).toString());
  ui->comboBoxCategory->setCurrentText(BasicInformation.value(CATEGORY).toString());
  ui->comboBoxApplicationArea->setCurrentText(BasicInformation.value(APPLICATION_AREA).toString());
  ui->lineEditStructurePicturePath->setText(BasicInformation.value(STRUCTURE_PICTURE).toString());
  ui->comboBoxState->setCurrentText(BasicInformation.value(STATE).toString());

  QString url_get_img = QString("%1%2/%3/%4")
                            .arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_LOAD_IMG), m_id,
                                 ui->lineEditStructurePicturePath->text());
  MY_HTTP->getImage(
      url_get_img,
      [=](QPixmap pix) {
        QGraphicsScene *scene = new QGraphicsScene(this);
        scene->addPixmap(pix);
        // scene->setSceneRect(pix.rect());
        scene->setSceneRect(QRectF(QPointF(0, 0), pix.size()));

        ui->graphicsViewStructure->setScene(scene);
        // ui->graphicsViewStructure->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
        ui->graphicsViewStructure->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        ui->graphicsViewStructure->setDragMode(QGraphicsView::ScrollHandDrag);
        emit sendStructure(pix);
      },
      [=](QString err) { qDebug() << "Download Error:" << err; });
}

void BasicInformation::on_tBtnName_en_Edit_clicked() {
  EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
  edit.setData(ui->lineEditCompoundName_en->text());
  edit.exec();
  ui->lineEditCompoundName_en->setText(edit.getData());
}

void BasicInformation::on_tBtnName_zh_Edit_clicked() {
  EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
  edit.setData(ui->lineEditCompoundName_zh->text());
  edit.exec();
  ui->lineEditCompoundName_zh->setText(edit.getData());
}

void BasicInformation::on_tBtnUploadStructure_clicked() {
  QString path =
      QFileDialog::getOpenFileName(this, tr("Choose Picture"), QString(), "Picture (*.png *.jpg *.jpeg *.bmp *.gif)");

  if (path.isEmpty()) {
    return;
  }
  ui->lineEditStructurePicturePath->setText(path);
  QPixmap pixStructure(path);
  QString url_post_img =
      QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER), MY_GLOBAL->get<QString>(PATH_UPLOAD_IMG));

  QJsonObject objInfo;
  objInfo.insert("UUID", m_uuid);
  MY_HTTP->postImage(
      url_post_img, path, objInfo,
      [=](QJsonObject obj) {
        qDebug() << "Upload OK:" << obj;
        QFileInfo info(path);
        QString name = info.fileName();
        ui->lineEditStructurePicturePath->setText(QString("original/%1").arg(name));
      },
      [](QString err) { qDebug() << "Upload Error:" << err; });

  QGraphicsScene *scene = new QGraphicsScene(this);
  scene->addPixmap(pixStructure);
  scene->setSceneRect(pixStructure.rect());

  ui->graphicsViewStructure->setScene(scene);
  ui->graphicsViewStructure->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
  ui->graphicsViewStructure->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  ui->graphicsViewStructure->setDragMode(QGraphicsView::ScrollHandDrag);
  emit sendStructure(pixStructure);
}

QJsonObject BasicInformation::getBasicInformation() {
  QJsonObject objBasicInformation;
  objBasicInformation.insert(COMPOUND_NAME_EN, ui->lineEditCompoundName_en->text());
  objBasicInformation.insert(COMPOUND_NAME_ZH, ui->lineEditCompoundName_zh->text());
  objBasicInformation.insert(MOLECULAR_FORMULA, ui->lineEditMolecularFormula->text());
  objBasicInformation.insert(MOLECULAR_WEIGHT, ui->lineEditMolecularWeight->text());
  objBasicInformation.insert(CAS_NUMBER, ui->lineEditCASNumber->text());
  objBasicInformation.insert(COMPOUND_TYPE, ui->lineEditCompoundType->text());
  objBasicInformation.insert(CATEGORY, ui->comboBoxCategory->currentText());
  objBasicInformation.insert(APPLICATION_AREA, ui->comboBoxApplicationArea->currentText());
  objBasicInformation.insert(STRUCTURE_PICTURE, ui->lineEditStructurePicturePath->text());
  objBasicInformation.insert(STATE, ui->comboBoxState->currentText());
  return objBasicInformation;
}

void BasicInformation::on_tBtnCategoryAdd_clicked() {
  EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
  QStringList items;
  for (int i = 0; i < ui->comboBoxCategory->count(); ++i) {
    items << ui->comboBoxCategory->itemText(i);
  }
  edit.setData(items.join(DELIMITER));
  edit.exec();
  ui->comboBoxCategory->clear();
  ui->comboBoxCategory->addItems(edit.getData().split(DELIMITER));
}

void BasicInformation::on_tBtnApplicationAreaAdd_clicked() {
  EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
  QStringList items;
  for (int i = 0; i < ui->comboBoxApplicationArea->count(); ++i) {
    items << ui->comboBoxApplicationArea->itemText(i);
  }
  edit.setData(items.join(DELIMITER));
  edit.exec();
  QString value = edit.getData();
  ui->comboBoxApplicationArea->clear();
  ui->comboBoxApplicationArea->addItems(value.split(DELIMITER));
  QString key = ui->comboBoxCategory->currentText();
  if (!key.isEmpty()) {
    SETTING_CONFIG_SET(CFG_GROUP_CHEMICAL_CATEGORY, key, value);
    auto map_MajorMinor = MY_GLOBAL->get<QMap<QString, QStringList>>(CHEMICAL_CATEGORY_MAJOR_MINOR);
    map_MajorMinor[key] = value.split(DELIMITER);
    MY_GLOBAL->set<QMap<QString, QStringList>>(CHEMICAL_CATEGORY_MAJOR_MINOR, map_MajorMinor);
  }
}

void BasicInformation::on_comboBoxCategory_currentTextChanged(const QString &category) {
  if (category.isEmpty()) {
    return;
  }

  auto map_MajorMinor = MY_GLOBAL->get<QMap<QString, QStringList>>(CHEMICAL_CATEGORY_MAJOR_MINOR);
  ui->comboBoxApplicationArea->clear();
  ui->comboBoxApplicationArea->addItems(map_MajorMinor.value(category));

  QString text = ui->lineEditCategory->text();
  QStringList list = text.split(DELIMITER, Qt::SkipEmptyParts);
  list.removeAll("");
  if (!list.contains(category)) {
    list << category;
    ui->lineEditCategory->setText(list.join(DELIMITER));
  }
}

void BasicInformation::on_comboBoxApplicationArea_currentTextChanged(const QString &area) {
  if (area.isEmpty()) {
    return;
  }

  QString text = ui->lineEditApplicationArea->text();

  QStringList list = text.split(DELIMITER, Qt::SkipEmptyParts);
  list.removeAll("");

  if (!list.contains(area)) {
    list << area;
    ui->lineEditApplicationArea->setText(list.join(DELIMITER));
  }
}
