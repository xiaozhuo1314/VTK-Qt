#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    changeImagePixel();
}

MainWindow::~MainWindow()
{

}

void MainWindow::changeImagePixel()
{
    vtkSmartPointer<vtkBMPReader> reader = vtkSmartPointer<vtkBMPReader>::New();
    reader->SetFileName("/home/silence/Project/VTK-Qt/changeImagePixel/lena.bmp");
    reader->Update();
    int subRegion[6] = {0,300, 0, 300, 0, 0};  //截取0-300正方形的数据
    vtkImageIterator<unsigned char> it(reader->GetOutput(), subRegion);
    while(!it.IsAtEnd())
    {
        unsigned char *inSI = it.BeginSpan();
        unsigned char *inSIEnd = it.EndSpan();

        while(inSI != inSIEnd)
        {
            *inSI = 255-*inSI;   //像素值取反
            ++inSI;
        }
        it.NextSpan();
    }

    vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
    imageViewer->SetInputConnection(reader->GetOutputPort());

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    imageViewer->SetupInteractor(renderWindowInteractor);
    imageViewer->Render();
    imageViewer->GetRenderer()->ResetCamera();
    imageViewer->Render();

    imageViewer->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
    imageViewer->SetSize(640, 480);
    imageViewer->GetRenderWindow()->SetWindowName("VisitImagePixelIterativelyExample");

    renderWindowInteractor->Start();

}
