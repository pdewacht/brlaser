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

#ifndef JOB_H
#define JOB_H

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>

struct page_params {
  int num_copies;
  int resolution;
  bool duplex;
  bool tumble;
  bool economode;
  std::string sourcetray;
  std::string mediatype;
  std::string papersize;

  bool operator==(const page_params &o) const {
    return num_copies == o.num_copies
      && resolution == o.resolution
      && duplex == o.duplex
      && tumble == o.tumble
      && economode == o.economode
      && sourcetray == o.sourcetray
      && mediatype == o.mediatype
      && papersize == o.papersize;
  }
};

class job {
 public:
  typedef bool (*nextline_fn)(std::vector<uint8_t> &buf);

  explicit job(FILE *out, const std::string &job_name);
  ~job();

  void encode_page(const page_params &params,
                   int lines,
                   int linesize,
                   nextline_fn nextline);

  int pages() const {
    return pages_;
  }

 private:
  void begin_job();
  void end_job();
  void write_page_header();

  FILE *out_;
  std::string job_name_;
  page_params page_params_;
  int pages_;
};

#endif  // JOB_H
