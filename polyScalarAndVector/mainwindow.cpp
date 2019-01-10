#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    polyScalarAndVector();
}

MainWindow::~MainWindow()
{

}

void MainWindow::polyScalarAndVector()
{
    vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
    plane->SetXResolution(3);
    plane->SetYResolution(3);  //设置3x3方格
    plane->Update();

    vtkSmartPointer<vtkPolyData> polyData = plane->GetOutput();
    //标量数据
    vtkSmartPointer<vtkFloatArray> scalar = vtkSmartPointer<vtkFloatArray>::New();
    scalar->SetNumberOfComponents(1);
    //矢量数据
    vtkSmartPointer<vtkFloatArray> vector = vtkSmartPointer<vtkFloatArray>::New();
    vector->SetNumberOfComponents(3);  //设置三个组分
    for(int i=0;i<9;i++)
    {
        scalar->InsertTuple1(i,i+1);  //设置标量数据，返回索引号
        vector->InsertTuple3(i,0.0, 0.0, 1.0);  //设置矢量数据，返回索引号
    }
    polyData->GetCellData()->SetScalars(scalar);  //依次设置9个方格标量数据
    polyData->GetCellData()->SetVectors(vector);  //依次设置9个方格矢量数据

    //颜色映射表
    vtkSmartPointer<vtkLookupTable> table = vtkSmartPointer<vtkLookupTable>::New();
    table->SetNumberOfTableValues(10);  //映射表数据个数
    table->SetTableValue(0, 0, 0, 0, 1);  //第一个参数是索引号，后四个数为rgba，a是透明度
//    table->SetTableValue(1, 0, 0, 0, 0);
//    table->SetTableValue(2, 0, 0, 0, 0);
//    table->SetTableValue(3, 0, 0, 0, 0);
//    table->SetTableValue(4, 0, 0, 0, 0);
//    table->SetTableValue(5, 0, 0, 0, 0);
//    table->SetTableValue(6, 0, 0, 0, 0);
//    table->SetTableValue(7, 0, 0, 0, 0);
//    table->SetTableValue(8, 0, 0, 0, 0);
//    table->SetTableValue(9, 0, 0, 0, 0);
    table->SetTableValue(1, 0.8900, 0.8100, 0.3400, 1);
    table->SetTableValue(2, 1.0000, 0.3882, 0.2784, 1);
    table->SetTableValue(3, 0.9608, 0.8706, 0.7020, 1);
    table->SetTableValue(4, 0.9020, 0.9020, 0.9804, 1);
    table->SetTableValue(5, 1.0000, 0.4900, 0.2500, 1);
    table->SetTableValue(6, 0.5300, 0.1500, 0.3400, 1);
    table->SetTableValue(7, 0.9804, 0.5020, 0.4471, 1);
    table->SetTableValue(8, 0.7400, 0.9900, 0.7900, 1);
    table->SetTableValue(9, 0.2000, 0.6300, 0.7900, 1);
    table->Build();

    //mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polyData);
    mapper->SetScalarRange(0, 9);
    //actor
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetBackground(0.0, 0.0, 0.0);

    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->SetSize(320, 320);
    renWin->SetWindowName("Setting Attribution of Vectors and Scalars");

    vtkSmartPointer<vtkRenderWindowInteractor> rwi =vtkSmartPointer<vtkRenderWindowInteractor>::New();
    rwi->SetRenderWindow(renWin);
    rwi->Start();
}
