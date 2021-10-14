#include "thread_grabframe_basler.h"

Thread_GrabFrame_Basler::Thread_GrabFrame_Basler()
{
    Pylon::PylonInitialize();
    stop = true;
    formatConverter.OutputPixelFormat = Pylon::PixelType_BGR8packed;
}

Thread_GrabFrame_Basler::~Thread_GrabFrame_Basler()
{
    Pylon::PylonTerminate();
    std::cout<<"destructed"<<std::endl;
}

void Thread_GrabFrame_Basler::run()
{
    std::cout<<"START grab"<<std::endl;
    if(camera->IsOpen())
    {
        if(!camera->IsGrabbing())
        {
            camera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);
        }

        while (camera->IsGrabbing() && !stop)
        {
            std::cout<<"grab";
            camera->RetrieveResult(5000, ptrGrabResult, Pylon::TimeoutHandling_ThrowException);

            if (ptrGrabResult->GrabSucceeded())
            {
                formatConverter.Convert(pylonImage, ptrGrabResult);
                buffer = pylonImage.GetBuffer();
                frame = cv::Mat(height, width, CV_8UC3, (uint8_t*)buffer);
                img = QImage((const unsigned char*)buffer, width, height, QImage::Format_RGB888);

                emit processedImage(frame, img);
                this->msleep(1);
            }
            std::cout<<"-end grab"<<std::endl;
        }
        std::cout<<"break"<<std::endl;
        camera->Close();
        camera->DetachDevice();
        std::cout<<"close, dettach"<<std::endl;
    }
}

void Thread_GrabFrame_Basler::Stop()
{
    stop = true;
}

void Thread_GrabFrame_Basler::Play(Pylon::IPylonDevice *de)
{
    camera = new Pylon::CInstantCamera();
    camera->Attach(de);
    stop = false;
    camera->Open();
    camera->MaxNumBuffer = 5;
    nodemap = &camera->GetNodeMap();
    w = nodemap->GetNode("Width");
    h = nodemap->GetNode("Height");
    width = w->GetValue();
    height = h->GetValue();
}

void Thread_GrabFrame_Basler::msleep(int ms)
{
    QThread::usleep(ms*1000);
}

