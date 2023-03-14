brlaser: Brother laser printer driver
=====================================

brlaser is a CUPS driver for Brother laser printers.

Although most Brother printers support a standard printer language
such as PCL or PostScript, not all do. If you have a monochrome
Brother laser printer (or multi-function device) and the other open
source drivers don't work, this one might help.

This driver has been reported to work with these printers:

* Brother DCP-1510 series
* Brother DCP-1600 series
* Brother DCP-1610W series
* Brother DCP-7020
* Brother DCP-7030
* Brother DCP-7040
* Brother DCP-7055
* Brother DCP-7055W
* Brother DCP-7060D
* Brother DCP-7065DN
* Brother DCP-7070DW
* Brother DCP-7080
* Brother DCP-8065DN
* Brother DCP-L2500D series
* Brother DCP-L2510D series 
* Brother DCP-L2520D series
* Brother DCP-L2520DW series
* Brother DCP-L2537DW
* Brother DCP-L2540DW series
* Brother DCP-L2550DW series
* Brother FAX-2820
* Brother HL-1110 series
* Brother HL-1200 series
* Brother HL-2030 series
* Brother HL-2130 series
* Brother HL-2140 series
* Brother HL-2220 series
* Brother HL-2230 series
* Brother HL-2240D series
* Brother HL-2250DN series
* Brother HL-2270DW series
* Brother HL-2280DW
* Brother HL-5030 series
* Brother HL-5040 series
* Brother HL-L2300D series
* Brother HL-L2305  series
* Brother HL-L2310D series
* Brother HL-L2320D series
* Brother HL-L2340D series
* Brother HL-L2350DW series
* Brother HL-L2360D series
* Brother HL-L2370DN series
* Brother HL-L2375DW series
* Brother HL-L2380DW series
* Brother HL-L2390DW
* Brother HL-L5000D series
* Brother MFC-1810 series
* Brother MFC-1910W
* Brother MFC-7240
* Brother MFC-7320
* Brother MFC-7340
* Brother MFC-7360N
* Brother MFC-7365DN
* Brother MFC-7420
* Brother MFC-7440N
* Brother MFC-7460DN
* Brother MFC-7840W
* Brother MFC-8710DW
* Brother MFC-8860DN
* Brother MFC-L2700DN series
* Brother MFC-L2700DW series
* Brother MFC-L2710DN series
* Brother MFC-L2710DW series
* Brother MFC-L2750DW series
* Brother MFC-L3750CDW series
* Lenovo LJ2650DN
* Lenovo M7605D
* Fuji Xerox DocuPrint P265 dw


Other printers
--------------

If your printer isn't included in the list above, just try selecting
any entry marked 'brlaser' and see if it works.

If it does, please create a new issue here in Github and include the
output of this command:

    sudo lpinfo --include-schemes usb -l -v

Then I'll be able to add a proper entry for your printer.


Installation
------------

Some operating systems already ship this driver. This is the case for
at least Debian, Gentoo, Ubuntu, Raspbian, openSUSE, NixOS, Arch Linux 
and Guix.
Look for a package named `printer-driver-brlaser`.

You'll also need Ghostscript, in case that's not installed
automatically.

Once brlaser is installed, you can add your printer using the usual
CUPS interface.


Building from source
--------------------

To compile brlaser you'll need CMake and the CUPS development packages
(libcups2-dev, libcupsimage2-dev or similar).

Get the code by cloning the git repo <!-- or downloading the [latest
release] -->. Compile and install with these commands:

    cmake .
    make
    sudo make install

It might be needed to restart CUPS after this.

[latest release]: https://github.com/pdewacht/brlaser/releases/latest


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
