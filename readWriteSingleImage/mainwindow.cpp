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
    //vtkActor、vtkRenderer、vtkRenderWindow等已经封装在vtkImageViewer2中，可视化管线执行时只需要vtkRenderWindowInteractor即可，但是用到这些类具体功能的话还是需要将头文件包含进去
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
