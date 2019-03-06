/*=========================================================================

Program:   Visualization Toolkit

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
* @class   vtkOpenVRRay
* @brief   OpenVR device model

* Represents a ray shooting from a VR controller, used for pointing or picking.
*/

#ifndef vtkOpenVRRay_h
#define vtkOpenVRRay_h

#include "vtkRenderingOpenVRModule.h" // For export macro
#include "vtkObject.h"
#include "vtkOpenGLHelper.h" // ivar
#include "vtkNew.h" // for ivar
#include <openvr.h> // for ivars

class vtkOpenGLRenderWindow;
class vtkRenderWindow;
class vtkOpenGLVertexBufferObject;
class vtkMatrix4x4;


class VTKRENDERINGOPENVR_EXPORT vtkOpenVRRay : public vtkObject
{
public:
  static vtkOpenVRRay *New();
  vtkTypeMacro(vtkOpenVRRay, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  bool Build(vtkOpenGLRenderWindow *win);
  void Render(vtkOpenGLRenderWindow *win,
    vtkMatrix4x4 *poseMatrix);

  // show the model
  void SetShow(bool v) {
    this->Show = v;
  };
  bool GetShow() {
    return this->Show;
  };

  void SetLength(float s) {
    this->Length = s;
  };

  void ReleaseGraphicsResources(vtkRenderWindow *win);

protected:
  vtkOpenVRRay();
  ~vtkOpenVRRay() override;

  bool Show;
  bool Loaded;

  vtkOpenGLHelper ModelHelper;
  vtkOpenGLVertexBufferObject *ModelVBO;
  vtkNew<vtkMatrix4x4> PoseMatrix;

  float Length;

private:
  vtkOpenVRRay(const vtkOpenVRRay&) = delete;
  void operator=(const vtkOpenVRRay&) = delete;
};

#endif