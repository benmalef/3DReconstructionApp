#include "stereorectification.h"

StereoRectification::StereoRectification() { readParametersFromFile(); }
StereoRectification::~StereoRectification() { destroyAllWindows(); }
void StereoRectification::readParametersFromFile() {
  FileStorage cvFile =
      FileStorage("calibrationParamaters.xml", FileStorage::READ);
  cvFile["leftStereoMapX"] >> leftStereoMapX;
  cvFile["leftStereoMapY"] >> leftStereoMapY;
  cvFile["rightStereoMapX"] >> rightStereoMapX;
  cvFile["rightStereoMapY"] >> rightStereoMapY;
  cvFile["Q"] >> Q;
  cvFile.release();
}
