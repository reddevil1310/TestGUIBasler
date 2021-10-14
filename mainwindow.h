#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <opencv2/opencv.hpp>
#include <MatToQImage.h>
#include "thread_grabframe_basler.h"
#include "threadbaslerpylon.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void updateDisplay(cv::Mat mat_image, QImage qimage);

    void on_pushButtonCapture_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

protected:
    void closeEvent(QCloseEvent *event);


private:
    Ui::MainWindow *ui;
    QGraphicsScene video_scene;
    QGraphicsPixmapItem videoPixmap;
    cv::Mat originalMat;
    QImage originalImage;
    QThreadPool* pool;
    Thread_GrabFrame_Basler* basler;
    Pylon::IPylonDevice* d;
    std::chrono::steady_clock::time_point t1, t2, t3, t4;

    ThreadBaslerPylon* pbasler;
};
#endif // MAINWINDOW_H
