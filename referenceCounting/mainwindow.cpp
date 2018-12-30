#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    referenceCounting();
}

MainWindow::~MainWindow()
{

}

void MainWindow::referenceCounting()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/referenceCounting/VTK-logo.bmp");
    reader->Update();
    //qDebug()<<reader->GetOutput()->GetClassName();    //vtkImageData
    qDebug()<<"Reference Count of reader->GetOutput (Before Assignment) = "<<reader->GetOutput()->GetReferenceCount();
    vtkSmartPointer<vtkImageData> image1 = reader->GetOutput();
    qDebug()<<"Reference Count of reader->GetOutput (Assign to image1) = "<<reader->GetOutput()->GetReferenceCount();
    qDebug()<<"Reference Count of image1->GetOutput = "<<image1->GetReferenceCount();
    vtkSmartPointer<vtkImageData> image2 = reader->GetOutput();
    qDebug()<<"Reference Count of reader->GetOutput (Assign to image2) = "<<reader->GetOutput()->GetReferenceCount();
    qDebug()<<"Reference Count of image2->GetOutput = "<<image2->GetReferenceCount();
    qDebug()<<"Reference Count of image1->GetOutput again = "<<image1->GetReferenceCount();

    //智能指针函数
    qDebug()<<"my functin reference count = "<<myfunction()->GetReferenceCount();
    vtkSmartPointer<vtkImageData> imageData = myfunction();
    qDebug()<<"my functin reference count again = "<<myfunction()->GetReferenceCount();
    qDebug()<<"my functin reference count after addignment = "<<imageData->GetReferenceCount();

    //如果没有给对象分配内存，仍然可以使用智能指针:
    vtkSmartPointer<vtkBMPReader> bmpReader = vtkSmartPointer<vtkBMPReader>::New();
    //one
    vtkImageData *pd = bmpReader->GetOutput();  //没有new就没有分配地址，所以之后不用delete
    //or
    vtkSmartPointer<vtkImageData> pdd = bmpReader->GetOutput();
}

vtkSmartPointer<vtkImageData> MainWindow::myfunction()
{
    vtkSmartPointer<vtkImageData> mydata = vtkSmartPointer<vtkImageData>::New();
    qDebug()<<"in function, myfunction reference count = "<<mydata->GetReferenceCount();
    return mydata;
}
