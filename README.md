Driver for (some) Brother laser printers
========================================

Most Brother printers support a standard printer language such as PCL
or PostScript, but not all do.  If you have a monochrome Brother laser
printer (or multi-function device) and the other open source drivers
don't work, this one might help.

It is known to support these printers:

* Brother DCP-1510 series
* Brother DCP-7030
* Brother DCP-7055
* Brother DCP-7055W
* Brother DCP-7065DN
* Brother MFC-7360N

Build requirements
------------------
Besides a working build environment (i.e. `gcc` or compatible compiler) you will need the header files for libcupsimage2 (especially `raster.h`), as well as the corresponding general build files for CUPS. On Ubuntu and Debian, and their derivatives, these are contained in the following packages and their respective dependencies:

* `libcups2-dev`
* `libcupsimage2-dev`

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
