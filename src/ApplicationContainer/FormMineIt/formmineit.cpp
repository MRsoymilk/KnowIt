#include "formmineit.h"

#include "FormDataset/formdataset.h"
#include "FormPlot/formplot.h"
#include "FormProperty/formproperty.h"
#include "ui_formmineit.h"

FormMineIt::FormMineIt(QWidget *parent) : QWidget(parent), ui(new Ui::FormMineIt) {
  ui->setupUi(this);
  init();
}

FormMineIt::~FormMineIt() { delete ui; }

void FormMineIt::init() {
  m_property = new FormProperty;
  ui->gLayProperty->addWidget(m_property);
  m_plot = new FormPlot;
  ui->gLayPlot->addWidget(m_plot);
  m_dataset = new FormDataset;
  ui->gLayDataset->addWidget(m_dataset);

  connect(m_dataset, &FormDataset::itInfo, m_property, &FormProperty::onItInfo);
  connect(m_dataset, &FormDataset::itPlot, m_plot, &FormPlot::onItPlot);
  connect(m_property, &FormProperty::selectedDataDelete, m_dataset, &FormDataset::onSelectedDataDelete);
  connect(m_property, &FormProperty::dataAdd, m_dataset, &FormDataset::onUpdateData);

  connect(m_dataset, &FormDataset::itStructure, m_plot, &FormPlot::onItSpectral);
  connect(m_dataset, &FormDataset::itSpectral, m_property, &FormProperty::onItStructure);
}

void FormMineIt::retranslateUI() {
  ui->retranslateUi(this);
  if (m_property) {
    m_property->retranslateUI();
  }
  if (m_plot) {
    m_plot->retranslateUI();
  }
  if (m_dataset) {
    m_dataset->retranslateUI();
  }
}
