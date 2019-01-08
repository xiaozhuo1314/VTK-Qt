#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createImageData();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createImageData()
{
    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    vtkSmartPointer<vtkInformation> info = vtkSmartPointer<vtkInformation>::New();
    imageData->SetDimensions(16, 16, 1);  //指定维数
    imageData->SetScalarType(VTK_UNSIGNED_CHAR, info);   //图像数据类型
    imageData->SetNumberOfScalarComponents(1, info);   //每个像素需要表示的组份 =1是指标量图
    imageData->AllocateScalars(info);  //分配内存生成图像数据，默认所有像素值为0

    unsigned char *ptr = (unsigned char*) imageData->GetScalarPointer();
    for(int i=0;i<16*16*1;i++){
        *ptr = i%256;
        ptr++;
    }
    //actor
    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInputData(imageData);
    double viewport[4] = {0,0,1,1};
    //render
    vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
    render->AddActor(actor);
    render->SetViewport(viewport);  //四个数为min x, min y, max x, max y,取之范围为0-1
    render->ResetCamera();
    render->SetBackground(1,1,1);
    //render window
    vtkSmartPointer<vtkRenderWindow> renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(render);
    renWin->SetWindowName("haha");
    renWin->SetSize(640, 480);
    renWin->Render();
    //render window interactor
    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyle> style = vtkSmartPointer<vtkInteractorStyle>::New();
    interactor->SetRenderWindow(renWin);
    interactor->SetInteractorStyle(style);
    interactor->Initialize();
    interactor->Start();
}
