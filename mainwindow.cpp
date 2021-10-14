#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->graphicsView->setScene(&video_scene);
    video_scene.addItem(&videoPixmap);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);


    Pylon::PylonInitialize();

    Pylon::CTlFactory& tlFactory = Pylon::CTlFactory::GetInstance();
    Pylon::DeviceInfoList_t devices;
    if (tlFactory.EnumerateDevices(devices) == 0)
    {
        std::cout << "No camera present." << std::endl;
        this->close();
    }
    Pylon::CDeviceInfo info = devices[0];
    d = tlFactory.CreateDevice(devices[0]);


//    baslercam = new BaslerCamera();
//    baslercam->setIPylonDevice(d);
//    baslercam->Open();
//    QObject::connect(baslercam, SIGNAL(processedImage(QImage)),this, SLOT(updateDisplay(QImage)));

     pool = new QThreadPool();

    pbasler = new ThreadBaslerPylon();


}

MainWindow::~MainWindow()
{
    Pylon::PylonTerminate();
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->text()== "Start Capture Basler")
    {
        basler = new Thread_GrabFrame_Basler();
        qRegisterMetaType<cv::Mat>("cv::Mat");
        QObject::connect(basler, SIGNAL(processedImage(cv::Mat, QImage)),this, SLOT(updateDisplay(cv::Mat, QImage)));

        basler->Play(d);
        std::cout<<"PLAY"<<std::endl;
        pool->start(basler);
        ui->pushButton->setText("Stop Capture Basler");
    }
    else
    {
        QObject::disconnect(basler, SIGNAL(processedImage(cv::Mat, QImage)),this, SLOT(updateDisplay(cv::Mat, QImage)));
        basler->Stop();
        std::cout<<"STOP"<<std::endl;
        ui->pushButton->setText("Start Capture Basler");

    }
}

void MainWindow::updateDisplay(cv::Mat mat_image, QImage qimage)
{
    if (!mat_image.empty())
    {
        originalMat = mat_image.clone();
//        originalImage = qimage;
        qimage = qimage.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        videoPixmap.setPixmap(QPixmap::fromImage(qimage));
//        namedWindow("img",0);
//        cv::imshow("img",mat_image);
//        cv::waitKey(1);
    }
}

void MainWindow::on_pushButtonCapture_clicked()
{
    if (!originalMat.empty())
    {
        cv::Mat a=originalMat.clone();
        cv::resize(a,a,cv::Size(500,500));
        cv::namedWindow("img",0);
        cv::imshow("img",a);
        cv::waitKey(1);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text()== "Start Capture Basler 2")
    {
        pbasler->Play();
        ui->pushButton_2->setText("Stop Capture Basler 2");
    }
    else
    {
        pbasler->Stop();
        ui->pushButton_2->setText("Start Capture Basler 2");

    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text()== "Attach")
    {
        ui->pushButton_3->setText("Dettach");
        pbasler->AttachDevice(d);
        pbasler->Open();
        pbasler->getProps();
        qRegisterMetaType<cv::Mat>("cv::Mat");
        QObject::connect(pbasler, SIGNAL(sendImage(cv::Mat, QImage)),this, SLOT(updateDisplay(cv::Mat, QImage)));
    }
    else
    {
        ui->pushButton_3->setText("Attach");
        pbasler->DettachDevice();
        QObject::disconnect(pbasler, SIGNAL(sendImage(cv::Mat, QImage)),this, SLOT(updateDisplay(cv::Mat, QImage)));
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, tr("Exit"), tr("Are you sure you want to exit?"), QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        event->accept();
        pbasler->Close();
        Pylon::PylonTerminate();

    }
    else
    {
        event->ignore();
    }
}
