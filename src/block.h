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

#ifndef BLOCK_H
#define BLOCK_H

#include <assert.h>
#include <stdio.h>
#include <vector>

class block {
 public:
  block(): line_bytes_(0) {
    lines_.reserve(max_lines_per_block_);
  }

  bool empty() const {
    return line_bytes_ == 0;
  }

  void add_line(std::vector<uint8_t> &&line) {
    assert(!line.empty());
    assert(line_fits(line.size()));
    line_bytes_ += line.size();
    lines_.emplace_back(line);
  }

  bool line_fits(unsigned size) {
    return lines_.size() != max_lines_per_block_
      && line_bytes_ + size < max_block_size_;
  }

  void flush(FILE *f) {
    if (!empty()) {
      fprintf(f, "%dw%c%c",
              line_bytes_ + 2, 0,
              static_cast<int>(lines_.size()));
      for (auto &line : lines_) {
        fwrite(line.data(), 1, line.size(), f);
      }
      line_bytes_ = 0;
      lines_.clear();
    }
  }

 private:
  static const unsigned max_block_size_ = 16350;
  static const unsigned max_lines_per_block_ = 32;

  std::vector<std::vector<uint8_t>> lines_;
  int line_bytes_;
};

#endif  // BLOCK_H
