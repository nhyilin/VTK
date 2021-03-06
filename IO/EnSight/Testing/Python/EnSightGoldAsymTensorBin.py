#!/usr/bin/env python

from vtk import *
from vtk.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

rdr = vtk.vtkGenericEnSightReader()
rdr.SetCaseFileName(str(VTK_DATA_ROOT) + "/Data/EnSight/RESULT_FLUID_DOMAIN.case")
rdr.Update()

srf = vtkDataSetSurfaceFilter()
srf.SetInputConnection(rdr.GetOutputPort())
srf.Update()

aa = vtkAssignAttribute()
aa.SetInputConnection(srf.GetOutputPort())
aa.Assign('R_ns', vtkDataSetAttributes.SCALARS, vtkAssignAttribute.CELL_DATA)
aa.Update()

renWin = vtkRenderWindow()
ri = vtkRenderWindowInteractor()
rr = vtkRenderer()
rr.SetBackground(1, 1, 1)
ac = vtkActor()
dm = vtkCompositePolyDataMapper()
ac.SetMapper(dm)
dm.SetInputConnection(aa.GetOutputPort())
dm.SetScalarVisibility(True)
dm.SetScalarRange([0.001, 0.008])
dm.UseLookupTableScalarRangeOff()
rr.AddActor(ac)
renWin.AddRenderer(rr)
renWin.SetInteractor(ri)
rr.ResetCamera()
renWin.Render()
#ri.Start()
