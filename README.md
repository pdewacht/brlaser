"brlaser" Brother DCP-7030 printer driver
=========================================

This is a driver for the Brother DCP-7030 laser printer. It might also
be worth trying if you have some other old Brother monochrome laser
printer.

Currently this printer is not supported by other free software
drivers. Ghostscript's hl1250 driver almost works, but has glitches
with some fonts. (That driver tries to use PCL emulation, but that
seems to be buggy on this printer. The proprietary driver never uses
that mode.)

Brother provides a non-free x86-only driver.

Requirements
------------

* CUPS: tested with version 1.6.
* A C++11 compiler: GCC 4.6 or later, or a recent version of Clang.

Copyright
---------

Copyright © 2013 Peter De Wachter

brlaser is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

brlaser is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with brlaser.  If not, see <http://www.gnu.org/licenses/>.
