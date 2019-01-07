#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    import3DS();
}

MainWindow::~MainWindow()
{

}

void MainWindow::import3DS()
{
    vtkSmartPointer<vtk3DSImporter> importer = vtkSmartPointer<vtk3DSImporter>::New();
    importer->SetFileName("/home/silence/Project/VTK-Qt/import3DS/R8.3ds");
    importer->ComputeNormalsOn();  //打开法向量计算功能
    importer->Read();

    vtkSmartPointer<vtkRenderer> renderer = importer->GetRenderer();
    vtkSmartPointer<vtkRenderWindow> renWin = importer->GetRenderWindow();
    renWin->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    renderer->GradientBackgroundOn();  //背景颜色梯度变化
    renderer->SetBackground(1.0, 1.0, 1.0);  //背景颜色1
    renderer->SetBackground2(0.0, 0.0, 0.0);  //背景颜色2

    renWin->Render();
    renWin->SetSize(640, 480);
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);
    interactor->Initialize();
    interactor->Start();
}
