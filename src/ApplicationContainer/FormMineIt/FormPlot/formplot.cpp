#include "formplot.h"

#include "g_define.h"
#include "mychartview.h"
#include "ui_formplot.h"

FormPlot::FormPlot(QWidget *parent) : QWidget(parent), ui(new Ui::FormPlot) {
  ui->setupUi(this);
  init();
}

FormPlot::~FormPlot() { delete ui; }

void FormPlot::onItPlot(const QJsonObject &data) {
  m_rawMainData.clear();
  m_rawAssistanceData.clear();

  QString peakStr = data[PEAK_MAIN].toString();
  QString peakAssistanceStr = data[PEAK_ASSISTANCE].toString();

  {
    QRegularExpression re(R"(\((\d+),\s*(\d+)\))");
    QRegularExpressionMatchIterator i = re.globalMatch(peakStr);

    while (i.hasNext()) {
      QRegularExpressionMatch match = i.next();
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x >= 0 && x <= 4000 && y >= 0 && y <= 100) m_rawMainData[x] = y;
    }
  }

  {
    QRegularExpression re(R"(\((\d+),\s*(\d+)\))");
    QRegularExpressionMatchIterator i = re.globalMatch(peakAssistanceStr);

    while (i.hasNext()) {
      QRegularExpressionMatch match = i.next();
      int x = match.captured(1).toInt();
      int y = match.captured(2).toInt();
      if (x >= 0 && x <= 4000 && y >= 0 && y <= 100) m_rawAssistanceData[x] = y;
    }
  }

  updateSeries();
}

void FormPlot::onItSpectral(const QPixmap &pix) {
  m_pixSpectral = pix;
  updateSpectralPixmap();
}

void FormPlot::updateSpectralPixmap() {
  if (!m_pixSpectral.isNull()) {
    if (!m_itemSpectral) {
      m_itemSpectral = new QGraphicsPixmapItem();
      m_sceneSpectral->addItem(m_itemSpectral);
    }

    m_itemSpectral->setPixmap(m_pixSpectral);

    m_sceneSpectral->setSceneRect(m_pixSpectral.rect());
  } else {
    m_sceneSpectral->clear();
    m_itemSpectral = nullptr;
    m_sceneSpectral->addText(tr("Spectral"));
  }
}

void FormPlot::updateSeries() {
  QVector<QPointF> pointsMain(4001, QPointF(0, 0));
  QVector<QPointF> pointsAssist(4001, QPointF(0, 0));

  for (int x = 0; x <= 4000; ++x) {
    pointsMain[x].setX(x);
    pointsAssist[x].setX(x);
  }

  if (m_isPeakAssistance) {
    for (auto it = m_rawAssistanceData.constBegin(); it != m_rawAssistanceData.constEnd(); ++it) {
      pointsAssist[it.key()].setY(it.value());
    }
  }

  for (auto it = m_rawMainData.constBegin(); it != m_rawMainData.constEnd(); ++it) {
    pointsMain[it.key()].setY(it.value());
  }

  m_seriesAssistance->replace(pointsAssist);
  m_seriesMain->replace(pointsMain);

  if (m_isXAxisInverted) {
    m_axisX->setRange(4000, 0);
    m_axisX->setReverse(true);
  } else {
    m_axisX->setRange(0, 4000);
    m_axisX->setReverse(false);
  }

  if (m_isAutoScale) {
    qreal maxY = 0;
    for (const auto &pt : pointsMain) {
      if (pt.y() > maxY) maxY = pt.y();
    }
    for (const auto &pt : pointsAssist) {
      if (pt.y() > maxY) maxY = pt.y();
    }
    m_axisY->setRange(0, maxY);
  } else {
    m_axisY->setRange(0, 100);
  }
}

void FormPlot::init() {
  initChart();

  initToolButtons();

  m_sceneSpectral = new QGraphicsScene(this);
  ui->graphicsViewPicture->setScene(m_sceneSpectral);

  ui->graphicsViewPicture->setRenderHint(QPainter::Antialiasing);
  ui->graphicsViewPicture->setRenderHint(QPainter::SmoothPixmapTransform);
  ui->graphicsViewPicture->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
  ui->graphicsViewPicture->setDragMode(QGraphicsView::ScrollHandDrag);
  ui->graphicsViewPicture->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void FormPlot::initChart() {
  m_chart = new QChart;

  m_seriesMain = new QLineSeries();
  m_seriesMain->setColor(Qt::red);

  m_seriesAssistance = new QLineSeries();
  m_seriesAssistance->setColor(Qt::blue);

  m_axisX = new QValueAxis;
  m_axisY = new QValueAxis;

  m_chart->addSeries(m_seriesAssistance);
  m_chart->addSeries(m_seriesMain);

  m_chart->addAxis(m_axisX, Qt::AlignBottom);
  m_chart->addAxis(m_axisY, Qt::AlignLeft);

  m_seriesMain->attachAxis(m_axisX);
  m_seriesMain->attachAxis(m_axisY);
  m_seriesAssistance->attachAxis(m_axisX);
  m_seriesAssistance->attachAxis(m_axisY);

  m_chartView = new MyChartView(m_chart);
  m_chartView->setRenderHint(QPainter::Antialiasing);
  ui->gLayPlot->addWidget(m_chartView);

  m_seriesMain->setName("Main");
  m_seriesAssistance->setName("Assistance");
  m_chart->legend()->setVisible(true);
  m_chart->legend()->setAlignment(Qt::AlignBottom);
}

void FormPlot::initToolButtons() {
  ui->tBtnOriginalImage->setObjectName("OriginalImage");
  ui->tBtnOriginalImage->setIconSize(QSize(24, 24));
  ui->tBtnOriginalImage->setCheckable(true);

  ui->tBtnXaxisInvert->setObjectName("XaxisInvert");
  ui->tBtnXaxisInvert->setIconSize(QSize(24, 24));
  ui->tBtnXaxisInvert->setCheckable(true);

  ui->tBtnCrop->setObjectName("Crop");
  ui->tBtnCrop->setIconSize(QSize(24, 24));
  ui->tBtnCrop->setCheckable(true);

  ui->tBtnAutoScale->setObjectName("AutoScale");
  ui->tBtnAutoScale->setIconSize(QSize(24, 24));
  ui->tBtnAutoScale->setCheckable(true);

  ui->tBtnPeakAssistance->setObjectName("PeakAssistance");
  ui->tBtnPeakAssistance->setIconSize(QSize(24, 24));
  ui->tBtnPeakAssistance->setCheckable(true);
}

void FormPlot::on_tBtnXaxisInvert_clicked() {
  m_isXAxisInverted = !m_isXAxisInverted;
  ui->tBtnXaxisInvert->setChecked(m_isXAxisInverted);
  updateSeries();
}

void FormPlot::on_tBtnOriginalImage_clicked() {
  m_isOriginalPicture = !m_isOriginalPicture;
  updateSpectralPixmap();
  if (m_isOriginalPicture) {
    ui->stackedWidget->setCurrentIndex(STACK_INDEX::PICTURE);
    ui->tBtnAutoScale->setVisible(false);
    ui->tBtnCrop->setVisible(false);
    ui->tBtnPeakAssistance->setVisible(false);
    ui->tBtnXaxisInvert->setVisible(false);
  } else {
    ui->stackedWidget->setCurrentIndex(STACK_INDEX::PLOT);
    ui->tBtnAutoScale->setVisible(true);
    ui->tBtnCrop->setVisible(true);
    ui->tBtnPeakAssistance->setVisible(true);
    ui->tBtnXaxisInvert->setVisible(true);
  }
}

void FormPlot::on_tBtnAutoScale_clicked() {
  m_isAutoScale = !m_isAutoScale;
  updateSeries();
}

void FormPlot::on_tBtnCrop_clicked() {
  m_isCrop = !m_isCrop;
  ui->tBtnCrop->setChecked(m_isCrop);
  if (m_isCrop) {
    m_chartView->setCropEnabled(true);
    m_chartView->recordInitialAxisRange();
  } else {
    m_chartView->setCropEnabled(false);
    m_chartView->backInitialRange();
  }
}

void FormPlot::on_tBtnPeakAssistance_clicked() {
  m_isPeakAssistance = !m_isPeakAssistance;
  updateSeries();
}

void FormPlot::wheelEvent(QWheelEvent *event) {
  if (ui->graphicsViewPicture->underMouse()) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
      ui->graphicsViewPicture->scale(scaleFactor, scaleFactor);
    else
      ui->graphicsViewPicture->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
  } else {
    QWidget::wheelEvent(event);
  }
}

void FormPlot::retranslateUI() {
  ui->retranslateUi(this);
  ui->tBtnOriginalImage->setToolTip(tr("original image"));
  ui->tBtnXaxisInvert->setToolTip(tr("xaxis invert"));
  ui->tBtnCrop->setToolTip(tr("crop"));
  ui->tBtnAutoScale->setToolTip(tr("auto scale"));
  ui->tBtnPeakAssistance->setToolTip(tr("peak assistance"));
}
