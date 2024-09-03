/**
 * \file   VectorField.h
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

#ifndef MMPPT_TOY_QT_VTK_EX005_VECTOR_FIELD_H_
#define MMPPT_TOY_QT_VTK_EX005_VECTOR_FIELD_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <vtkActor.h>
#include <vtkArrayCalculator.h>
#include <vtkArrowSource.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkElevationFilter.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkGradientFilter.h>
#include <vtkLookupTable.h>
#include <vtkMaskPoints.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkContourGrid.h>

#include "field.hpp"
#include "mesh.hpp"

class VectorField {

public:

  VectorField(const std::string& file, double arrowScale);

  [[nodiscard]] static std::vector<std::string>
  split(const std::string &s, char delim);

  [[nodiscard]] static std::string
  tail(std::string const& source, size_t length) ;

  std::string displayName();

  [[nodiscard]] std::string name() const { return mName; }

  [[nodiscard]] std::string nameIndex() const;
  
  [[nodiscard]] double hmin() const { return mHmin; }

  [[nodiscard]] double hmax() const { return mHmax; }

  [[nodiscard]] double hmid() const { return (mHmin+mHmax)/2.0; }

  [[nodiscard]] double mx() const { return mMx; }

  [[nodiscard]] double my() const { return mMy; }

  [[nodiscard]] double mz() const { return mMz; }

  [[nodiscard]] double mmag() const { return mMmag; }

  [[nodiscard]] std::string handedness() const {

    double hm = hmid();
    if (hm < 0.0) {
      return "Left";
    } else {
      return "Right";
    }

  }

  void setArrowLut(const vtkSmartPointer<vtkLookupTable>& arrowLut) {

    mArrowGlyphPolyDataMapper->SetLookupTable(arrowLut);
    mArrowGlyphPolyDataMapper->SetScalarRange(mHmin, mHmax);
    mArrowGlyphPolyDataMapper->Update();

  }

  vtkSmartPointer<vtkActor> arrows() { return mArrowActor; }
  vtkSmartPointer<vtkActor> geometry() { return mGeometryActor; }
  vtkSmartPointer<vtkActor> isosurface() { return mIsosurfaceActor; }

  void setArrowScale(double arrowScale) {

    if (fabs(mArrowScale - arrowScale) > 1E-9) {
      mArrowScale = arrowScale; 
      mArrowGlyph->SetScaleFactor(mArrowScale);
      mArrowGlyph->Update();
    }

  }

  void setIsosurfaceHelicity(double helicity);

  [[nodiscard]] double isoSurfaceHelicity() const { return mIsosurfaceHelicity; }

private:

  std::string mName;
  double mHmin;
  double mHmax;
  double mMx{};
  double mMy{};
  double mMz{};
  double mMmag{};

  double mArrowScale;

  vtkSmartPointer<vtkUnstructuredGrid> mUGrid;

  vtkSmartPointer<vtkDataSetMapper> mGeometryDataMapper;
  vtkSmartPointer<vtkActor> mGeometryActor;

  vtkSmartPointer<vtkArrowSource> mArrowSource;
  vtkSmartPointer<vtkTransform> mArrowTransform;
  vtkSmartPointer<vtkTransformPolyDataFilter> mArrowTransformFilter;
  vtkSmartPointer<vtkGlyph3D> mArrowGlyph;
  vtkSmartPointer<vtkPolyDataMapper> mArrowGlyphPolyDataMapper;
  vtkSmartPointer<vtkActor> mArrowActor;

  double mIsosurfaceHelicity{};
  vtkSmartPointer<vtkContourGrid> mIsosurface;
  vtkSmartPointer<vtkPolyDataMapper> mIsosurfacePolyDataMapper;
  vtkSmartPointer<vtkActor> mIsosurfaceActor;

  void setGrid(const Mesh &mesh);

  void setMagnetisation(const Field& field);

  void setHelicity();

  void setGeometry();

  void setArrows();

  void setIsosurface();

};

#endif  // MMPPT_TOY_QT_VTK_EX005_VECTOR_FIELD_H_
