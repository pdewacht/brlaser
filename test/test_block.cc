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

#include "lest.hpp"
#include <stdint.h>
#include <vector>
#include "tempfile.h"
#include "src/block.h"

typedef std::vector<uint8_t> vec;

const lest::test specification[] = {
  "Block line limit",
  [] {
    block b;
    for (int i = 0; i < 128; ++i) {
      EXPECT(b.line_fits(1));
      b.add_line(vec(1));
    }
    EXPECT(!b.line_fits(1));
  },

  "Block size limit",
  [] {
    block b;
    for (int i = 0; i < 16; ++i) {
      EXPECT(b.line_fits(1000));
      b.add_line(vec(1000));
    }
    EXPECT(!b.line_fits(400));
  },

  "Flush",
  [] {
    block b;
    {
      tempfile f;
      b.flush(f.file());
      EXPECT(f.data().empty());
    }
    for (uint8_t n = 0; n < 10; n += 2) {
      vec line = {n, static_cast<uint8_t>(n+1)};
      EXPECT(b.line_fits(line.size()));
      b.add_line(std::move(line));
    }
    {
      tempfile f;
      b.flush(f.file());
      EXPECT(( f.data() == vec{'1','2','w',0,5,0,1,2,3,4,5,6,7,8,9} ));
    }
    {
      tempfile f;
      b.flush(f.file());
      EXPECT(f.data().empty());
    }
  },
};

int main() {
  return lest::run(specification);
}
