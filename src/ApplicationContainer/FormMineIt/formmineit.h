#ifndef FORMMINEIT_H
#define FORMMINEIT_H

#include <QWidget>

class FormProperty;
class FormDataset;
class FormPlot;

namespace Ui {
class FormMineIt;
}

class FormMineIt : public QWidget {
  Q_OBJECT

 public:
  explicit FormMineIt(QWidget *parent = nullptr);
  ~FormMineIt();
  void init();
  void retranslateUI();

 private:
  Ui::FormMineIt *ui;
  FormProperty *m_property;
  FormDataset *m_dataset;
  FormPlot *m_plot;
};

#endif  // FORMMINEIT_H
