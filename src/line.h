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

#ifndef LINE_H
#define LINE_H

#include <stdint.h>
#include <vector>

std::vector<uint8_t> encode_line(
    const std::vector<uint8_t> &line,
    const std::vector<uint8_t> &reference);

std::vector<uint8_t> encode_line(
    const std::vector<uint8_t> &line);

#endif  // LINE_H
