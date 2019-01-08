#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    grayAccumulate();
}

MainWindow::~MainWindow()
{

}

void MainWindow::grayAccumulate()
{
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/grayAccumulate/lena-gray.jpg");
    reader->Update();

    int bins   = 16;  //指共有几个长方形
    int comps  = 1;  //组分是1

    vtkSmartPointer<vtkImageAccumulate> histogram = vtkSmartPointer<vtkImageAccumulate>::New();
    histogram->SetInputData(reader->GetOutput());
    histogram->SetComponentExtent(0, bins-1, 0, 0, 0, 0);   //bins-1指的是长方形序号最大值,确定的写为bins-1
    histogram->SetComponentOrigin(0, 0, 0); //灰度从0开始计算
    histogram->SetComponentSpacing(256.0/bins, 0, 0); // 256.0/bins每个长方形表示的灰度范围的长度
    histogram->Update();

    int* output = static_cast<int*>(histogram->GetOutput()->GetScalarPointer());

    vtkSmartPointer<vtkIntArray> frequencies = vtkSmartPointer<vtkIntArray>::New();
    frequencies->SetNumberOfComponents(1);

    for(int j = 0; j < bins; ++j)
    {
        for(int i=0; i<comps; i++)
        {
            frequencies->InsertNextTuple1(*output++);
        }
    }

    vtkSmartPointer<vtkDataObject> dataObject = vtkSmartPointer<vtkDataObject>::New();
    dataObject->GetFieldData()->AddArray( frequencies );

    vtkSmartPointer<vtkBarChartActor> barChart = vtkSmartPointer<vtkBarChartActor>::New();
    barChart->SetInput(dataObject);
    barChart->SetTitle("Histogram");
    barChart->GetPositionCoordinate()->SetValue(0.05,0.05,0.0);
    barChart->GetPosition2Coordinate()->SetValue(0.95,0.95,0.0);
    barChart->GetProperty()->SetColor(0,0,0);
    barChart->GetTitleTextProperty()->SetColor(0,0,0);
    barChart->GetLabelTextProperty()->SetColor(0,0,0);
    barChart->GetLegendActor()->SetNumberOfEntries(dataObject->GetFieldData()->GetArray(0)->GetNumberOfTuples());
    barChart->LegendVisibilityOff();
    barChart->LabelVisibilityOff();

    double colors[3][3] = {
        { 1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, 1 } };

    int count = 0;
    for( int i = 0; i < bins; ++i )
    {
        for( int j = 0; j < comps; ++j )
        {
            barChart->SetBarColor( count++, colors[j] );
        }
    }

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(barChart);
    renderer->SetBackground(1.0, 1.0, 1.0);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(640, 480);
    renderWindow->Render();
    renderWindow->SetWindowName("ImageAccumulateExample");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);

    interactor->Initialize();
    interactor->Start();
}
