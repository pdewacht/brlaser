// A quick-and-dirty tool to convert print files back to pbm images.
//
// Copyright 2013 Peter De Wachter
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <exception>
#include <vector>
#include <string>

namespace {

const size_t MAX_LINE_SIZE = 2000;

FILE *in_file;
std::vector<std::vector<uint8_t>> page;
std::vector<uint8_t> line;
size_t line_offset;


class unexpected_eof: public std::exception {
 public:
  virtual const char *what() const noexcept {
    return "Unexpected EOF";
  }
};

class line_overflow: public std::exception {
 public:
  virtual const char *what() const noexcept {
    return "Unreasonable long line, aborting";
  }
};


uint8_t get() {
  int ch = getc(in_file);
  if (ch < 0)
    throw unexpected_eof();
  return ch;
}

unsigned read_overflow() {
  uint8_t ch;
  unsigned sum = 0;
  do {
    ch = get();
    sum += ch;
  } while (ch == 255);
  return sum;
}

void read_repeat(uint8_t cmd) {
  uint16_t offset = (cmd >> 5) & 3;
  if (offset == 3)
    offset += read_overflow();
  uint16_t count = cmd & 31;
  if (count == 31)
    count += read_overflow();
  count += 2;
  uint8_t data = get();

  size_t end = line_offset + offset + count;
  if (end > line.size()) {
    if (end > MAX_LINE_SIZE)
      throw line_overflow();
    line.resize(end);
  }
  line_offset += offset;
  std::fill_n(line.begin() + line_offset, count, data);
  line_offset += count;
}

void read_substitute(uint8_t cmd) {
  uint16_t offset = (cmd >> 3) & 15;
  if (offset == 15)
    offset += read_overflow();
  uint16_t count = cmd & 7;
  if (count == 7)
    count += read_overflow();
  count += 1;

  size_t end = line_offset + offset + count;
  if (end > line.size()) {
    if (end > MAX_LINE_SIZE)
      throw line_overflow();
    line.resize(end);
  }
  line_offset += offset;
  std::generate_n(line.begin() + line_offset, count, get);
  line_offset += count;
}

void read_edit() {
  int8_t cmd = get();
  if (cmd < 0) {
    read_repeat(cmd);
  } else {
    read_substitute(cmd);
  }
}

void read_line() {
  uint8_t num_edits = get();
  if (num_edits == 255) {
    line.clear();
  } else {
    line_offset = 0;
    for (int i = 0; i < num_edits; ++i) {
      read_edit();
    }
  }
  page.push_back(line);
}

void read_block() {
  unsigned count = get();
  count = count * 256 + get();
  for (unsigned i = 0; i < count; ++i) {
    read_line();
  }
}

bool read_page() {
  bool in_esc = false;
  int ch;

  page.clear();
  line.clear();
  while ((ch = getc(in_file)) >= 0) {
    if (ch == '\f') {
      break;
    } else if (ch == 033) {
      in_esc = true;
    } else if (in_esc && ch == 'w') {
      read_block();
    } else if (in_esc && (ch >= 'A' && ch <= 'Z')) {
      in_esc = false;
    }
  }
  return !page.empty();
}

void write_pnm(FILE *f) {
  size_t height = page.size();
  size_t width = 0;
  for (auto &l : page) {
    width = std::max(width, l.size());
  }

  fprintf(f, "P4 %zd %zd\n", width * 8, height);
  std::vector<uint8_t> empty(width);
  for (auto &l : page) {
    fwrite(l.data(), 1, l.size(), f);
    fwrite(empty.data(), 1, width - l.size(), f);
  }
}

}  // namespace

int main(int argc, char *argv[]) {
  const char *in_filename;
  std::string out_prefix;

  if (argc > 2) {
    in_filename = argv[1];
    out_prefix = argv[2];
  } else if (argc > 1) {
    in_filename = argv[1];
    out_prefix = argv[1];
  } else {
    in_filename = nullptr;
    out_prefix = "page";
  }

  if (in_filename) {
    in_file = fopen(in_filename, "rb");
    if (!in_file) {
      fprintf(stderr, "Can't open file \"%s\"\n", in_filename);
      return 1;
    }
  } else {
    in_file = stdin;
    if (isatty(0)) {
      fprintf(stderr, "No filename given and no input on stdin\n");
      return 1;
    }
  }

  try {
    int page_num = 1;
    while (read_page()) {
      std::string out_filename = out_prefix
        + "-" + std::to_string(page_num) + ".pbm";
      FILE *out_file = fopen(out_filename.c_str(), "wb");
      if (!out_file) {
        fprintf(stderr, "Can't write file \"%s\"\n", out_filename.c_str());
        return 1;
      }
      write_pnm(out_file);
      fclose(out_file);
      fprintf(stderr, "%s\n", out_filename.c_str());
      ++page_num;
    }
  } catch (std::exception &e) {
    fprintf(stderr, "%s\n", e.what());
    return 1;
  }
  return 0;
}
