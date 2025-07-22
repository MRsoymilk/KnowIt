#include "editcontainer.h"

#include <QRegularExpression>

#include "PeakInfo/peakinfo.h"
#include "SingleLineInfo/singlelineinfo.h"
#include "g_define.h"
#include "ui_editcontainer.h"

EditContainer::EditContainer(TYPE type, QWidget *parent) : QDialog(parent), ui(new Ui::EditContainer) {
  ui->setupUi(this);
  m_type = type;
  ui->tBtnAdd->setObjectName("add");
}

EditContainer::~EditContainer() { delete ui; }

void EditContainer::setData(const QString &data) {
  switch (m_type) {
    case EditContainer::PEAK: {
      QRegularExpression re(R"(\(\s*([+-]?\d*\.?\d+)\s*,\s*([+-]?\d*\.?\d+)\s*\))");
      QRegularExpressionMatchIterator i = re.globalMatch(data);

      while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        double x = match.captured(1).toDouble();
        double y = match.captured(2).toDouble();

        auto *info = new PeakInfo(this, x, y);
        ui->vLay->addWidget(info);
      }
    } break;
    case EditContainer::SINGLE_LINE: {
      QStringList list = data.split(DELIMITER);
      for (auto x : list) {
        auto *info = new SingleLineInfo(x);
        ui->vLay->addWidget(info);
      }
    } break;
  }
}

const QString EditContainer::getData() {
  QStringList result;
  int count = ui->vLay->count();

  switch (m_type) {
    case EditContainer::PEAK: {
      for (int i = 0; i < count; ++i) {
        QLayoutItem *item = ui->vLay->itemAt(i);
        QWidget *widget = item->widget();
        if (!widget) continue;

        PeakInfo *info = qobject_cast<PeakInfo *>(widget);
        if (!info) continue;

        result << QString("(%1, %2)").arg(info->getWavelength()).arg(info->getIntensity());
      }
    } break;
    case EditContainer::SINGLE_LINE: {
      for (int i = 0; i < count; ++i) {
        QLayoutItem *item = ui->vLay->itemAt(i);
        QWidget *widget = item->widget();
        if (!widget) continue;

        SingleLineInfo *info = qobject_cast<SingleLineInfo *>(widget);
        if (!info) continue;

        result << info->getInfo();
      }
    } break;
  }

  return result.join(";");
}

void EditContainer::on_tBtnAdd_clicked() {
  switch (m_type) {
    case EditContainer::PEAK: {
      auto *info = new PeakInfo(this, 0, 0);
      ui->vLay->addWidget(info);
    } break;
    case EditContainer::SINGLE_LINE: {
      auto *info = new SingleLineInfo();
      ui->vLay->addWidget(info);
    } break;
  }
}

void EditContainer::on_buttonBox_accepted() {}
