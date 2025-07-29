#include "downloadhelper.h"

#include <QDir>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMimeData>

#include "g_define.h"
#include "myhttp.h"
#include "ui_downloadhelper.h"

DownloadHelper::DownloadHelper(QWidget *parent) : QWidget(parent), ui(new Ui::DownloadHelper) {
  ui->setupUi(this);
  init();
}

DownloadHelper::~DownloadHelper() { delete ui; }

void DownloadHelper::init() {
  setAcceptDrops(true);

  ui->graphicsView->setScene(new QGraphicsScene(this));
  ui->graphicsView->setRenderHint(QPainter::Antialiasing);
  ui->graphicsView->setRenderHint(QPainter::SmoothPixmapTransform);
  ui->graphicsView->setAcceptDrops(true);
  ui->graphicsView->viewport()->setAcceptDrops(true);
  ui->graphicsView->viewport()->installEventFilter(this);

  ui->lineEditDir->setText(QDir::tempPath());
}

void DownloadHelper::display(const QPixmap &pix) {
  ui->graphicsView->scene()->clear();
  ui->graphicsView->scene()->addPixmap(pix.scaled(ui->graphicsView->size(), Qt::KeepAspectRatio));
}

void DownloadHelper::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls() || event->mimeData()->hasImage()) {
    event->acceptProposedAction();
  }
}

void DownloadHelper::dropEvent(QDropEvent *event) {
  const QMimeData *mimeData = event->mimeData();

  if (mimeData->hasUrls()) {
    QList<QUrl> urlList = mimeData->urls();
    for (const QUrl &url : urlList) {
      if (url.isLocalFile()) {
        QString localPath = url.toLocalFile();

        QPixmap pix(localPath);
        if (!pix.isNull()) {
          display(pix);
        }
      } else {
        QString imageUrl = url.toString();

        QString saveDir = ui->lineEditDir->text().trimmed();
        QString name = ui->lineEditName->text().trimmed();

        if (saveDir.isEmpty()) {
          SHOW_AUTO_CLOSE_MSGBOX(this, TITLE_WARNING, tr("Save directory is empty!"));
          return;
        }

        QString savePath;
        if (!name.isEmpty()) {
          savePath = QDir(saveDir).filePath(name);
        } else {
          QString dftName = QString("%1.png").arg(TIMESTAMP());
          ui->lineEditName->setText(dftName);
          QFileInfo fi(dftName);
          savePath = QDir(saveDir).filePath(fi.fileName());
        }

        MY_HTTP->getImage(
            imageUrl,
            [=](QPixmap pix) {
              if (!pix.isNull()) {
                pix.save(savePath);
                display(pix);
              }
            },
            [=](QString err) { SHOW_AUTO_CLOSE_MSGBOX(this, TITLE_WARNING, tr("Download failed: %1").arg(err)); });
      }
    }
  } else if (mimeData->hasImage()) {
    QImage image = qvariant_cast<QImage>(mimeData->imageData());
    QPixmap pix = QPixmap::fromImage(image);
    display(pix);
  }
}

bool DownloadHelper::eventFilter(QObject *watched, QEvent *event) {
  if (watched == ui->graphicsView->viewport()) {
    if (event->type() == QEvent::DragEnter) {
      QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent *>(event);
      if (dragEvent->mimeData()->hasUrls() || dragEvent->mimeData()->hasImage()) {
        dragEvent->acceptProposedAction();
        return true;
      }
    } else if (event->type() == QEvent::Drop) {
      QDropEvent *dropEvent = static_cast<QDropEvent *>(event);
      this->dropEvent(dropEvent);
      return true;
    }
  }

  return QWidget::eventFilter(watched, event);
}

void DownloadHelper::on_tBtnChooseDir_clicked() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("choose save dir"), QDir::homePath());
  if (!dir.isEmpty()) {
    ui->lineEditDir->setText(dir);
  }
}
