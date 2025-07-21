#include "formplot.h"
#include "mychartview.h"
#include "ui_formplot.h"

FormPlot::FormPlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormPlot)
{
    ui->setupUi(this);
    init();
}

FormPlot::~FormPlot()
{
    delete ui;
}
void FormPlot::onItPlot(const QJsonObject &data)
{
    m_rawData.clear();
    QString peakStr = data["Peak(Main)"].toString();

    QRegularExpression re(R"(\((\d+),\s*(\d+)\))");
    QRegularExpressionMatchIterator i = re.globalMatch(peakStr);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int x = match.captured(1).toInt();
        int y = match.captured(2).toInt();
        if (x >= 0 && x <= 4000 && y >= 0 && y <= 100)
            m_rawData[x] = y;
    }

    updateSeries();
}

void FormPlot::onItSpectral(const QPixmap &pix)
{
    m_pixSpectral = pix;
    updateSpectralPixmap();
}

void FormPlot::updateSpectralPixmap()
{
    if (!m_pixSpectral.isNull()) {
        ui->labelSpectral->setPixmap(m_pixSpectral.scaled(ui->labelSpectral->size(),
                                                          Qt::KeepAspectRatio,
                                                          Qt::SmoothTransformation));
    } else {
        ui->labelSpectral->setText(tr("Spectral"));
    }
}

void FormPlot::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateSpectralPixmap();
}

void FormPlot::updateSeries()
{
    QVector<QPointF> points;
    points.reserve(4001);

    for (int x = 0; x <= 4000; ++x) {
        int y = m_rawData.value(x, 0);
        points.append(QPointF(x, y));
    }

    m_series->replace(points);

    if (m_isXAxisInverted) {
        m_axisX->setRange(4000, 0);
        m_axisX->setReverse(true);
    } else {
        m_axisX->setRange(0, 4000);
        m_axisX->setReverse(false);
    }
    m_axisY->setRange(0, 100);
}

void FormPlot::init()
{
    initChart();

    initToolButtons();
}

void FormPlot::initChart()
{
    m_chart = new QChart;

    m_series = new QLineSeries();
    m_axisX = new QValueAxis;
    m_axisY = new QValueAxis;

    m_chart->addSeries(m_series);
    m_chart->addAxis(m_axisX, Qt::AlignBottom);
    m_chart->addAxis(m_axisY, Qt::AlignLeft);

    m_chart->legend()->setVisible(false);

    m_series->attachAxis(m_axisX);
    m_series->attachAxis(m_axisY);

    qDebug() << "m_axisX:" << m_axisX;
    qDebug() << "chart axisX:" << m_chart->axes(Qt::Horizontal).first();

    m_chartView = new MyChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    ui->gLayPlot->addWidget(m_chartView);
}

void FormPlot::initToolButtons()
{
    ui->tBtnOriginalImage->setToolTip(tr("original image"));
    ui->tBtnOriginalImage->setObjectName("OriginalImage");
    ui->tBtnOriginalImage->setIcon(QIcon(":/res/icons/black/original_image.png"));
    ui->tBtnOriginalImage->setIconSize(QSize(24, 24));
    ui->tBtnOriginalImage->setCheckable(true);
    // ui->tBtnOriginalImage->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->tBtnXaxisInvert->setToolTip(tr("xaxis invert"));
    ui->tBtnXaxisInvert->setObjectName("XaxisInvert");
    ui->tBtnXaxisInvert->setIcon(QIcon(":/res/icons/black/xaxis_invert.png"));
    ui->tBtnXaxisInvert->setIconSize(QSize(24, 24));
    ui->tBtnXaxisInvert->setCheckable(true);
    // ui->tBtnXaxisInvert->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->tBtnCrop->setToolTip(tr("crop"));
    ui->tBtnCrop->setObjectName("Crop");
    ui->tBtnCrop->setIcon(QIcon(":/res/icons/black/crop.png"));
    ui->tBtnCrop->setIconSize(QSize(24, 24));
    ui->tBtnCrop->setCheckable(true);
    // ui->tBtnCrop->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->tBtnAutoScale->setToolTip(tr("auto scale"));
    ui->tBtnAutoScale->setObjectName("AutoScale");
    ui->tBtnAutoScale->setIcon(QIcon(":/res/icons/black/auto_scale.png"));
    ui->tBtnAutoScale->setIconSize(QSize(24, 24));
    ui->tBtnAutoScale->setCheckable(true);
    // ui->tBtnAutoScale->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void FormPlot::on_tBtnXaxisInvert_clicked()
{
    m_isXAxisInverted = !m_isXAxisInverted;
    ui->tBtnXaxisInvert->setChecked(m_isXAxisInverted);
    updateSeries();
}

void FormPlot::on_tBtnOriginalImage_clicked()
{
    m_isOriginalPicture = !m_isOriginalPicture;
    if (m_isOriginalPicture) {
        ui->stackedWidget->setCurrentIndex(STACK_INDEX::PICTURE);
    } else {
        ui->stackedWidget->setCurrentIndex(STACK_INDEX::PLOT);
    }
}

void FormPlot::on_tBtnAutoScale_clicked() {}

void FormPlot::on_tBtnCrop_clicked() {}
