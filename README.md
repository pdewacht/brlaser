brlaser: Brother laser printer driver
=====================================

brlaser is a CUPS driver for Brother laser printers.

Although most Brother printers support a standard printer language
such as PCL or PostScript, not all do. If you have a monochrome
Brother laser printer (or multi-function device) and the other open
source drivers don't work, this one might help.

This driver is known to work with these printers:

* Brother DCP-1510
* Brother DCP-7030
* Brother DCP-7040
* Brother DCP-7055
* Brother DCP-7055W
* Brother DCP-7065DN
* Brother HL-L2300D
* Brother MFC-7360N


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
at least Debian, Ubuntu, Raspbian, openSUSE, NixOS and Arch Linux.
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
