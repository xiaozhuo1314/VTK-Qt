#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    readWriteSingleImage();
}

MainWindow::~MainWindow()
{

}

void MainWindow::readWriteSingleImage()
{
    //读取图像
    vtkSmartPointer<vtkPNGReader> pngReader = vtkSmartPointer<vtkPNGReader>::New();
    pngReader->SetFileName("/home/silence/Project/VTK-Qt/readWriteSingleImage/VTK-logo.png");

    //显示图像
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(pngReader->GetOutputPort());
    viewer->Render();
    viewer->GetRenderWindow()->SetWindowName("haha");
    viewer->GetRenderer()->ResetCamera();
    viewer->Render();

    //设置交互
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    viewer->SetupInteractor(interactor);

    //保存jpeg
    vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();
    jpegWriter->SetFileName("VTK-logo.jpg");
    jpegWriter->SetInputConnection(pngReader->GetOutputPort());
    jpegWriter->Write();

    interactor->Start();
}
