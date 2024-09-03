/**
 * \file   VectorField.cxx 
 * \author L. Nagy
 * 
 * MIT License
 *
 * Copyright (c) [2016] Lesleis Nagy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#include "vector_field.h"
#include "load_tecplot.hpp"

VectorField::VectorField(const std::string& file, double arrowScale):
  mName(file),
  mHmin(0.0),
  mHmax(0.0),
  mArrowScale(arrowScale) {

  Model model = TecplotFileLoader::read(file);

  setGrid(model.mesh());

  setMagnetisation(model.field_list().fields()[0]);

  setHelicity();

  setGeometry();

  setArrows();

  mUGrid->GetPointData()->SetActiveVectors("Magnetisation");
  mUGrid->GetPointData()->SetActiveScalars("Helicity");

  setIsosurface();

}

std::vector<std::string>
VectorField::split(const std::string & s, char delim) {

  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }

  return tokens;

}

std::string
VectorField::tail(std::string const& source, size_t const length) {

  if (length >= source.size()) { return source; }
  return source.substr(source.size() - length);

}

std::string
VectorField::displayName() {

  auto tokens = split(mName, '/');
  return tokens.back();

}

std::string
VectorField::nameIndex() const {

  std::string end = tail(mName, 13);
  return end.substr(0, 4);

}

void
VectorField::setIsosurfaceHelicity(double helicity) {

  mIsosurfaceHelicity = helicity;
  // TODO: implement rest

}

///////////////////////////////////////////////////////////////////////////////
// Private functions.
///////////////////////////////////////////////////////////////////////////////

void
VectorField::setGrid(const Mesh &mesh) {

  mUGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();

  const auto &vcl = mesh.vcl();
  const auto &til = mesh.til();
  
  // Add vertices to the unstructured grid.
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  points->SetNumberOfPoints((vtkIdType)vcl.size());
  for (size_t i = 0; i < vcl.size(); ++i) {
    points->SetPoint((vtkIdType)i, vcl[i][0], vcl[i][1], vcl[i][2]);
  }
  mUGrid->SetPoints(points);

  // Add connectivity information to unstructured grid.
  for (const auto & c : til) {
    vtkIdType element[4] = {
        (vtkIdType)c[0],
        (vtkIdType)c[1],
        (vtkIdType)c[2],
        (vtkIdType)c[3]
    };
    mUGrid->InsertNextCell(VTK_TETRA, 4, element);
  }

}

void
VectorField::setMagnetisation(const Field &field) {

  vtkSmartPointer<vtkDoubleArray> f = vtkSmartPointer<vtkDoubleArray>::New();

  const auto &vectors = field.vectors();

  f->SetName("Magnetisation");
  f->SetNumberOfComponents(3);
  f->SetNumberOfTuples((vtkIdType)vectors.size());

  mMx = 0.0;
  mMy = 0.0;
  mMz = 0.0;
  mMmag = 0.0;


  for (size_t i = 0; i < vectors.size(); ++i) {

    double fd[3] = {
        vectors[i][0],
        vectors[i][1] ,
        vectors[i][2]
    };

    f->SetTuple((vtkIdType)i, fd);

    mMx += fd[0];
    mMy += fd[1];
    mMz += fd[2];

  }

  mUGrid->GetPointData()->AddArray(f);

  mMx /= (double)vectors.size();
  mMy /= (double)vectors.size();
  mMz /= (double)vectors.size();

  mMmag = sqrt(mMx*mMx + mMy*mMy + mMz*mMz);

  mMx /= mMmag;
  mMy /= mMmag;
  mMz /= mMmag;

}

void
VectorField::setHelicity() {

  vtkSmartPointer<vtkGradientFilter> vorticity =
    vtkSmartPointer<vtkGradientFilter>::New();

  vtkSmartPointer<vtkArrayCalculator> helicity =
    vtkSmartPointer<vtkArrayCalculator>::New();

  // Vorticity.
  vorticity->ComputeVorticityOn();
  vorticity->SetInputArrayToProcess(0, 0, 0, 0, "Magnetisation");
  vorticity->SetVorticityArrayName("Vorticity");
  vorticity->SetInputData(mUGrid);
  vorticity->Update();

  // Helicity.
  helicity->SetAttributeTypeToPointData();
  helicity->AddVectorArrayName("Magnetisation");
  helicity->AddVectorArrayName("Vorticity");
  helicity->SetResultArrayName("Helicity");
  helicity->SetFunction("Magnetisation.Vorticity");
  helicity->SetInputData(vorticity->GetOutput());
  helicity->Update();

  // Add helicity field.
  vtkUnstructuredGrid * hug = 
    vtkUnstructuredGrid::SafeDownCast(helicity->GetOutput());

  vtkSmartPointer<vtkDoubleArray> h =
    vtkDoubleArray::SafeDownCast(hug->GetPointData()->GetArray("Helicity"));

  vtkSmartPointer<vtkDoubleArray> hd = vtkSmartPointer<vtkDoubleArray>::New();
  hd->SetName("Helicity");
  hd->SetNumberOfComponents(1);
  hd->SetNumberOfTuples(h->GetNumberOfTuples());

  double hmin = 1E12, hmax = -1E12;
  for (int i = 0; i < h->GetNumberOfTuples(); ++i) {
    hmin = (h->GetValue(i) < hmin) ? h->GetValue(i) : hmin;
    hmax = (h->GetValue(i) > hmax) ? h->GetValue(i) : hmax;
    hd->SetValue(i, h->GetTuple(i)[0]);
  }
  mUGrid->GetPointData()->AddArray(hd);
  mHmin = hmin;
  mHmax = hmax;

}

void
VectorField::setGeometry() {

  mGeometryDataMapper = vtkSmartPointer<vtkDataSetMapper>::New();
  mGeometryDataMapper->SetInputData(mUGrid);
  mGeometryDataMapper->ScalarVisibilityOff();

  mGeometryActor = vtkSmartPointer<vtkActor>::New();
  mGeometryActor->SetMapper(mGeometryDataMapper);

  mGeometryActor->GetProperty()->SetRepresentationToWireframe();
  mGeometryActor->GetProperty()->SetAmbient(1.0);
  mGeometryActor->GetProperty()->SetDiffuse(0.0);
  mGeometryActor->GetProperty()->SetSpecular(0.0);

}

void
VectorField::setArrows() {

  mArrowSource = vtkSmartPointer<vtkArrowSource>::New();
  mArrowSource->SetShaftResolution(10);
  mArrowSource->SetTipResolution(30);

  mArrowTransform = vtkSmartPointer<vtkTransform>::New();
  mArrowTransform->Translate(-0.5, 0.0, 0.0);

  mArrowTransformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
  mArrowTransformFilter->SetTransform(mArrowTransform);
  mArrowTransformFilter->SetInputConnection(mArrowSource->GetOutputPort());

  mArrowGlyph = vtkSmartPointer<vtkGlyph3D>::New();
  mArrowGlyph->SetInputData(mUGrid);
  mArrowGlyph->SetSourceConnection(mArrowTransformFilter->GetOutputPort());
  mArrowGlyph->SetScaleModeToScaleByVector();
  mArrowGlyph->SetVectorModeToUseVector();
  mArrowGlyph->SetColorModeToColorByScalar();
  mArrowGlyph->ScalingOn();
  mArrowGlyph->OrientOn();
  mArrowGlyph->SetScaleFactor(mArrowScale);
  mArrowGlyph->Update();

  mArrowGlyphPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mArrowGlyphPolyDataMapper->SetInputConnection(mArrowGlyph->GetOutputPort());
  mArrowGlyphPolyDataMapper->Update();

  mArrowActor = vtkSmartPointer<vtkActor>::New();
  mArrowActor->SetMapper(mArrowGlyphPolyDataMapper);

}

void
VectorField::setIsosurface() {

  double h = (mHmin+mHmax)/2;

  mIsosurface = vtkSmartPointer<vtkContourGrid>::New();
  mIsosurface->SetInputData(mUGrid);
  mIsosurface->SetValue(0, h);

  mIsosurfacePolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  mIsosurfacePolyDataMapper->ScalarVisibilityOff();
  mIsosurfacePolyDataMapper->SetInputData(mIsosurface->GetOutput());
  mIsosurfacePolyDataMapper->Update();

  mIsosurfaceActor = vtkSmartPointer<vtkActor>::New();
  mIsosurfaceActor->SetMapper(mIsosurfacePolyDataMapper);
  mIsosurfaceActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
  mIsosurfaceActor->Modified();

}


