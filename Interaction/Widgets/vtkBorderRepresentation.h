/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkBorderRepresentation.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkBorderRepresentation
 * @brief   represent a vtkBorderWidget
 *
 * This class is used to represent and render a vtBorderWidget. To
 * use this class, you need to specify the two corners of a rectangular
 * region.
 *
 * The class is typically subclassed so that specialized representations can
 * be created.  The class defines an API and a default implementation that
 * the vtkBorderRepresentation interacts with to render itself in the scene.
 *
 * @warning
 * The separation of the widget event handling (e.g., vtkBorderWidget) from
 * the representation (vtkBorderRepresentation) enables users and developers
 * to create new appearances for the widget. It also facilitates parallel
 * processing, where the client application handles events, and remote
 * representations of the widget are slaves to the client (and do not handle
 * events).
 *
 * @sa
 * vtkBorderWidget vtkTextWidget
 */

#ifndef vtkBorderRepresentation_h
#define vtkBorderRepresentation_h

#include "vtkCoordinate.h"               //Because of the viewport coordinate macro
#include "vtkInteractionWidgetsModule.h" // For export macro
#include "vtkWidgetRepresentation.h"

#include "vtkNew.h"

class vtkPoints;
class vtkPolyData;
class vtkTransform;
class vtkTransformPolyDataFilter;
class vtkPolyDataMapper2D;
class vtkActor2D;
class vtkProperty2D;
class vtkCellArray;

class VTKINTERACTIONWIDGETS_EXPORT vtkBorderRepresentation : public vtkWidgetRepresentation
{
public:
  /**
   * Instantiate this class.
   */
  static vtkBorderRepresentation* New();

  //@{
  /**
   * Define standard methods.
   */
  vtkTypeMacro(vtkBorderRepresentation, vtkWidgetRepresentation);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  //@}

  //@{
  /**
   * Specify opposite corners of the box defining the boundary of the
   * widget. By default, these coordinates are in the normalized viewport
   * coordinate system, with Position the lower left of the outline, and
   * Position2 relative to Position. Note that using these methods are
   * affected by the ProportionalResize flag. That is, if the aspect ratio of
   * the representation is to be preserved (e.g., ProportionalResize is on),
   * then the rectangle (Position,Position2) is a bounding rectangle.
   */
  vtkViewportCoordinateMacro(Position);
  vtkViewportCoordinateMacro(Position2);
  //@}

  enum
  {
    BORDER_OFF = 0,
    BORDER_ON,
    BORDER_ACTIVE
  };

  //@{
  /**
   * Specify when and if the border should appear. If ShowBorder is "on",
   * then the border will always appear. If ShowBorder is "off" then the
   * border will never appear.  If ShowBorder is "active" then the border
   * will appear when the mouse pointer enters the region bounded by the
   * border widget.
   * This method is provided as conveniency to set both horizontal and
   * vertical borders.
   * BORDER_ON by default.
   * See Also: SetShowHorizontalBorder(), SetShowVerticalBorder()
   */
  virtual void SetShowBorder(int border);
  virtual int GetShowBorderMinValue();
  virtual int GetShowBorderMaxValue();
  virtual int GetShowBorder();
  void SetShowBorderToOff() { this->SetShowBorder(BORDER_OFF); }
  void SetShowBorderToOn() { this->SetShowBorder(BORDER_ON); }
  void SetShowBorderToActive() { this->SetShowBorder(BORDER_ACTIVE); }
  //@}

  //@{
  /**
   * Specify when and if the vertical border should appear.
   * See Also: SetShowBorder(), SetShowHorizontalBorder()
   */
  vtkSetClampMacro(ShowVerticalBorder, int, BORDER_OFF, BORDER_ACTIVE);
  vtkGetMacro(ShowVerticalBorder, int);
  //@}

  //@{
  /**
   * Specify when and if the horizontal border should appear.
   * See Also: SetShowBorder(), SetShowVerticalBorder()
   */
  vtkSetClampMacro(ShowHorizontalBorder, int, BORDER_OFF, BORDER_ACTIVE);
  vtkGetMacro(ShowHorizontalBorder, int);
  //@}

  //@{
  /**
   * Specify the properties of the border.
   */
  vtkGetObjectMacro(BorderProperty, vtkProperty2D);
  //@}

  //@{
  /**
   * Indicate whether resizing operations should keep the x-y directions
   * proportional to one another. Also, if ProportionalResize is on, then
   * the rectangle (Position,Position2) is a bounding rectangle, and the
   * representation will be placed in the rectangle in such a way as to
   * preserve the aspect ratio of the representation.
   */
  vtkSetMacro(ProportionalResize, vtkTypeBool);
  vtkGetMacro(ProportionalResize, vtkTypeBool);
  vtkBooleanMacro(ProportionalResize, vtkTypeBool);
  //@}

  //@{
  /**
   * Specify a minimum and/or maximum size (in pixels) that this representation
   * can take. These methods require two values: size values in the x and y
   * directions, respectively.
   */
  vtkSetVector2Macro(MinimumSize, int);
  vtkGetVector2Macro(MinimumSize, int);
  vtkSetVector2Macro(MaximumSize, int);
  vtkGetVector2Macro(MaximumSize, int);
  //@}

  //@{
  /**
   * The tolerance representing the distance to the widget (in pixels)
   * in which the cursor is considered to be on the widget, or on a
   * widget feature (e.g., a corner point or edge).
   */
  vtkSetClampMacro(Tolerance, int, 1, 10);
  vtkGetMacro(Tolerance, int);
  //@}

  //@{
  /**
   * After a selection event within the region interior to the border; the
   * normalized selection coordinates may be obtained.
   */
  vtkGetVectorMacro(SelectionPoint, double, 2);
  //@}

  //@{
  /**
   * This is a modifier of the interaction state. When set, widget interaction
   * allows the border (and stuff inside of it) to be translated with mouse
   * motion.
   */
  vtkSetMacro(Moving, vtkTypeBool);
  vtkGetMacro(Moving, vtkTypeBool);
  vtkBooleanMacro(Moving, vtkTypeBool);
  //@}

  /**
   * Define the various states that the representation can be in.
   */
  enum _InteractionState
  {
    Outside = 0,
    Inside,
    AdjustingP0,
    AdjustingP1,
    AdjustingP2,
    AdjustingP3,
    AdjustingE0,
    AdjustingE1,
    AdjustingE2,
    AdjustingE3
  };
  vtkSetClampMacro(InteractionState, int, 0, AdjustingE3);

  /**
   * Return the MTime of this object. It takes into account MTimes
   * of position coordinates and border's property.
   */
  vtkMTimeType GetMTime() override;

  //@{
  /**
   * Subclasses should implement these methods. See the superclasses'
   * documentation for more information.
   */
  void BuildRepresentation() override;
  void StartWidgetInteraction(double eventPos[2]) override;
  void WidgetInteraction(double eventPos[2]) override;
  virtual void GetSize(double size[2])
  {
    size[0] = 1.0;
    size[1] = 1.0;
  }
  int ComputeInteractionState(int X, int Y, int modify = 0) override;
  //@}

  //@{
  /**
   * These methods are necessary to make this representation behave as
   * a vtkProp.
   */
  void GetActors2D(vtkPropCollection*) override;
  void ReleaseGraphicsResources(vtkWindow*) override;
  int RenderOverlay(vtkViewport*) override;
  int RenderOpaqueGeometry(vtkViewport*) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport*) override;
  vtkTypeBool HasTranslucentPolygonalGeometry() override;
  //@}

  void SetBWActorDisplayOverlay(bool);

  //@{
  /**
   * Set/Get the RGB color of the border.
   * Default is white (1.0, 1.0, 1.0).
   */
  vtkSetVector3Macro(BorderColor, double);
  vtkGetVector3Macro(BorderColor, double);
  //@}

  //@{
  /**
   * Set/Get the thickness of the border in screen units.
   * Default is 1.0.
   */
  vtkSetClampMacro(BorderThickness, float, 0, VTK_FLOAT_MAX);
  vtkGetMacro(BorderThickness, float);
  //@}

  //@{
  /**
   * Set/Get the ratio between no radius and maximum radius.
   * In order to compute round corners, we create 2 points
   * in each side of the corner. The maximum radius is then
   * the minimum length of the two sides of each corners.
   * This maximum radius is scaled by the CornerRadiusStrength.
   * Default is 0.0 (no radius).
   */
  vtkSetClampMacro(CornerRadiusStrength, double, 0.0, 1.0);
  vtkGetMacro(CornerRadiusStrength, double);
  //@}

  //@{
  /**
   * Set/Get the number of points that define each round corners.
   * A high value increase the resolution of the corners.
   * Default is 20.
   */
  vtkSetClampMacro(CornerResolution, int, 0, 1000);
  vtkGetMacro(CornerResolution, int);
  //@}

  //@{
  /**
   * Set/Get the RGB color of the background polygon.
   * Default is white (1.0, 1.0, 1.0).
   */
  vtkSetVector3Macro(PolygonColor, double);
  vtkGetVector3Macro(PolygonColor, double);
  //@}

  //@{
  /**
   * Set/Get the opacity of the background color.
   * Default is 0.0.
   */
  vtkSetClampMacro(PolygonOpacity, double, 0.0, 1.0);
  vtkGetMacro(PolygonOpacity, double);
  //@}

protected:
  vtkBorderRepresentation();
  ~vtkBorderRepresentation() override;

  // Ivars
  int ShowVerticalBorder;
  int ShowHorizontalBorder;
  vtkNew<vtkProperty2D> BorderProperty;
  vtkNew<vtkProperty2D> PolygonProperty;
  vtkTypeBool ProportionalResize;
  int Tolerance;
  vtkTypeBool Moving;
  double SelectionPoint[2];

  // Layout (position of lower left and upper right corners of border)
  vtkNew<vtkCoordinate> PositionCoordinate;
  vtkNew<vtkCoordinate> Position2Coordinate;

  // Sometimes subclasses must negotiate with their superclasses
  // to achieve the correct layout.
  int Negotiated;
  virtual void NegotiateLayout();

  // Update the border visibility based on InteractionState.
  // See Also: SetShowHorizontalBorder(), SetShowHorizontalBorder(),
  // ComputeInteractionState()
  virtual void UpdateShowBorder();

  // Keep track of start position when moving border
  double StartPosition[2];

  // Border representation. Subclasses may use the BWTransform class
  // to transform their geometry into the region surrounded by the border.
  vtkNew<vtkPoints> BWPoints;
  vtkNew<vtkPolyData> BWPolyData;
  vtkNew<vtkPolyData> PolyDataEdges;
  vtkNew<vtkPolyData> PolyDataPolygon;
  vtkNew<vtkTransform> BWTransform;
  vtkNew<vtkTransformPolyDataFilter> BWTransformFilter;
  vtkNew<vtkPolyDataMapper2D> BWMapperEdges;
  vtkNew<vtkPolyDataMapper2D> BWMapperPolygon;
  vtkNew<vtkActor2D> BWActorEdges;
  vtkNew<vtkActor2D> BWActorPolygon;

  // Constraints on size
  int MinimumSize[2];
  int MaximumSize[2];

  // Properties of the border
  double BorderColor[3] = { 1.0, 1.0, 1.0 };
  float BorderThickness = 1.0;
  double CornerRadiusStrength = 0.0;
  int CornerResolution = 20;

  // Properties of the inner polygon (ie. the background)
  double PolygonColor[3] = { 1.0, 1.0, 1.0 };
  double PolygonOpacity = 0.0;

  /**
   * Create all 4 round corners with the specified radius and resolution.
   */
  void ComputeRoundCorners();

  /**
   * Create a quarter circle centered in point[idCenterX].x, point[idCenterY].y),
   * of radius 'radius' with a starting angle 'startAngle' ending in
   * 'startAngle + PI/2' with CornerResolution number of points.
   * Computed points are stored in the vtkPoints 'points' and
   * inserted in the vtkCellArray 'polys'
   */
  void ComputeOneRoundCorner(vtkCellArray* polys, vtkPoints* points, const double radius,
    vtkIdType xPt, vtkIdType yPt, const double startAngle);

private:
  vtkBorderRepresentation(const vtkBorderRepresentation&) = delete;
  void operator=(const vtkBorderRepresentation&) = delete;
};

#endif
