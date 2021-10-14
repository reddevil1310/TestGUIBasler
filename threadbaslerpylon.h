#ifndef THREADBASLERPYLON_H
#define THREADBASLERPYLON_H
#include <QThread>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include <pylon/PylonIncludes.h>
#include <QImage>
#include <iostream>
#include <QWaitCondition>

class ThreadBaslerPylon : public QThread
{
    Q_OBJECT

public:
    ThreadBaslerPylon(QObject *parent = nullptr);
    ~ThreadBaslerPylon();


    void Play();
    void Stop();

    void AttachDevice(Pylon::IPylonDevice *de);
    void DettachDevice();
    bool isAttached();

    void Open();
    bool isOpen();

    void Close();

    void getProps();
    int getWidth();
    int getHeight();

protected:
    void run() override;

signals:
    void sendImage(const cv::Mat& image,const QImage& qimage);

private:
    int width, height;
    bool stop, open, attach;
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
    QWaitCondition condition;


};

#endif // THREADBASLERPYLON_H
