#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkCubeSource.h>
#include <vtkConeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkProgrammableGlyphFilter.h>
#include <vtkCellArray.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkFloatArray.h>
 
void CalcGlyph(void *arg)
{
  
  vtkProgrammableGlyphFilter *glyphFilter = (vtkProgrammableGlyphFilter*) arg;
  
  if(!glyphFilter)
    {
    std::cerr << "glyphFilter is not valid!" << std::endl;
    }
  double pointCoords[3];
  glyphFilter->GetPoint(pointCoords);
  
  std::cout << "Calling CalcGlyph for point " << glyphFilter->GetPointId() << std::endl;
  std::cout << "Point corods are: " << pointCoords[0] << " " << pointCoords[1] << " " << pointCoords[2] << std::endl;
  if(glyphFilter->GetPointId() == 0)
    {
    vtkSmartPointer<vtkConeSource> coneSource = 
      vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetCenter(pointCoords);
    coneSource->Update();
    glyphFilter->SetSource(coneSource->GetOutput());
    }
  else if(glyphFilter->GetPointId() == 1)
    {
    vtkSmartPointer<vtkCubeSource> cubeSource = 
      vtkSmartPointer<vtkCubeSource>::New();
    cubeSource->SetCenter(pointCoords);
    cubeSource->Update();
    glyphFilter->SetSource(cubeSource->GetOutput());
    }
  else if(glyphFilter->GetPointId() == 2)
    {
    vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetCenter(pointCoords);
    sphereSource->Update();
    glyphFilter->SetSource(sphereSource->GetOutput());
    }
}

int main(int, char *[])
{
  //create points
  vtkSmartPointer<vtkPoints> points = 
      vtkSmartPointer<vtkPoints>::New();
  points->InsertNextPoint(0,0,0);
  points->InsertNextPoint(5,0,0);
  points->InsertNextPoint(10,0,0);
 
  //combine into a polydata
  vtkSmartPointer<vtkPolyData> polydata = 
      vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);
  
  vtkSmartPointer<vtkProgrammableGlyphFilter> glyphFilter = 
      vtkSmartPointer<vtkProgrammableGlyphFilter>::New();
  glyphFilter->SetInput(polydata);
  glyphFilter->SetGlyphMethod(CalcGlyph, glyphFilter);
  //need a default glyph, but this should not be used
  vtkSmartPointer<vtkConeSource> coneSource = 
    vtkSmartPointer<vtkConeSource>::New();
  glyphFilter->SetSource(coneSource->GetOutput());
    
  //Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyphFilter->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  //Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1); // Background color white
 
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}