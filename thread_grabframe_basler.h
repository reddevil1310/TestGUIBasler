#ifndef THREAD_GRABFRAME_BASLER_H
#define THREAD_GRABFRAME_BASLER_H
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <pylon/PylonIncludes.h>
#include <QImage>
#include <QObject>
#include <QRunnable>

class Thread_GrabFrame_Basler : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Thread_GrabFrame_Basler();
    ~Thread_GrabFrame_Basler();



    void run() override;

    void Stop();
    void Play(Pylon::IPylonDevice *de);

    void msleep(int ms);

signals:
//Signal to output frame to be displayed
     void processedImage(const cv::Mat& image,const QImage& qimage);

private:
    int width, height;
    bool stop;
    QMutex mutex;
    cv::Mat frame,RGBframe;
    QImage img;
    int FPS;
    Pylon::CInstantCamera* camera;
    GenApi::INodeMap* nodemap;
    GenApi::CIntegerPtr w, h;
    void *buffer;

    Pylon::CImageFormatConverter formatConverter;
    Pylon::CPylonImage pylonImage;
    Pylon::CGrabResultPtr ptrGrabResult;
    Pylon::PylonAutoInitTerm autoInitTerm;
};

#endif // THREAD_GRABFRAME_BASLER_H
