#include "dialogsemiglobalblockmatching.h"
#include "ui_dialogsemiglobalblockmatching.h"

DialogSemiGlobalBlockMatching::DialogSemiGlobalBlockMatching(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogSemiGlobalBlockMatching) {
  ui->setupUi(this);
}

DialogSemiGlobalBlockMatching::~DialogSemiGlobalBlockMatching() { delete ui; }

void DialogSemiGlobalBlockMatching::SGBmatchingExecute() {
  std::chrono::time_point<std::chrono::system_clock> start, end;
  StereoCamera stereoCamera;
  StereoRectification stereoRectification;
  Mat leftFrame;
  Mat rightFrame;
  Mat disparityMap, disparityMapVis;
  int minDisparity = 0;
  int numDisparities = 128;
  int blockSize = 5;
  int P1 = 600;
  int P2 = 2400;
  int disp12MaxDiff = 10;
  int preFilterCap = 4;
  int uniquenessRatio = 1;
  int speckleWindowSize = 150;
  int speckleRange = 2;
  int mode = StereoSGBM::MODE_SGBM;

  auto cpuStereoSGBM = StereoSGBM::create(
      minDisparity, numDisparities, blockSize, P1, P2, disp12MaxDiff,
      preFilterCap, uniquenessRatio, speckleWindowSize, speckleRange, mode);
  while (stereoCamera.isOpened()) {
    stereoCamera.getCapLeft().read(leftFrame);
    stereoCamera.getCapRight().read(rightFrame);

    remap(leftFrame, leftFrame, stereoRectification.leftStereoMapX,
          stereoRectification.leftStereoMapY, cv::INTER_LANCZOS4,
          cv::BORDER_CONSTANT, 0);
    remap(rightFrame, rightFrame, stereoRectification.rightStereoMapX,
          stereoRectification.rightStereoMapY, cv::INTER_LANCZOS4,
          cv::BORDER_CONSTANT, 0);
    cvtColor(rightFrame, rightFrame, COLOR_BGR2GRAY);
    cvtColor(leftFrame, leftFrame, COLOR_BGR2GRAY);
    cpuStereoSGBM->compute(leftFrame, rightFrame, disparityMap);
    ximgproc::getDisparityVis(disparityMap, disparityMapVis, 1.0);
    imshow("LeftFrame", leftFrame);
    imshow("RightFrame", rightFrame);
    imshow("DisparityMap", disparityMapVis);
    char key = waitKey(1);
    if (key == 'q' || this->btnExit == true) {
      break;
    }
  }
}

void DialogSemiGlobalBlockMatching::on_btnExit_clicked() {
  btnExit = true;
  reject();
}

void DialogSemiGlobalBlockMatching::on_btnStart_clicked() {
  SGBmatchingExecute();
}
