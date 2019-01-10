#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPolygon.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void polyDataColor();
};

#endif // MAINWINDOW_H
