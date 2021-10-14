#include "threadbaslerpylon.h"

ThreadBaslerPylon::ThreadBaslerPylon(QObject *parent)
{
    Pylon::PylonInitialize();
    stop = true;
    formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;
    camera = new Pylon::CInstantCamera();
    open = false;
    attach = false;
    width = 0;
    height = 0;
}

ThreadBaslerPylon::~ThreadBaslerPylon()
{
    mutex.lock();
    Pylon::PylonTerminate();
    std::cout<<"destructed ThreadBaslerPylon"<<std::endl;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void ThreadBaslerPylon::Play()
{
    if (!isRunning())
    {
        stop = false;
        start(LowPriority);
    }
    else
    {
        condition.wakeOne();
    }
}

void ThreadBaslerPylon::Stop()
{
    stop = true;
}

void ThreadBaslerPylon::AttachDevice(Pylon::IPylonDevice *pde)
{
    if (attach)
    {
        camera->DetachDevice();
    }
    camera->Attach(pde);
    attach = true;
}

void ThreadBaslerPylon::DettachDevice()
{
    if (open)
    {
        stop = true;
        camera->Close();
        open = false;
    }
    if (attach)
    {
        camera->DetachDevice();
        attach = false;
    }
}

bool ThreadBaslerPylon::isAttached()
{
    return attach;
}

void ThreadBaslerPylon::Open()
{
    if (attach)
    {
        camera->Open();
        open = true;
        camera->MaxNumBuffer = 5;
        std::cout<<"Opened"<<std::endl;
    }

}

bool ThreadBaslerPylon::isOpen()
{
    return open;
}

void ThreadBaslerPylon::Close()
{
    if (open)
    {
        camera->Close();
        open = false;
    }
}

void ThreadBaslerPylon::getProps()
{
    if (open)
    {
        nodemap = &camera->GetNodeMap();
        w = nodemap->GetNode("Width");
        h = nodemap->GetNode("Height");
        width = w->GetValue();
        height = h->GetValue();
    }
}

int ThreadBaslerPylon::getWidth()
{
    return width;
}

int ThreadBaslerPylon::getHeight()
{
    return height;
}

void ThreadBaslerPylon::run()
{
    if(this->isOpen())
    {
        if(!camera->IsGrabbing())
        {
            camera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);
        }
        while (camera->IsGrabbing() && !stop)
        {

            camera->RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

            if (ptrGrabResult->GrabSucceeded())
            {
                formatConverter.Convert(pylonImage, ptrGrabResult);
                buffer = pylonImage.GetBuffer();
                frame = cv::Mat(height, width, CV_8UC3, (uint8_t*)buffer);
                img = QImage((const unsigned char*)buffer, width, height, QImage::Format_RGB888);
                emit sendImage(frame, img);
                this->msleep(30);
            }
        }
    }
}
