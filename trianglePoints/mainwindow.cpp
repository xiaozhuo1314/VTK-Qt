#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    trianglePoints();
}

MainWindow::~MainWindow()
{

}

void MainWindow::trianglePoints()
{
    //创建点数据
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(1.0, 0.0, 0.0);
    points->InsertNextPoint(0.0, 0.0, 0.0);
    points->InsertNextPoint(0.0, 1.0, 0.0);

    //创建vtkPolyData类型数据，vtkPolyData派生自vtkPointSet，vtkPointSet是vtkDataSet的子类。
    //vtkDataSet是抽象类，具体工作需要其字类实现
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    //将创建的点数据添加到vtkPolyData里
    polyData->SetPoints(points);

    //输出vtk文件
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName("triangle.vtk");
    writer->SetInputData(polyData);
    writer->Write();
}
