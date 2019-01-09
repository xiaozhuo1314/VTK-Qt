#ifndef VTKIMAGEINTERACTIONCALLBACK_H
#define VTKIMAGEINTERACTIONCALLBACK_H

#include <vtkImageReslice.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCommand.h>
#include <QDebug>
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkInteractorStyleImage.h>

class vtkImageInteractionCallback : public vtkCommand
{
private:
    int Slicing;
    vtkImageReslice* ImageReslice;   //vtkImageSlice根据设置的变换矩阵提取三维图像切片。
    vtkRenderWindowInteractor* Interactor;  //vtkRenderWindowInteractor类对象负责每次提取切片后刷新视图。

public:
    static vtkImageInteractionCallback *New()
    {
        return new vtkImageInteractionCallback;
    }
    vtkImageInteractionCallback()
    {
        this->Slicing = 0;
        this->ImageReslice = 0;
        this->Interactor = 0;
    }
    ~vtkImageInteractionCallback(){}
    void SetImageReslice(vtkImageReslice *reslice)
    {
        this->ImageReslice = reslice;
    }
    vtkImageReslice *GetImageReslice()
    {
        return this->ImageReslice;
    }
    void SetInteractor(vtkRenderWindowInteractor *interactor)
    {
        this->Interactor = interactor;
    }
    vtkRenderWindowInteractor *GetInteractor()
    {
        return this->Interactor;
    }
    virtual void Execute(vtkObject *, unsigned long event, void *)  //继承重写vtkCommand的虚函数Execute
    {
        vtkRenderWindowInteractor *interactor = this->GetInteractor();

        int lastPos[2];
        interactor->GetLastEventPosition(lastPos);
        int currPos[2];
        interactor->GetEventPosition(currPos);

        if (event == vtkCommand::LeftButtonPressEvent)
        {
            this->Slicing = 1;
        }
        else if (event == vtkCommand::LeftButtonReleaseEvent)
        {
            this->Slicing = 0;
        }
        else if (event == vtkCommand::MouseMoveEvent)
        {
            if (this->Slicing)
            {
                vtkImageReslice *reslice = this->ImageReslice;

                // Increment slice position by deltaY of mouse
                int deltaY = lastPos[1] - currPos[1];

                reslice->Update();
                double sliceSpacing = reslice->GetOutput()->GetSpacing()[2];
                vtkMatrix4x4 *matrix = reslice->GetResliceAxes();
                // move the center point that we are slicing through
                double point[4];
                double center[4];
                point[0] = 0.0;
                point[1] = 0.0;
                point[2] = sliceSpacing * deltaY;
                point[3] = 1.0;
                matrix->MultiplyPoint(point, center);
                matrix->SetElement(0, 3, center[0]);
                matrix->SetElement(1, 3, center[1]);
                matrix->SetElement(2, 3, center[2]);

                interactor->Render();
            }
            else
            {
                vtkInteractorStyle *style = vtkInteractorStyle::SafeDownCast(
                    interactor->GetInteractorStyle());
                if (style)
                {
                    style->OnMouseMove();
                }
            }
        }
    }
};

#endif // VTKIMAGEINTERACTIONCALLBACK_H
