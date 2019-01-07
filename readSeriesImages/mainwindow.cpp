#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    readSeriesImages3();
}

MainWindow::~MainWindow()
{

}

void MainWindow::readSeriesImages1()
{
    vtkSmartPointer<vtkStringArray> fileArray = vtkSmartPointer<vtkStringArray>::New();
    char filename[128];
    for(int i=1;i<=100;i++){
        sprintf(filename, "/home/silence/Project/VTK-Qt/readSeriesImages/Head/head%03d.jpg", i);
        vtkStdString fileStr(filename);
        fileArray->InsertNextValue(fileStr);
    }
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    reader->SetFileNames(fileArray);

    //显示图像
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(reader->GetOutputPort());
    viewer->SetSlice(50);
    viewer->SetSliceOrientationToXY();
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(style);
    viewer->SetupInteractor(interactor);
    viewer->Render();
    interactor->Start();
}

void MainWindow::readSeriesImages2()
{
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    reader->SetFilePrefix("/home/silence/Project/VTK-Qt/readSeriesImages/Head/head");
    reader->SetFilePattern("%s%03d.jpg");
    reader->SetDataExtent(0, 255, 0, 255, 1, 100); //图像大小是256x256，故为0, 255, 0, 255；图像名字的数字是从1到100
    reader->Update();

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetInteractorStyle(style);
    viewer->SetSlice(50);
    viewer->SetSliceOrientationToXY();
    viewer->SetupInteractor(interactor);
    viewer->Render();
    interactor->Start();
}

void MainWindow::readSeriesImages3()   //结果与前两个不一样，这个方不对
{
    vtkSmartPointer<vtkImageAppend> append = vtkSmartPointer<vtkImageAppend>::New();
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    append->SetAppendAxis(2); //设置z轴，0是x轴，1是y轴
    char filename[128];
    for(int i=1;i<=100;i++){
        sprintf(filename, "/home/silence/Project/VTK-Qt/readSeriesImages/Head/head%03d.jpg", i);
        reader->SetFileName(filename);
        append->AddInputConnection(reader->GetOutputPort());
    }

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetInteractorStyle(style);
    viewer->SetSlice(50);
    viewer->SetSliceOrientationToXY();
    viewer->SetupInteractor(interactor);
    viewer->Render();
    interactor->Start();
}
