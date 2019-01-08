#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    imageChangeInformation();
}

MainWindow::~MainWindow()
{

}

void MainWindow::imageChangeInformation()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/imageChangeInformation/lena.bmp");
    reader->Update();

    int dims[3];
    double origin[3];
    double spaceing[3];
    reader->GetOutput()->GetDimensions(dims);
    qDebug()<<"原图像维数:" <<dims[0]<<" "<<dims[1]<<" "<<dims[2];
    reader->GetOutput()->GetOrigin(origin);
    qDebug()<<"原图像原点:" <<origin[0]<<" "<<origin[1]<<" "<<origin[2];
    reader->GetOutput()->GetSpacing(spaceing);
    qDebug()<<"原像素间隔:" <<spaceing[0]<<" "<<spaceing[1]<<" "<<spaceing[2];

    vtkSmartPointer<vtkImageChangeInformation> changer = vtkSmartPointer<vtkImageChangeInformation>::New();
    changer->SetInputData(reader->GetOutput());
    changer->SetOutputOrigin(100, 100, 0);
    changer->SetOutputSpacing(5,5,1);
    changer->SetCenterImage(1);  //里面的1是布尔值
    changer->Update();
    changer->GetOutput()->GetDimensions(dims);
    qDebug()<<"原图像维数:" <<dims[0]<<" "<<dims[1]<<" "<<dims[2];
    changer->GetOutput()->GetOrigin(origin);
    qDebug()<<"原图像原点:" <<origin[0]<<" "<<origin[1]<<" "<<origin[2];
    changer->GetOutput()->GetSpacing(spaceing);
    qDebug()<<"原像素间隔:" <<spaceing[0]<<" "<<spaceing[1]<<" "<<spaceing[2];
}
