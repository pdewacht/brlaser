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
#include <unistd.h>
#include <algorithm>
#include <exception>
#include <vector>

#define LINE_SIZE 1200

namespace {

FILE * in_file;
std::vector<uint8_t> page;
uint8_t line[LINE_SIZE] = { 0 };
size_t line_offset;
size_t num_lines = 0;


class unexpected_eof: public std::exception {
 public:
  virtual const char *what() const noexcept {
    return "Unexpected EOF";
  }
};

class line_overflow: public std::exception {
 public:
  virtual const char *what() const noexcept {
    return "Line buffer overflow, recompile with bigger LINE_SIZE";
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

  if (line_offset + offset + count > sizeof(line))
    throw line_overflow();
  line_offset += offset;
  std::fill_n(line + line_offset, count, data);
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

  if (line_offset + offset + count > sizeof(line))
    throw line_overflow();
  line_offset += offset;
  std::generate_n(line + line_offset, count, get);
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
    std::fill_n(line, sizeof(line), 0);
  } else {
    line_offset = 0;
    for (int i = 0; i < num_edits; ++i) {
      read_edit();
    }
  }
  page.insert(page.end(), line, line + sizeof(line));
  ++num_lines;
}

void read_block() {
  uint8_t count = get();
  count = count * 256 + get();
  for (int i = 0; i < count; ++i) {
    read_line();
  }
}

bool read_page() {
  bool in_esc = true;
  int ch;

  page.clear();
  page.reserve(20 * 1000 * 1000);
  num_lines = 0;

  while ((ch = getc(in_file)) >= 0) {
    if (ch == '\f') {
      in_esc = false;
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
      size_t width = sizeof(line) * 8;
      fprintf(out_file, "P4 %zd %zd\n", width, num_lines);
      fwrite(page.data(), 1, page.size(), out_file);
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
