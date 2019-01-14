#ifndef POINTPICKERINTERACTORSTYLE_H
#define POINTPICKERINTERACTORSTYLE_H

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPointPicker.h>
#include <vtkRendererCollection.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>  //vtkStandardNewMacro();
#include <vtkProperty.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>

class PointPickerInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static PointPickerInteractorStyle* New(); //如果使用{return new PointPickerInteractorStyle;的话，就不需要宏vtkStandardNewMacro了}
    //vtkTypeMacro(PointPickerInteractorStyle, vtkInteractorStyleTrackballCamera); //貌似不需要也可以

    virtual void OnLeftButtonDown() override
    {
        //打印鼠标左键像素位置
        std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
            << " " << this->Interactor->GetEventPosition()[1] << std::endl;
        //注册拾取点函数
        this->Interactor->GetPicker()->Pick(
            this->Interactor->GetEventPosition()[0],
            this->Interactor->GetEventPosition()[1], 0,  // always zero.
            this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
            );
        //打印拾取点空间位置
        double picked[3];
        this->Interactor->GetPicker()->GetPickPosition(picked);
        std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
        //对拾取点进行标记
        vtkSmartPointer<vtkSphereSource> sphereSource =
            vtkSmartPointer<vtkSphereSource>::New();
        sphereSource->Update();

        vtkSmartPointer<vtkPolyDataMapper> mapper =
            vtkSmartPointer<vtkPolyDataMapper>::New();
        mapper->SetInputConnection(sphereSource->GetOutputPort());

        vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
        actor->SetMapper(mapper);
        actor->SetPosition(picked);
        actor->SetScale(0.05);
        actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);

        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();   //转发事件  交互事件貌似都需要这样
    }
};
#endif // POINTPICKERINTERACTORSTYLE_H
