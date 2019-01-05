#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    triangleVertices();
}

MainWindow::~MainWindow()
{

}

void MainWindow::triangleVertices()
{
    double x[3] = {1.0, 0.0, 0.0};
    double y[3] = {0.0, 0.0, 1.0};
    double z[3] = {0.0, 0.0, 0.0};

    //创建点数据以及在每个点坐标上加入顶点(Vertex)这种Cell
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> vertices = vtkSmartPointer<vtkCellArray>::New();

    for(int i=0; i<3; i++){
        vtkIdType pid[1];  //pid只有一个元素
        //把每个点坐标加入到vtkPoints中，InsertNextPoint()返回加入的点的索引号，我们需要使用这个索引号来创建Vertex类型的Cell
        pid[0] = points->InsertNextPoint(x[i], y[i], z[i]);
        vertices->InsertNextCell(1, pid);
    }
    vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
    //添加几何信息
    polyData->SetPoints(points);
    //添加拓扑信息
    polyData->SetVerts(vertices);

    //写入vtk文件
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkSmartPointer<vtkPolyDataWriter>::New();
    writer->SetFileName("triangle.vtk");
    writer->SetInputData(polyData);
    writer->Write();
}
