// This file is part of the brlaser printer driver.
//
// Copyright 2014 Peter De Wachter
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

#ifndef TEMPFILE_H
#define TEMPFILE_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <vector>

class tempfile {
 public:
  explicit tempfile()
      : ptr_(0),
        size_(0),
        file_(open_memstream(&ptr_, &size_)) {
  }

  ~tempfile() {
    fclose(file_);
    free(ptr_);
  }

  FILE *file() {
    return file_;
  }

  std::vector<uint8_t> data() {
    if (fflush(file_))
      return std::vector<uint8_t>();
    return std::vector<uint8_t>(ptr_, ptr_ + size_);
  }

 private:
  char *ptr_;
  size_t size_;
  FILE *file_;
};

#endif  // TEMPFILE_H
