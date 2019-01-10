#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkDijkstraGraphGeodesicPath.h>
#include <vtkCubeSource.h>
#include <vtkTriangleFilter.h>
#include <vtkMassProperties.h>
#include <QDebug>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataNormals.h>
#include <vtkMaskPoints.h>
#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>
#include <vtkPointData.h>
#include <vtkCurvatures.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void triangleSurfaceAndVolume();
    void distanceOnSurface();
    void boundingBox();
    void normalVector();
    void curvatures();
};

#endif // MAINWINDOW_H
