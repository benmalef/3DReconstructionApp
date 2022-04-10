#include "dialogstereobmcuda.h"
#include "ui_dialogstereobmcuda.h"

DialogStereoBMcuda::DialogStereoBMcuda(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogStereoBMcuda) {
  ui->setupUi(this);
  ui->labelDisparity->setText(QString::number(numDisparities));
  ui->labelBlockSize->setText(QString::number(blockSize));
}

DialogStereoBMcuda::~DialogStereoBMcuda() { delete ui; }

void DialogStereoBMcuda::on_btnExit_clicked() {
  reject();
  btnExit = true;
}

void DialogStereoBMcuda::on_btnSave_clicked() {
  this->btnExit = true;
  int tmpValue = ui->sliderDisparityNum->value();
  int tmpBlockSize = ui->sliderBlockSize->value();
  if (tmpValue % 16 == 0)
    numDisparities = tmpValue;
  if (tmpBlockSize % 2 == 1)
    blockSize = tmpBlockSize;
}

void DialogStereoBMcuda::stereoBMcudaExecute() {
  StereoRectification stereoRectification;
  StereoCamera stereoCamera;
  Mat leftFrame;
  Mat rightFrame;
  auto bm = cuda::createStereoBM(numDisparities, blockSize);
  cuda::GpuMat cudaLeftFrame, cudaRightFrame;

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

    Mat disparityMap(leftFrame.size(), CV_16S);
    cuda::GpuMat cudaDisparityMap(leftFrame.size(), CV_16S);
    cuda::GpuMat cudaDrawColorDisparity(leftFrame.size(), CV_8UC4);

    cudaLeftFrame.upload(leftFrame);
    cudaRightFrame.upload(rightFrame);

    bm->compute(cudaLeftFrame, cudaRightFrame, cudaDisparityMap);
    cuda::drawColorDisp(cudaDisparityMap, cudaDrawColorDisparity, 128);
    // cudaDisparityMap.download(disparityMap);
    cudaDrawColorDisparity.download(disparityMap);
    imshow("LeftFrame", leftFrame);
    imshow("RightFrame", rightFrame);
    //    imshow("DisparityMap", (Mat_<uchar>)disparityMap);
    //    applyColorMap(disparityMap, disparityMap, COLORMAP_RAINBOW);
    imshow("DisparityMap", disparityMap);

    char key = waitKey(1);
    if (key == 'q' || btnExit == true) {
      break;
    }
  }
}

void DialogStereoBMcuda::on_pushButton_clicked() {
  btnExit = false;
  stereoBMcudaExecute();
}

void DialogStereoBMcuda::on_sliderDisparityNum_valueChanged(int value) {
  if (value % 16 == 0)
    ui->labelDisparity->setText(QString::number(value));
}

void DialogStereoBMcuda::on_sliderBlockSize_valueChanged(int value) {
  if (value % 2 == 1)
    ui->labelBlockSize->setText(QString::number(value));
}
