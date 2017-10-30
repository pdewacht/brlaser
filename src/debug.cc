// This file is part of the brlaser printer driver.
//
// Copyright 2013 Peter De Wachter
//
// brlaser is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// brlaser is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with brlaser.  If not, see <http://www.gnu.org/licenses/>.

#include "config.h"
#include "debug.h"
#include <iostream>
#include <typeinfo>

namespace {

template <typename T>
void dump(const char *name, const T &value) {
  std::cerr << "DEBUG: " PACKAGE ": page header: " << name << " = " << value << '\n';
}

template <typename T, int N>
void dump(const char *name, const T (&value)[N]) {
  std::cerr << "DEBUG: " PACKAGE ": page header: " << name << " =";
  for (int i = 0; i < N; ++i) {
    std::cerr << ' ' << value[i];
  }
  std::cerr << '\n';
}

void dump(const char *name, const char *value) {
  std::cerr << "DEBUG: " PACKAGE ": page header: " << name << " = \"" << value << "\"\n";
}

template <int N, int M>
void dump(const char *name, const char (&value)[N][M]) {
  std::cerr << "DEBUG: " PACKAGE ": page header: " << name << " =";
  for (int i = 0; i < N; ++i) {
    std::cerr << " \"" << value[i] << '"';
  }
  std::cerr << '\n';
}

}  // namespace


void dump_page_header(const cups_page_header2_t &h) {
#define d(f) dump(#f, h.f)
  d(MediaClass);
  d(MediaColor);
  d(MediaType);
  d(OutputType);
  d(AdvanceDistance);
  d(AdvanceMedia);
  d(Collate);
  d(CutMedia);
  d(Duplex);
  d(HWResolution);
  d(ImagingBoundingBox);
  d(InsertSheet);
  d(Jog);
  d(LeadingEdge);
  d(Margins);
  d(ManualFeed);
  d(MediaPosition);
  d(MediaWeight);
  d(MirrorPrint);
  d(NegativePrint);
  d(NumCopies);
  d(Orientation);
  d(OutputFaceUp);
  d(PageSize);
  d(Separations);
  d(TraySwitch);
  d(Tumble);
  d(cupsWidth);
  d(cupsHeight);
  d(cupsMediaType);
  d(cupsBitsPerColor);
  d(cupsBitsPerPixel);
  d(cupsBytesPerLine);
  d(cupsColorOrder);
  d(cupsColorSpace);
  d(cupsCompression);
  d(cupsRowCount);
  d(cupsRowFeed);
  d(cupsRowStep);
  d(cupsNumColors);
  d(cupsBorderlessScalingFactor);
  d(cupsPageSize);
  d(cupsImagingBBox);
  d(cupsInteger);
  d(cupsReal);
  d(cupsString);
  d(cupsMarkerType);
  d(cupsRenderingIntent);
  d(cupsPageSizeName);
#undef d
}
