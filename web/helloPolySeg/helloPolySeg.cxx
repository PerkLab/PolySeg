/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See https://opensource.org/licenses/BSD-2-Clause for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/
#include <iostream>
#include "itkImage.h"

// VTK includes
#include "vtkNew.h"
#include "vtkVersion.h"
#include "vtkPolyData.h"
#include "vtkSphereSource.h"
#include "vtkMatrix4x4.h"
// Not sure why: fatal error: 'vtkImageAccumulate.h' file not found
//#include "vtkImageAccumulate.h"
//#include "vtkImageStencil.h"

// SegmentationCore includes
#include "vtkSegmentation.h"
#include "vtkSegment.h"
#include "vtkSegmentationConverter.h"
#include "vtkOrientedImageData.h"
#include "vtkSegmentationConverterFactory.h"
#include "vtkBinaryLabelmapToClosedSurfaceConversionRule.h"
#include "vtkClosedSurfaceToBinaryLabelmapConversionRule.h"

void CreateSpherePolyData(vtkPolyData* polyData);
void CreateCubeLabelmap(vtkOrientedImageData* imageData);

//----------------------------------------------------------------------------
int main(int vtkNotUsed(argc), char* vtkNotUsed(argv)[])
{
  using ImageType = itk::Image< unsigned short, 3 >;
  ImageType::Pointer image = ImageType::New();


  // TODO: For some reason uncommenting these gives pthread errors
  // Register converter rules
  vtkSegmentationConverterFactory::GetInstance()->RegisterConverterRule(
    vtkSmartPointer<vtkBinaryLabelmapToClosedSurfaceConversionRule>::New() );
  vtkSegmentationConverterFactory::GetInstance()->RegisterConverterRule(
    vtkSmartPointer<vtkClosedSurfaceToBinaryLabelmapConversionRule>::New() );

  //////////////////////////////////////////////////////////////////////////
  // Create segmentation with one segment from model and test segment
  // operations and conversion to labelmap with and without reference geometry

  // Generate sphere model
  vtkNew<vtkPolyData> spherePolyData;
  CreateSpherePolyData(spherePolyData.GetPointer());

  std::cout << "ITK Hello World!" << std::endl;
  std::cout << spherePolyData->GetNumberOfPolys() << std::endl;

  // Create segment
  vtkNew<vtkSegment> sphereSegment;
  sphereSegment->SetName("sphere1");
  sphereSegment->AddRepresentation(
    vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName(), spherePolyData.GetPointer());
  if (!sphereSegment->GetRepresentation(vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName()))
    {
    std::cerr << __LINE__ << ": Failed to add closed surface representation to segment!" << std::endl;
    return EXIT_FAILURE;
    }

  // Create segmentation with segment
  vtkNew<vtkSegmentation> sphereSegmentation;
  sphereSegmentation->SetMasterRepresentationName(
    vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName() );
  sphereSegmentation->AddSegment(sphereSegment.GetPointer());
  if (sphereSegmentation->GetNumberOfSegments() != 1)
    {
    std::cerr << __LINE__ << ": Failed to add segment to segmentation!" << std::endl;
    return EXIT_FAILURE;
    }

  // Convert to binary labelmap without reference geometry
  sphereSegmentation->CreateRepresentation(vtkSegmentationConverter::GetSegmentationBinaryLabelmapRepresentationName());
  vtkOrientedImageData* defaultImageData = vtkOrientedImageData::SafeDownCast(
    sphereSegment->GetRepresentation(vtkSegmentationConverter::GetSegmentationBinaryLabelmapRepresentationName()) );
  if (!defaultImageData)
    {
    std::cerr << __LINE__ << ": Failed to convert closed surface representation to binary labelmap without reference geometry!" << std::endl;
    return EXIT_FAILURE;
    }
  std::string defaultGeometryString = vtkSegmentationConverter::SerializeImageGeometry(defaultImageData);
  if (defaultGeometryString.compare("1;0;0;20.7521629333496;0;1;0;20.7521629333496;0;0;1;20;0;0;0;1;0;59;0;59;0;60;"))
    {
    std::cerr << __LINE__ << ": Default reference geometry mismatch!" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << sphereSegmentation->GetNumberOfSegments() << std::endl;
  std::cout << "Segmentation test passed." << std::endl;
  return EXIT_SUCCESS;
}

//----------------------------------------------------------------------------
void CreateSpherePolyData(vtkPolyData* polyData)
{
  if (!polyData)
    {
    return;
    }

  vtkNew<vtkSphereSource> sphere;
  sphere->SetCenter(50,50,50);
  sphere->SetRadius(30);
  sphere->Update();
  polyData->DeepCopy(sphere->GetOutput());
}

//----------------------------------------------------------------------------
void CreateCubeLabelmap(vtkOrientedImageData* imageData)
{
  if (!imageData)
    {
    return;
    }

  unsigned int size = 100;

  // Create new one because by default the direction is identity, origin is zeros and spacing is ones
  vtkNew<vtkOrientedImageData> identityImageData;
  identityImageData->SetExtent(0,size-1,0,size,0,size-1);
  identityImageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

  unsigned char* imagePtr = (unsigned char*)identityImageData->GetScalarPointer();
  for (unsigned int x=0; x<size; ++x)
    {
    for (unsigned int y=0; y<size; ++y)
      {
      for (unsigned int z=0; z<size; ++z)
        {
        unsigned char* currentPtr = imagePtr + z*size*size + y*size + x;
        if (x>100/4 && x<size*3/4 && y>100/4 && y<size*3/4 && z>100/4 && z<size*3/4)
          {
          (*currentPtr) = 1;
          }
        else
          {
          (*currentPtr) = 0;
          }
        }
      }
    }

  imageData->DeepCopy(identityImageData.GetPointer());
}
