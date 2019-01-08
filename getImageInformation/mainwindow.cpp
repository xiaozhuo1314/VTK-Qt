#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    getImageInformation();
}

MainWindow::~MainWindow()
{

}

void MainWindow::getImageInformation()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/getImageInformation/lena.bmp");
    reader->Update();

    int dimensions[3];
    reader->GetOutput()->GetDimensions(dimensions);
    qDebug()<<"图像维数:" <<dimensions[0]<<" "<<dimensions[1]<<" "<<dimensions[2];
    double origin[3];
    reader->GetOutput()->GetOrigin(origin);
    qDebug()<<"图像原点:" <<origin[0]<<" "<<origin[1]<<" "<<origin[2];
    double spaceing[3];
    reader->GetOutput()->GetSpacing(spaceing);
    qDebug()<<"像素间隔:" <<spaceing[0]<<" "<<spaceing[1]<<" "<<spaceing[2];
}
