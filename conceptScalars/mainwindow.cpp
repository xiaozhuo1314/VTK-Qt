#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    conceptScalars();
}

MainWindow::~MainWindow()
{
}

void MainWindow::conceptScalars()
{
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 0, 0);

    //创建多边形数据
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);

    //准备加入两个点的标量值,分别为1和2
    vtkSmartPointer<vtkDoubleArray> weights = vtkSmartPointer<vtkDoubleArray>::New();
    weights->SetNumberOfValues(2);
    weights->SetValue(0, 1);
    weights->SetValue(1, 2);
    //先获取多边形数据的点数据指针，然后设置该点数据的标量属性值。
    polyData->GetPointData()->SetScalars(weights);

    //输出索引号为0的点的标量值。
    double weight = vtkDoubleArray::SafeDownCast(polyData->GetPointData()->GetScalars())->GetValue(0);
    qDebug()<<"double weight: "<<weight;
}
