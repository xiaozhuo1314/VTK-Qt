#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    headMHD();
}

MainWindow::~MainWindow()
{

}

void MainWindow::headMHD()
{
    vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/headMHD/head.mhd");
    reader->Update();

    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(reader->GetOutputPort());

    //设置基本属性
    viewer->SetSize(640, 480);

    //SetColorWindow和SetColorLevel共同确定把输入的图像像素值线性映射到的范围。实际映射的范围是2*SetColorLevel–SetColorWindow 至 SetColorWindow
    //就是说像素在这个范围的能够呈现出来
    viewer->SetColorLevel(500);
    viewer->SetColorWindow(2000);
    viewer->SetSlice(40);  //设置边界大小
    viewer->SetSliceOrientationToXY();
    viewer->Render();
    viewer->GetRenderer()->SetBackground(1, 1, 1);
    viewer->GetRenderWindow()->SetWindowName("ImageViewer2D");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    viewer->SetupInteractor(interactor);
    interactor->Initialize();
    interactor->Start();
}
