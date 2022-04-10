#ifndef DIALOGSEMIGLOBALBLOCKMATCHING_H
#define DIALOGSEMIGLOBALBLOCKMATCHING_H

#include "stereocamera.h"
#include "stereorectification.h"
#include <QDialog>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudastereo.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/viz/viz3d.hpp>
#include <opencv2/viz/vizcore.hpp>
#include <opencv2/ximgproc/disparity_filter.hpp>

namespace Ui {
class DialogSemiGlobalBlockMatching;
}

class DialogSemiGlobalBlockMatching : public QDialog {
  Q_OBJECT

public:
  explicit DialogSemiGlobalBlockMatching(QWidget *parent = nullptr);
  ~DialogSemiGlobalBlockMatching();
  void SGBmatchingExecute();

private slots:
  void on_btnStart_clicked();

private slots:
  void on_btnExit_clicked();

private:
  Ui::DialogSemiGlobalBlockMatching *ui;
  bool btnExit = false;
};

#endif // DIALOGSEMIGLOBALBLOCKMATCHING_H
