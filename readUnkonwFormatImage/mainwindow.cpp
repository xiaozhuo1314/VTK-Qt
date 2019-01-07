#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    readUnkonwFormatImage();
}

MainWindow::~MainWindow()
{

}

void MainWindow::readUnkonwFormatImage()
{
    vtkSmartPointer<vtkImageReader2Factory> readerF = vtkSmartPointer<vtkImageReader2Factory>::New();
    //CreateImageReader2遍历所有图片读取类，找到了就返回true，并将vtkImageReader2(即reader)向下转型成对应类，但是此时并没有读取数据，需要将此时的reader读取一遍图像
    vtkSmartPointer<vtkImageReader2> reader = readerF->CreateImageReader2("/home/silence/Project/VTK-Qt/readUnkonwFormatImage/VTK-logo");
    //qDebug()<<reader->GetClassName(); //reader返回vtkPNGReader类
    reader->SetFileName("/home/silence/Project/VTK-Qt/readUnkonwFormatImage/VTK-logo");
    reader->Update();   //将数据实际读取过来，不需要等最后反向执行

    //显示图像
    vtkSmartPointer<vtkImageViewer2> viewer = vtkSmartPointer<vtkImageViewer2>::New();
    viewer->SetInputConnection(reader->GetOutputPort());
    viewer->Render();
    viewer->GetRenderWindow()->SetWindowName("haha");
    viewer->GetRenderer()->ResetCamera();
    viewer->Render();

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    viewer->SetupInteractor(interactor);
    interactor->Start();
}
