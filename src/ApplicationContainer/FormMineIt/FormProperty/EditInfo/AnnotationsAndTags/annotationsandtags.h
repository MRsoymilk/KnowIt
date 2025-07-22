#ifndef ANNOTATIONSANDTAGS_H
#define ANNOTATIONSANDTAGS_H

#include <QWidget>

namespace Ui {
class AnnotationsAndTags;
}

class AnnotationsAndTags : public QWidget {
  Q_OBJECT

 public:
  explicit AnnotationsAndTags(QWidget *parent = nullptr);
  ~AnnotationsAndTags();
  void setAnnotationsAndTags(const QJsonObject &data);
  QJsonObject getAnnotationsAndTags();

 private:
  Ui::AnnotationsAndTags *ui;
};

#endif  // ANNOTATIONSANDTAGS_H
