timelapse_test
==============

A small program for capturing timelapse images.


Compilation
-----------
Requires:
 * POSIX compliant environment (e.g. Linux)
 * OpenCV2
 * g++

To build:
`cd src/`
`make`

Usage
-----
`$ ./capture_timelapse <timeout> <destination_folder>`

 * `<timeout>` is in seconds
 * `<destination_folder>` where to save all of the images that are captured

All images are saved to a `.jpg` extension and have a UNIX timestamp of when
they were captured.  e.g. `1405282940.jpg` would be the capture made on July
13th, 2014 at 20:22:20 UTC (alternativly the time when Mario Goetze kicked the
winning goal of the FIFA 2014 final match).

Folder Description
------------------
 * `./toybox/` is just a little place where I tinker with code.  Some of it may
   be moved into the main source code.
 * `./src/` main source directory.
