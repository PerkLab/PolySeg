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
#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"

// VTK includes
#include "vtkPolyData.h"
#include "vtkImageData.h"
#include "vtkJSONDataSetWriter.h"
#include "vtkNew.h"
#include "vtkSphereSource.h"

// SegmentationCore includes
#include "vtkSegmentation.h"
#include "vtkSegment.h"
#include "vtkSegmentationConverter.h"
#include "vtkOrientedImageData.h"
#include "vtkSegmentationConverterFactory.h"
#include "vtkBinaryLabelmapToClosedSurfaceConversionRule.h"
#include "vtkClosedSurfaceToBinaryLabelmapConversionRule.h"

//----------------------------------------------------------------------------
int main( int argc, char * argv[] )
{
  std::cout << __LINE__ << argv << std::endl;
  std::cout << __LINE__ << argc << std::endl;
  if( argc < 2 )
  {
    std::cerr << "Usage: " << argv[0] << " <inputImage> <outputPolyData>" << std::endl;
    //return EXIT_FAILURE;
  }
  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;
  using ImageType = itk::Image< PixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< ImageType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( inputFilename );

  using FilterType = itk::ImageToVTKImageFilter< ImageType >;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  vtkSegmentationConverterFactory::GetInstance()->RegisterConverterRule(
    vtkSmartPointer<vtkBinaryLabelmapToClosedSurfaceConversionRule>::New() );
  vtkSegmentationConverterFactory::GetInstance()->RegisterConverterRule(
    vtkSmartPointer<vtkClosedSurfaceToBinaryLabelmapConversionRule>::New() );

  // Create new one because by default the direction is identity, origin is zeros and spacing is ones
  vtkImageData * myvtkImageData = filter->GetOutput();
  vtkNew<vtkOrientedImageData> imageData;
  //imageData->SetExtent(vtkImageData->GetExtent);
  imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
  imageData->DeepCopy(myvtkImageData);
  imageData->Print(std::cout);


  //imageData->SetExtent(0,size-1,0,size,0,size-1);
  //imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

  // Create a segment
  vtkNew<vtkSegment> segment;
  segment->SetName("inputImage");
  segment->AddRepresentation(
    vtkSegmentationConverter::GetSegmentationBinaryLabelmapRepresentationName(), imageData.GetPointer());
  if (!segment->GetRepresentation(vtkSegmentationConverter::GetSegmentationBinaryLabelmapRepresentationName()))
    {
    std::cerr << __LINE__ << ": Failed to add binary labelmap representation to segment!" << std::endl;
    return EXIT_FAILURE;
  }

  // Create segmentation with segment
  vtkNew<vtkSegmentation> segmentation;
  segmentation->SetMasterRepresentationName(
    vtkSegmentationConverter::GetSegmentationBinaryLabelmapRepresentationName() );
  segmentation->AddSegment(segment.GetPointer());
  if (segmentation->GetNumberOfSegments() != 1)
  {
    std::cerr << __LINE__ << ": Failed to add segment to segmentation!" << std::endl;
    return EXIT_FAILURE;
  }

  // Convert to closed surface model
  segmentation->CreateRepresentation(vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName());
  vtkPolyData* closedSurfaceModel = vtkPolyData::SafeDownCast(
    segment->GetRepresentation(vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName()) );
  if (!closedSurfaceModel)
  {
    std::cerr << __LINE__ << ": Failed to convert binary labelmap representation to closed surface model!" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "ClosedSurfaceModel: " << closedSurfaceModel->GetBounds() << std::endl;
  closedSurfaceModel->Print(std::cout);

  vtkNew<vtkSphereSource> sphere;
  sphere->SetCenter(0.0, 0.0, 0.0);
  sphere->SetRadius(5.0);

  // Create the output writer
  vtkNew<vtkJSONDataSetWriter> writer;
  writer->SetFileName(outputFilename);
  //writer->SetInputData(sphere);
  //writer->SetInputConnection(sphere->GetOutputPort());
  writer->SetInputData(closedSurfaceModel);

  try
    {
    writer->Update();
    std::cout << "Written to: " << outputFilename << std::endl;
    }
  catch( const std::exception & error )
    {
    std::cerr << "Error: " << error.what() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

