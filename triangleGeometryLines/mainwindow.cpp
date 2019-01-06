#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    triangleGeometryLines();
}

MainWindow::~MainWindow()
{

}

void MainWindow::triangleGeometryLines()
{
    //创建坐标点
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(1.0, 0.0, 0.0);  //返回点的索引号0
    points->InsertNextPoint(0.0, 0.0, 1.0);  //返回点的索引号1
    points->InsertNextPoint(0.0, 0.0, 0.0);  //返回点的索引号2

    //每两个点之间创建线段
    vtkSmartPointer<vtkLine> line0 = vtkSmartPointer<vtkLine>::New();
    //SetId两个参数，一个是当前线段的端点号，另一个是创建的点的索引号，比如这里的线段有两个端点，故为0和1，索引分别为0和1
    line0->GetPointIds()->SetId(0, 0);   //返回cell object
    line0->GetPointIds()->SetId(1, 1);

    vtkSmartPointer<vtkLine> line1 = vtkSmartPointer<vtkLine>::New();
    line1->GetPointIds()->SetId(0, 1);
    line1->GetPointIds()->SetId(1, 2);

    vtkSmartPointer<vtkLine> line2 = vtkSmartPointer<vtkLine>::New();
    line2->GetPointIds()->SetId(0, 2);
    line2->GetPointIds()->SetId(1, 0);

    //创建cell。存储线段
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(line0);
    lines->InsertNextCell(line1);
    lines->InsertNextCell(line2);

    //将点和线加入数据集中
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    polyData->SetPoints(points);
    polyData->SetLines(lines);
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName("triangle.vtk");
    writer->SetInputData(polyData);
    writer->Write();

    //注意到，这里的三角形是按照一个线段一个线段画的，更加快捷的方法是vtkTriangle类:
    //triangle->GetPointIds()->SetId(0, 0);	triangle->GetPointIds()->SetId(1, 1);	triangle->GetPointIds()->SetId(2, 2);
    //这里整个看成一条折线段，故端点号为0，1，2
}
