#include "mainwindow.h"
vtkStandardNewMacro(PointPickerInteractorStyle);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    pointePicker();
}

MainWindow::~MainWindow()
{

}

void MainWindow::pointePicker()
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(actor);
    renderer->SetBackground(1, 1, 1);

    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->Render();
    renderWindow->SetWindowName("PointPicker");
    renderWindow->AddRenderer(renderer);

    vtkSmartPointer<vtkPointPicker> pointPicker = vtkSmartPointer<vtkPointPicker>::New();

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetPicker(pointPicker);
    renderWindowInteractor->SetRenderWindow(renderWindow);

    vtkSmartPointer<PointPickerInteractorStyle> style = vtkSmartPointer<PointPickerInteractorStyle>::New();
    renderWindowInteractor->SetInteractorStyle(style);

    vtkSmartPointer<vtkAxesActor>  Axes = vtkSmartPointer<vtkAxesActor>::New();
    vtkSmartPointer<vtkOrientationMarkerWidget>  widget =vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetInteractor(renderWindowInteractor);
    widget->SetOrientationMarker(Axes);
    widget->SetOutlineColor(1, 1, 1);
    widget->SetViewport(0, 0, 0.2, 0.2);
    widget->SetEnabled(1);
    widget->InteractiveOn();

    renderWindow->Render();
    renderWindowInteractor->Start();
}
