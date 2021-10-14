

#ifndef MATTOQIMAGE_H
#define MATTOQIMAGE_H

// Qt header files
#include <QtGui>
#include <QDebug>
// OpenCV header files
#include "opencv2/opencv.hpp"

using namespace cv;

QImage MatToQImage(const Mat&);
QImage MatToQImage2(const Mat&);

#endif // MATTOQIMAGE_H
