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

#include "line.h"
#include <assert.h>
#include <algorithm>
using std::vector;

namespace {

void write_overflow(int value, vector<uint8_t> *out) {
  if (value >= 0) {
    if (value < 255) {
      out->push_back(value);
    } else {
      out->insert(out->end(), value / 255, 255);
      out->push_back(value % 255);
    }
  }
}

template <typename Iterator>
void write_substitute(int offset,
                      Iterator first,
                      Iterator last,
                      vector<uint8_t> *out) {
  assert(offset >= 0);
  assert(offset < 10000);
  assert(first != last);

  const int offset_max = 15;
  const int count_max = 7;
  int count = std::distance(first, last) - 1;

  int offset_low = std::min(offset, offset_max);
  int count_low = std::min(count, count_max);
  out->push_back((offset_low << 3) | count_low);
  write_overflow(offset - offset_max, out);
  write_overflow(count - count_max, out);
  out->insert(out->end(), first, last);
}

void write_repeat(int offset, int count, int value, vector<uint8_t> *out) {
  assert(offset >= 0);
  assert(offset < 10000);
  assert(count >= 2);
  assert(count < 10000);

  const int offset_max = 3;
  const int count_max = 31;
  count -= 2;

  int offset_low = std::min(offset, offset_max);
  int count_low = std::min(count, count_max);
  out->push_back(128 | (offset_low << 5) | count_low);
  write_overflow(offset - offset_max, out);
  write_overflow(count - count_max, out);
  out->push_back(value);
}


bool all_zeros(const vector<uint8_t> &buf) {
  return std::none_of(buf.begin(), buf.end(), [](uint8_t b) { return b; });
}

template <typename Iterator1, typename Iterator2>
int skip_to_next_mismatch(Iterator1 *first1,
                          Iterator1 last1,
                          Iterator2 *first2) {
  auto mismatch_it = std::mismatch(*first1, last1, *first2);
  int skipped = std::distance(*first1, mismatch_it.first);
  *first1 = mismatch_it.first;
  *first2 = mismatch_it.second;
  return skipped;
}

template <typename Iterator>
int repeat_length(Iterator first, Iterator last) {
  if (first != last) {
    auto k = *first;
    auto mismatch = std::find_if(std::next(first), last,
                                 [=](decltype(k) x) { return x != k; });
    return std::distance(first, mismatch);
  }
  return 0;
}

template <typename Iterator1, typename Iterator2>
int substitute_length(Iterator1 first1, Iterator1 last1, Iterator2 first2) {
  if (first1 != last1) {
    Iterator1 it1 = first1;
    Iterator2 it2 = first2;
    Iterator1 next1 = std::next(first1);
    Iterator2 next2 = std::next(first2);
    Iterator1 prev1 = first1;
    while (next1 != last1) {
      if ((*it1 == *it2 && *next1 == *next2)) {
        return std::distance(first1, it1);
      }
      if (*it1 == *next1 && *it1 == *prev1) {
        return std::distance(first1, prev1);
      }
      prev1 = it1;
      it1 = next1; it2 = next2;
      ++next1; ++next2;
    }
  }
  return std::distance(first1, last1);
}

size_t reserve_size(const vector<uint8_t> &line) {
  // Big enough to store the line uncompressed together with an Substitute
  // command with many overflow bytes.
  return line.size() + 16;
}

}  // namespace

vector<uint8_t> encode_line(const vector<uint8_t> &line,
                            const vector<uint8_t> &reference) {
  assert(line.size() == reference.size());
  if (all_zeros(line)) {
    return vector<uint8_t>(1, 0xFF);
  }

  vector<uint8_t> output;
  output.reserve(reserve_size(line));
  output.push_back(0);  // first byte is the edit count

  const uint8_t max_edits = 254;
  int num_edits = 0;

  auto line_it = line.begin();
  auto line_end_it =
    std::mismatch(line.rbegin(), line.rend(), reference.rbegin()).first.base();
  auto ref_it = reference.begin();
  while (1) {
    int offset = skip_to_next_mismatch(&line_it, line_end_it, &ref_it);
    if (line_it == line_end_it) {
      // No more differences, we're done.
      break;
    }

    if (++num_edits == max_edits) {
      // We've run out of edits. Just output the rest of the line in a big
      // substitute command.
      write_substitute(offset, line_it, line_end_it, &output);
      break;
    }

    int s = substitute_length(line_it, line_end_it, ref_it);
    if (s > 0) {
      write_substitute(offset, line_it, std::next(line_it, s), &output);
      line_it += s;
      ref_it += s;
    } else {
      int r = repeat_length(line_it, line_end_it);
      assert(r >= 2);
      write_repeat(offset, r, *line_it, &output);
      line_it += r;
      ref_it += r;
    }
  }

  assert(num_edits <= max_edits);
  output[0] = num_edits;
  return output;
}

vector<uint8_t> encode_line(const vector<uint8_t> &line) {
  if (all_zeros(line)) {
    return vector<uint8_t>(1, 0xFF);
  }
  vector<uint8_t> buf;
  buf.reserve(reserve_size(line));
  buf.push_back(1);
  write_substitute(0, line.begin(), line.end(), &buf);
  return buf;
}
