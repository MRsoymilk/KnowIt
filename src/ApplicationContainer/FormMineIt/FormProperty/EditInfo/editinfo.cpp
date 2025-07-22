#include "editinfo.h"
#include <QFileDialog>
#include <QJsonArray>
#include <QMainWindow>
#include <QWheelEvent>
#include "EditContainer/editcontainer.h"
#include "g_define.h"
#include "myglobal.h"
#include "myhttp.h"
#include "ui_editinfo.h"
#include <qmessagebox.h>

EditInfo::EditInfo(const QJsonObject &data, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditInfo)
{
    ui->setupUi(this);
    ui->tBtnRedoCrop->setVisible(false);
    init();
    m_uuid = QUuid::createUuid().toString();
    if (!data.isEmpty()) {
        setData(data);
    }
}

EditInfo::~EditInfo()
{
    delete ui;
}

void EditInfo::init()
{
    ui->graphicsViewStructure->viewport()->installEventFilter(this);
    ui->graphicsViewSpectral->viewport()->installEventFilter(this);

    toolButtonGroup = new QButtonGroup(this);
    toolButtonGroup->setExclusive(true);
    ui->tBtnPointStart->setCheckable(true);
    ui->tBtnPoint4000->setCheckable(true);
    ui->tBtnPoint2000->setCheckable(true);
    ui->tBtnPoint0->setCheckable(true);

    toolButtonGroup->addButton(ui->tBtnPointStart, 0);
    toolButtonGroup->addButton(ui->tBtnPoint4000, 1);
    toolButtonGroup->addButton(ui->tBtnPoint2000, 2);
    toolButtonGroup->addButton(ui->tBtnPoint0, 3);
    ui->tBtnPointStart->setChecked(true);

    m_pointData[0] = {ui->labelPointStart, QPoint(), nullptr};
    m_pointData[1] = {ui->labelPoint4000, QPoint(), nullptr};
    m_pointData[2] = {ui->labelPoint2000, QPoint(), nullptr};
    m_pointData[3] = {ui->labelPoint0, QPoint(), nullptr};

    ui->tBtnEditPeakAssistance->setObjectName("edit");
    ui->tBtnEditPeakMain->setObjectName("edit");
    ui->tBtnName_en_Edit->setObjectName("edit");
    ui->tBtnName_zh_Edit->setObjectName("edit");
    ui->tBtnSourceEdit->setObjectName("edit");
    ui->tBtnUploadSpectral->setObjectName("upload");
    ui->tBtnUploadStructure->setObjectName("upload");
}

bool EditInfo::handleGraphicsViewSpectral(QEvent *event)
{
    if (ui->graphicsViewSpectral->scene() == nullptr) {
        return true;
    }

    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
    const double scaleFactor = 1.15;

    if (wheelEvent->angleDelta().y() > 0) {
        ui->graphicsViewSpectral->scale(scaleFactor, scaleFactor); // 放大
    } else {
        ui->graphicsViewSpectral->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // 缩小
    }
    return true; // 已处理事件
}

bool EditInfo::handleGraphicsViewStructure(QEvent *event)
{
    if (ui->graphicsViewStructure->scene() == nullptr) {
        return true;
    }

    QWheelEvent *wheelEvent = static_cast<QWheelEvent *>(event);
    const double scaleFactor = 1.15;

    if (wheelEvent->angleDelta().y() > 0) {
        ui->graphicsViewStructure->scale(scaleFactor, scaleFactor); // 放大
    } else {
        ui->graphicsViewStructure->scale(1.0 / scaleFactor, 1.0 / scaleFactor); // 缩小
    }
    return true; // 已处理事件
}

bool EditInfo::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->graphicsViewSpectral->viewport() && event->type() == QEvent::Wheel) {
        return handleGraphicsViewSpectral(event);
    }

    if (obj == ui->graphicsViewStructure->viewport() && event->type() == QEvent::Wheel) {
        return handleGraphicsViewStructure(event);
    }

    if (obj == ui->graphicsViewSpectral->viewport() && event->type() == QEvent::MouseButtonPress) {
        if (ui->graphicsViewSpectral->scene() == nullptr) {
            return true;
        }

        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

        if (mouseEvent->button() == Qt::RightButton) {
            QPointF scenePos = ui->graphicsViewSpectral->mapToScene(mouseEvent->pos());
            int pixelX = static_cast<int>(scenePos.x());
            int pixelY = static_cast<int>(scenePos.y());
            QString coordText = QString("X: %1, Y: %2").arg(pixelX).arg(pixelY);

            QAbstractButton *checkedBtn = toolButtonGroup->checkedButton();
            if (!checkedBtn) {
                return true;
            }

            int id = toolButtonGroup->id(checkedBtn);
            if (!m_pointData.contains(id)) {
                qWarning() << "Unknown tool button ID:" << id;
                return true;
            }

            PointData &data = m_pointData[id];

            if (data.item) {
                // 已经画过，删除图元
                ui->graphicsViewSpectral->scene()->removeItem(data.item);
                data.item = nullptr;
                data.label->clear();

                // 重新选中当前按钮
                checkedBtn->setChecked(true);
            } else {
                // 绘制新点
                QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-0.5, -0.5, 1, 1);
                ellipse->setPos(scenePos);
                ellipse->setBrush(Qt::red);
                ellipse->setPen(Qt::NoPen);
                ui->graphicsViewSpectral->scene()->addItem(ellipse);

                // 保存数据
                data.item = ellipse;
                data.point = QPoint(pixelX, pixelY);
                data.label->setText(coordText);

                // 当前按钮取消选中
                checkedBtn->setChecked(false);

                // 设置下一个按钮为选中状态
                QAbstractButton *nextBtn = toolButtonGroup->button(id + 1);
                if (nextBtn) {
                    nextBtn->setChecked(true);
                }
            }

            return true;
        }
    }

    return QWidget::eventFilter(obj, event); // 其他事件交由默认处理
}

void EditInfo::setData(const QJsonObject &data)
{
    m_id = data["ID"].toString();
    setBasicInformation(data);
    setSpectralData(data);
    setSpectralAcquisitionConditions(data);
    setSampleInformation(data);
    setAnnotationsAndTags(data);
    setOriginate(data);
}

QJsonObject EditInfo::getData()
{
    QJsonObject obj;
    obj.insert("uuid", m_uuid);
    obj.insert(BASIC_INFORMATION, getBasicInformation());
    obj.insert(SPECTRAL_DATA, getSpectralData());
    obj.insert(SPECTRAL_ACQUISITION_CONDITIONS, getSpectralAcquisitionConditions());
    obj.insert(SAMPLE_INFOMATION, getSampleInformation());
    obj.insert(ANNOTATIONS_AND_TAGS, getAnnotationsAndTags());
    obj.insert(ORIGINATE, getOriginate());
    return obj;
}

void EditInfo::setBasicInformation(const QJsonObject &data)
{
    QJsonObject BasicInformation = data[BASIC_INFORMATION].toObject();
    ui->lineEditCompoundName_en->setText(BasicInformation.value(COMPOUND_NAME_EN).toString());
    ui->lineEditCompoundName_zh->setText(BasicInformation.value(COMPOUND_NAME_ZH).toString());
    ui->lineEditMolecularFormula->setText(BasicInformation.value(MOLECULAR_FORMULA).toString());
    ui->lineEditMolecularWeight->setText(BasicInformation.value(MOLECULAR_WEIGHT).toString());
    ui->lineEditCASNumber->setText(BasicInformation.value(CAS_NUMBER).toString());
    ui->lineEditCompoundType->setText(BasicInformation.value(COMPOUND_TYPE).toString());
    ui->lineEditStructurePicturePath->setText(BasicInformation.value(STRUCTURE_PICTURE).toString());
    ui->lineEditState->setText(BasicInformation.value(STATE).toString());

    QString url_get_img = QString("%1%2/%3/%4")
                              .arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                   MY_GLOBAL->get<QString>(PATH_LOAD_IMG),
                                   m_id,
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
            ui->graphicsViewStructure->setRenderHints(QPainter::Antialiasing
                                                      | QPainter::SmoothPixmapTransform);
            ui->graphicsViewStructure->setDragMode(QGraphicsView::ScrollHandDrag);
            emit sendStructure(pix);
        },
        [=](QString err) { qDebug() << "Download Error:" << err; });
}

void EditInfo::setSpectralData(const QJsonObject &data)
{
    QJsonObject objSpectralData = data[SPECTRAL_DATA].toObject();
    ui->lineEditPeakMain->setText(objSpectralData.value(PEAK_MAIN).toString());
    ui->lineEditSpectralPicturePath->setText(objSpectralData.value(PLOT_PATH).toString());

    QString url_get_img = QString("%1%2/%3/%4")
                              .arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                   MY_GLOBAL->get<QString>(PATH_LOAD_IMG),
                                   m_id,
                                   ui->lineEditSpectralPicturePath->text());
    MY_HTTP->getImage(
        url_get_img,
        [=](QPixmap pix) {
            QGraphicsScene *scene = new QGraphicsScene(this);
            scene->addPixmap(pix);
            scene->setSceneRect(pix.rect());

            ui->graphicsViewSpectral->setScene(scene);
            // ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
            ui->graphicsViewSpectral->setRenderHints(QPainter::Antialiasing
                                                     | QPainter::SmoothPixmapTransform);
            ui->graphicsViewSpectral->setDragMode(QGraphicsView::ScrollHandDrag);
            emit sendSpectral(pix);
        },
        [=](QString err) { qDebug() << "Download Error:" << err; });
}

void EditInfo::setSpectralAcquisitionConditions(const QJsonObject &data)
{
    QJsonObject objSpectralAcquisitionConditions = data[SPECTRAL_ACQUISITION_CONDITIONS].toObject();
    ui->lineEditInstrumentModel->setText(
        objSpectralAcquisitionConditions.value(INSTRUMENT_MODEL).toString());
    ui->lineEditLaserWavelength->setText(
        objSpectralAcquisitionConditions.value(LASER_WAVELENGTH).toString());
    ui->lineEditLaserPower->setText(objSpectralAcquisitionConditions.value(LASER_POWER).toString());
    ui->lineEditCalculusTimes->setText(
        objSpectralAcquisitionConditions.value(CALCULUS_TIMES).toString());
    ui->lineEditMeasuringEnvironment->setText(
        objSpectralAcquisitionConditions.value(MEASURING_ENVIRONMENT).toString());
    ui->lineEditMeasuringTemperature->setText(
        objSpectralAcquisitionConditions.value(MEASURING_TEMPERATURE).toString());
}

void EditInfo::setSampleInformation(const QJsonObject &data)
{
    QJsonObject objSampleInformation = data[SAMPLE_INFOMATION].toObject();
    ui->lineEditOrigin->setText(objSampleInformation.value(ORIGIN).toString());
    ui->lineEditStorage->setText(objSampleInformation.value(STORAGE).toString());
    ui->lineEditDerivativeState->setText(objSampleInformation.value(DERIVATIVE_STATE).toString());
}

void EditInfo::setAnnotationsAndTags(const QJsonObject &data)
{
    QJsonObject objAnnotationsAndTags = data[ANNOTATIONS_AND_TAGS].toObject();
    ui->lineEditCategory->setText(objAnnotationsAndTags.value(CATEGORY).toString());
    ui->lineEditApplicationArea->setText(objAnnotationsAndTags.value(APPLICATION_AREA).toString());
    ui->lineEditDataVersion->setText(objAnnotationsAndTags.value(DATA_VERSION).toString());
    ui->lineEditLastModified->setText(objAnnotationsAndTags.value(LAST_MODIFIED).toString());
}

void EditInfo::setOriginate(const QJsonObject &data)
{
    QJsonObject objOriginate = data[ORIGINATE].toObject();
    ui->lineEditSource->setText(objOriginate.value(SOURCE).toString());
}

QJsonObject EditInfo::getBasicInformation()
{
    QJsonObject objBasicInformation;
    objBasicInformation.insert(COMPOUND_NAME_EN, ui->lineEditCompoundName_en->text());
    objBasicInformation.insert(COMPOUND_NAME_ZH, ui->lineEditCompoundName_zh->text());
    objBasicInformation.insert(MOLECULAR_FORMULA, ui->lineEditMolecularFormula->text());
    objBasicInformation.insert(MOLECULAR_WEIGHT, ui->lineEditMolecularWeight->text());
    objBasicInformation.insert(CAS_NUMBER, ui->lineEditCASNumber->text());
    objBasicInformation.insert(COMPOUND_TYPE, ui->lineEditCompoundType->text());
    objBasicInformation.insert(STRUCTURE_PICTURE, ui->lineEditStructurePicturePath->text());
    objBasicInformation.insert(STATE, ui->lineEditState->text());
    return objBasicInformation;
}

QJsonObject EditInfo::getSpectralData()
{
    QJsonObject objSpectralData;
    objSpectralData.insert(PEAK_MAIN, ui->lineEditPeakMain->text());
    objSpectralData.insert(PEAK_ASSISTANCE, ui->lineEditPeakAssistance->text());
    objSpectralData.insert(PLOT_PATH, ui->lineEditSpectralPicturePath->text());
    return objSpectralData;
}

QJsonObject EditInfo::getSpectralAcquisitionConditions()
{
    QJsonObject objSpectralAcquisitionConditions;
    objSpectralAcquisitionConditions.insert(INSTRUMENT_MODEL, ui->lineEditInstrumentModel->text());
    objSpectralAcquisitionConditions.insert(LASER_WAVELENGTH, ui->lineEditLaserWavelength->text());
    objSpectralAcquisitionConditions.insert(LASER_POWER, ui->lineEditLaserPower->text());
    objSpectralAcquisitionConditions.insert(CALCULUS_TIMES, ui->lineEditCalculusTimes->text());
    objSpectralAcquisitionConditions.insert(MEASURING_ENVIRONMENT,
                                            ui->lineEditMeasuringEnvironment->text());
    objSpectralAcquisitionConditions.insert(MEASURING_TEMPERATURE,
                                            ui->lineEditMeasuringTemperature->text());
    return objSpectralAcquisitionConditions;
}

QJsonObject EditInfo::getSampleInformation()
{
    QJsonObject objSampleInformation;
    objSampleInformation.insert(ORIGIN, ui->lineEditOrigin->text());
    objSampleInformation.insert(STORAGE, ui->lineEditStorage->text());
    objSampleInformation.insert(DERIVATIVE_STATE, ui->lineEditDerivativeState->text());
    return objSampleInformation;
}

QJsonObject EditInfo::getAnnotationsAndTags()
{
    QJsonObject objAnnotationsAndTags;
    objAnnotationsAndTags.insert(CATEGORY, ui->lineEditCategory->text());
    objAnnotationsAndTags.insert(APPLICATION_AREA, ui->lineEditApplicationArea->text());
    objAnnotationsAndTags.insert(DATA_VERSION, ui->lineEditDataVersion->text());
    objAnnotationsAndTags.insert(LAST_MODIFIED, ui->lineEditLastModified->text());
    return objAnnotationsAndTags;
}

QJsonObject EditInfo::getOriginate()
{
    QJsonObject objOriginate;
    objOriginate.insert(SOURCE, ui->lineEditSource->text());
    return objOriginate;
}

void EditInfo::on_tBtnName_en_Edit_clicked()
{
    EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
    edit.setData(ui->lineEditCompoundName_en->text());
    edit.exec();
    ui->lineEditCompoundName_en->setText(edit.getData());
}

void EditInfo::on_tBtnName_zh_Edit_clicked()
{
    EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
    edit.setData(ui->lineEditCompoundName_zh->text());
    edit.exec();
    ui->lineEditCompoundName_zh->setText(edit.getData());
}

void EditInfo::drawCropOnImage(const QPixmap &pixmap, const QJsonObject &res)
{
    if (ui->graphicsViewSpectral->scene()) {
        delete ui->graphicsViewSpectral->scene();
        ui->graphicsViewSpectral->setScene(nullptr);
    }
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(pixmap);

    auto getPoint = [&](const QString &key) -> QPointF {
        QJsonArray arr = res[key].toArray();
        return QPointF(arr[0].toDouble(), arr[1].toDouble());
    };

    QPointF A = getPoint("A");
    QPointF B = getPoint("B");
    QPointF C = getPoint("C");
    QPointF D = getPoint("D");
    m_points = {{"A", A}, {"B", B}, {"C", C}, {"D", D}};

    QPen pen(Qt::green, 1);

    scene->addLine(QLineF(A, B), pen); // A-B
    scene->addLine(QLineF(A, C), pen); // A-C
    scene->addLine(QLineF(D, C), pen); // D-C
    scene->addLine(QLineF(D, B), pen); // D-B

    ui->graphicsViewSpectral->setScene(scene);
    ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsViewSpectral->setRenderHint(QPainter::Antialiasing);
}

void EditInfo::on_tBtnSourceEdit_clicked()
{
    EditContainer edit(EditContainer::TYPE::SINGLE_LINE);
    edit.setData(ui->lineEditSource->text());
    edit.exec();
    ui->lineEditSource->setText(edit.getData());
}

void EditInfo::on_tBtnRedoCrop_clicked()
{
    // draw line
    QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                      MY_GLOBAL->get<QString>(PATH_CV_FIND_CROP));
    QJsonObject obj;
    obj.insert("UUID", m_uuid);
    QJsonObject res = MY_HTTP->post_sync(url, obj);
    drawCropOnImage(m_pixRamanSpectral, res["points"].toObject());
}

void EditInfo::on_tBtnFindPeak_clicked()
{
    QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                      MY_GLOBAL->get<QString>(PATH_CV_FIND_PEAK));
    QJsonObject obj;
    obj.insert("UUID", m_uuid);
    QMap<int, QString> keyNames;
    keyNames[0] = "start";
    keyNames[1] = "4000";
    keyNames[2] = "2000";
    keyNames[3] = "0";

    for (auto it = m_pointData.begin(); it != m_pointData.end(); ++it) {
        int id = it.key();
        const PointData &data = it.value();

        QString key = keyNames.value(id, QString::number(id));

        if (data.item) {
            QJsonObject pointObj;
            pointObj.insert("x", data.point.x());
            pointObj.insert("y", data.point.y());
            obj.insert(key, pointObj);
        } else {
            QMessageBox::warning(this, "warning", tr("%1 is empty!").arg(key));
            return;
        }
    }
    QJsonObject res = MY_HTTP->post_sync(url, obj);

    if (!res.contains("peak_points") || !res["peak_points"].isArray()) {
        qDebug() << "无效返回：未找到 peak_points";
        return;
    }

    QJsonArray peaks = res["peak_points"].toArray();

    // 绘制蓝色圆圈表示峰点
    for (const auto &val : peaks) {
        if (!val.isArray())
            continue;
        QJsonArray point = val.toArray();
        if (point.size() != 2)
            continue;

        int x = point[0].toInt();
        int y = point[1].toInt();

        // 创建图元
        QGraphicsEllipseItem *ellipse = new QGraphicsEllipseItem(-3, -3, 6, 6);
        ellipse->setPos(x, y);
        ellipse->setBrush(Qt::transparent);
        ellipse->setPen(QPen(Qt::blue, 1.5));
        ui->graphicsViewSpectral->scene()->addItem(ellipse);
    }

    qDebug() << "峰点绘制完成，数量:" << peaks.size();
    QStringList list;
    for (const auto &val : res["peak_points_real"].toArray()) {
        if (!val.isArray())
            continue;
        QJsonArray point = val.toArray();
        if (point.size() != 2)
            continue;

        int x = qRound(point[0].toDouble());
        int y = qRound(point[1].toDouble());
        list.append(QString("(%1,%2)").arg(x).arg(y));
    }
    ui->lineEditPeakAssistance->setText(list.join(";"));
}

void EditInfo::on_tBtnFittingCurve_clicked()
{
    QString url = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                      MY_GLOBAL->get<QString>(PATH_CV_FIND_CURVE));
    QJsonObject obj;
    obj.insert("UUID", m_uuid);
    QString fullPath = ui->lineEditSpectralPicturePath->text();
    QString fileName = QFileInfo(fullPath).fileName();
    obj.insert("file", fileName);

    QMap<int, QString> keyNames;
    keyNames[0] = "start";
    keyNames[1] = "4000";
    keyNames[2] = "2000";
    keyNames[3] = "0";

    for (auto it = m_pointData.begin(); it != m_pointData.end(); ++it) {
        int id = it.key();
        const PointData &data = it.value();

        QString key = keyNames.value(id, QString::number(id));

        if (data.item) {
            QJsonObject pointObj;
            pointObj.insert("x", data.point.x());
            pointObj.insert("y", data.point.y());
            obj.insert(key, pointObj);
        } else {
            QMessageBox::warning(this, "warning", tr("%1 is empty!").arg(key));
            return;
        }
    }
    QJsonObject res = MY_HTTP->post_sync(url, obj);
    QJsonArray points = res["points"].toArray();
    int pointCount = res["point_count"].toInt();
    for (const QJsonValue &val : points) {
        QJsonArray arr = val.toArray();
        if (arr.size() != 2)
            continue;

        int x = arr[0].toInt();
        int y = arr[1].toInt();

        // 用红色小圆点标出
        QGraphicsEllipseItem *dot = new QGraphicsEllipseItem(-1, -1, 2, 2);
        dot->setBrush(Qt::red);
        dot->setPen(Qt::NoPen);
        dot->setPos(x, y);
        ui->graphicsViewSpectral->scene()->addItem(dot);
    }
}

void EditInfo::on_tBtnEditPeakMain_clicked()
{
    EditContainer edit(EditContainer::TYPE::PEAK);
    edit.setData(ui->lineEditPeakMain->text());
    edit.exec();
    ui->lineEditPeakMain->setText(edit.getData());
}

void EditInfo::on_tBtnEditPeakAssistance_clicked()
{
    EditContainer edit(EditContainer::TYPE::PEAK);
    edit.setData(ui->lineEditPeakAssistance->text());
    edit.exec();
    ui->lineEditPeakAssistance->setText(edit.getData());
}

void EditInfo::on_tBtnUploadStructure_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Choose Picture"),
                                                QString(),
                                                tr("Picture (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (path.isEmpty()) {
        return;
    }
    ui->lineEditStructurePicturePath->setText(path);
    QPixmap pixStructure(path);
    QString url_post_img = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                               MY_GLOBAL->get<QString>(PATH_UPLOAD_IMG));

    QJsonObject objInfo;
    objInfo.insert("UUID", m_uuid);
    MY_HTTP->postImage(
        url_post_img,
        path,
        objInfo,
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
    ui->graphicsViewStructure->setRenderHints(QPainter::Antialiasing
                                              | QPainter::SmoothPixmapTransform);
    ui->graphicsViewStructure->setDragMode(QGraphicsView::ScrollHandDrag);
    emit sendStructure(pixStructure);
}

void EditInfo::on_tBtnUploadSpectral_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Choose Picture"),
                                                QString(),
                                                tr("Picture (*.png *.jpg *.jpeg *.bmp *.gif)"));

    if (path.isEmpty()) {
        return;
    }
    ui->lineEditSpectralPicturePath->setText(path);
    m_pixRamanSpectral.load(path);
    if (m_pixRamanSpectral.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image."));
        return;
    }

    // send img
    QString url_post_img = QString("%1%2").arg(MY_GLOBAL->get<QString>(URL_SERVER),
                                               MY_GLOBAL->get<QString>(PATH_UPLOAD_IMG));

    QJsonObject objInfo;
    objInfo.insert("UUID", m_uuid);
    MY_HTTP->postImage(
        url_post_img,
        path,
        objInfo,
        [=](QJsonObject obj) {
            qDebug() << "Upload OK:" << obj;
            QFileInfo info(path);
            ui->lineEditSpectralPicturePath->setText(QString("original/%1").arg(info.fileName()));
        },
        [](QString err) { qDebug() << "Upload Error:" << err; });

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(m_pixRamanSpectral);
    scene->setSceneRect(m_pixRamanSpectral.rect());

    ui->graphicsViewSpectral->setScene(scene);
    ui->graphicsViewSpectral->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsViewSpectral->setRenderHint(QPainter::Antialiasing);
    ui->graphicsViewSpectral->setDragMode(QGraphicsView::ScrollHandDrag);
    emit sendSpectral(m_pixRamanSpectral);
}
