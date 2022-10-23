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

#include "job.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include "line.h"
#include "block.h"


job::job(FILE *out, const std::string &job_name)
    : out_(out),
      job_name_(job_name),
      page_params_(),
      pages_(0) {
  // Delete dubious characters from job name
  std::replace_if(job_name_.begin(), job_name_.end(), [](char c) {
      return c < 32 || c >= 127 || c == '"' || c == '\\';
    }, ' ');
}

job::~job() {
  if (pages_ != 0) {
    end_job();
  }
}

void job::begin_job() {
  for (int i = 0; i < 128; ++i) {
    putc(0, out_);
  }
  fprintf(out_, "\033%%-12345X@PJL\n");
  fprintf(out_, "@PJL JOB NAME=\"%s\"\n", job_name_.c_str());
}

void job::end_job() {
  fprintf(out_, "\033%%-12345X@PJL\n");
  fprintf(out_, "@PJL EOJ NAME=\"%s\"\n", job_name_.c_str());
  fprintf(out_, "\033%%-12345X\n");
}

void job::write_page_header() {
  fprintf(out_, "\033%%-12345X@PJL\n");
  if (page_params_.resolution != 1200) {
    fprintf(out_, "@PJL SET RAS1200MODE = FALSE\n");
    fprintf(out_, "@PJL SET RESOLUTION = %d\n", page_params_.resolution);
  } else {
    fprintf(out_, "@PJL SET RAS1200MODE = TRUE\n");
    fprintf(out_, "@PJL SET RESOLUTION = 600\n");
  }
  fprintf(out_, "@PJL SET ECONOMODE = %s\n",
          page_params_.economode ? "ON" : "OFF");
  fprintf(out_, "@PJL SET SOURCETRAY = %s\n",
          page_params_.sourcetray.c_str());
  fprintf(out_, "@PJL SET MEDIATYPE = %s\n",
          page_params_.mediatype.c_str());
  fprintf(out_, "@PJL SET PAPER = %s\n",
          page_params_.papersize.c_str());
  fprintf(out_, "@PJL SET PAGEPROTECT = AUTO\n");
  fprintf(out_, "@PJL SET ORIENTATION = PORTRAIT\n");
  fprintf(out_, "@PJL ENTER LANGUAGE = PCL\n");

  fputs("\033E", out_);
  fprintf(out_, "\033&l%dX", std::max(1, page_params_.num_copies));

  if (page_params_.duplex) {
    fputs("\033&l2S", out_);
  } else {
    fputs("\033&l0S", out_);
  }
}

void job::encode_page(const page_params &page_params,
                      int lines,
                      int linesize,
                      nextline_fn nextline) {
  if (pages_ == 0) {
    begin_job();
  }
  ++pages_;

  if (!(page_params_ == page_params)) {
    page_params_ = page_params;
    write_page_header();
  }

  std::vector<uint8_t> line(linesize);
  std::vector<uint8_t> reference(linesize);
  block block;

  if (!nextline(line)) {
    return;
  }
  block.add_line(encode_line(line));
  std::swap(line, reference);

  fputs("\033*b1030m", out_);

  // XXX brother driver uses 128 lines per band
  const int lines_per_band = 64;

  for (int i = 1; i < lines && nextline(line); ++i) {
    std::vector<uint8_t> encoded;
    if (i % lines_per_band == 0) {
      block.flush(out_);
      encoded = encode_line(line);
    } else {
      encoded = encode_line(line, reference);
      if (!block.line_fits(encoded.size())) {
        block.flush(out_);
        encoded = encode_line(line);
      }
    }
    block.add_line(std::move(encoded));
    std::swap(line, reference);
  }

  block.flush(out_);
  fputs("1030M\f", out_);
  fflush(out_);
}
