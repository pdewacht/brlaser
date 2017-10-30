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
#include <assert.h>
#include <stdint.h>
#include <vector>
#include "../src/line.h"

typedef std::vector<uint8_t> vec;


uint8_t sub(uint8_t offset, uint8_t count) {
  assert(offset < 16);
  assert(count < 8);
  return (offset << 3) | count;
}

uint8_t rep(uint8_t offset, uint8_t count) {
  assert(offset < 4);
  assert(count < 32);
  return 128 | (offset << 5) | count;
}


const lest::test specification[] = {
  "Don't crash on zero-length lines",
  [] {
    EXPECT(( encode_line(vec{}) == vec{0xFF} ));
    EXPECT(( encode_line(vec{}, vec{}) == vec{0xFF} ));
  },

  "Encoding an initial blank line",
  [] {
    EXPECT(( encode_line(vec{0,0,0}) == vec{0xFF} ));
  },

  "Encoding an initial non-blank line",
  [] {
    EXPECT(( encode_line(vec{1,2,3}) == (vec{1,sub(0,2),1,2,3}) ));
  },

  "Encoding a (non-initial) blank line",
  [] {
    EXPECT(( encode_line(vec{0,0,0}, vec{1,2,3}) == vec{0xFF} ));
  },

  "Encoding a repeated line",
  [] {
    EXPECT(( encode_line(vec{1,2,3}, vec{1,2,3}) == vec{0} ));
  },

  "Using a subsitute command",
  [] {
    EXPECT(( encode_line(vec{0,0,1,2,3,0,0}, vec(7)) == vec{1,sub(2,2),1,2,3} ));
  },

  "Using a repeat command",
  [] {
    EXPECT(( encode_line(vec{0,0,1,1,0,0}, vec(6)) == vec{1,rep(2,0),1} ));
  },

  "Repeat command followed by substitute command",
  [] {
    EXPECT(( encode_line(vec{1,1,1,2,3}, vec(5)) == vec{2,rep(0,1),1,sub(0,1),2,3} ));
  },

  "Substitute comand followed by repeat command",
  [] {
    EXPECT(( encode_line(vec{3,2,1,1,1}, vec(5)) == vec{2,sub(0,1),3,2,rep(0,1),1} ));
  },

  "Substitute with an unmodified byte in the middle",
  [] {
    EXPECT(( encode_line(vec{1,2,3,0,1,2,3}, vec(7)) == vec{1,sub(0,6),1,2,3,0,1,2,3} ));
  },

  "Substitue with two unmodified bytes in the middle",
  [] {
    EXPECT(( encode_line(vec{1,2,3,0,0,1,2,3}, vec(8)) == vec{2,sub(0,2),1,2,3,sub(2,2),1,2,3} ));
  },

  "Repeat with an unmodified byte in the middle",
  [] {
    EXPECT(( encode_line(vec{1,1,1,0,1,1,1}, vec(7)) == vec{2,rep(0,1),1,rep(1,1),1} ));
  },

  "254 edits needed for a single line",
  [] {
    vec line, result;
    for (int i = 0; i < 254; ++i)
      line.insert(line.end(), {0,0,1});
    result.push_back(254);
    for (int i = 0; i < 254; ++i)
      result.insert(result.end(), {sub(2,0),1});
    EXPECT(( encode_line(line, vec(line.size())) == result ));
  },

  "Give up if more than 254 edits needed...",
  [] {
    vec line, result;
    for (int i = 0; i < 255; ++i)
      line.insert(line.end(), {0,0,1});
    result.push_back(254);
    for (int i = 0; i < 253; ++i)
      result.insert(result.end(), {sub(2,0),1});
    result.insert(result.end(), {sub(2,3),1,0,0,1});
    EXPECT(( encode_line(line, vec(line.size())) == result ));
  },

  "Repeat command with overflow bytes",
  [] {
    vec line(3, 0);
    line.insert(line.end(), 512, 1);
    vec ref(line.size(), 0);
    vec expected{1,rep(3,31),0,255,224,1};
    EXPECT(encode_line(line, ref) == expected);
  },

  "Substitute command with overflow bytes",
  [] {
    vec expected{1,sub(15,7),255,0,255,237};
    vec line(270, 0);
    for (int i = 0; i < 250; ++i) {
      expected.insert(expected.end(), {1,2});
      line.insert(line.end(), {1,2});
    }
    vec ref(line.size(), 0);
    EXPECT(encode_line(line, ref) == expected);
  },
};


int main() {
  return lest::run(specification);
}
