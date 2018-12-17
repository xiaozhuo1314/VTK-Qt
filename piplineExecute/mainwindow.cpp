#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    piplineExecute();
}

MainWindow::~MainWindow()
{

}

void MainWindow::piplineExecute()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    qDebug()<<"Modification Time of reader (After New()): "<<reader->GetMTime();
    reader->SetFileName("/home/silence/Project/VTK-Qt/piplineExecute/VTK-logo.bmp");
    qDebug()<<"Modification Time of reader (After SetFileName()): "<<reader->GetMTime();

    vtkSmartPointer<vtkImageData> imageData = reader->GetOutput(); //GetOutput returns imageData* and getOutputPort returns ImageDataAlgorithm
    //还有之前关于SetInputConnection和SetInputData,如果要建立管道连接，请使用SetInputConnection,如果要处理独立数据集，请使用SetInputData
    qDebug()<<"Modification Time of reader (After GetOutput()): "<<reader->GetMTime();
    reader->Update();
    qDebug()<<"Modification Time of reader (After Update()): "<<reader->GetMTime();

    int dimensions[3];
    imageData->GetDimensions(dimensions);  //获取长宽等
    qDebug()<<dimensions[0]<<" "<<dimensions[1]<<" "<<dimensions[2];

    vtkSmartPointer<vtkImageViewer2> view = vtkSmartPointer<vtkImageViewer2>::New();
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    view->SetupInteractor(interactor);  //设置交互属性
    view->SetInputConnection(reader->GetOutputPort());
    view->Render();
    view->SetSize(dimensions[0], dimensions[1]);
    view->GetRenderWindow()->SetWindowName("vtkPipelineExecute");
    interactor->Initialize();
    interactor->Start();
}
