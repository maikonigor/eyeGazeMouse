CVC Eye Tracker [![Build Status](https://travis-ci.org/tiendan/OpenGazer.svg?branch=master)](https://travis-ci.org/tiendan/OpenGazer)
===========================
This eye tracking software is based on the open source project [Opengazer] (http://www.inference.phy.cam.ac.uk/opengazer/)
and is improved with several features. The updated software supports video recording
for offline processing, offline batch processing, enhanced video outputting for visual
analysis of experiments and a head repositioning feature which allows users to correct
their head pose during eye tracker usage in order to improve accuracy.

This software is the result of [on-going research] (http://mv.cvc.uab.es/projects/eye-tracker) in [Computer Vision Center (CVC)] (http://www.cvc.uab.es/index.asp?idioma=en) and 
[Universitat Autònoma de Barcelona] (http://uab.es/english/), Barcelona. Please refer to the [CONTRIBUTORS] (https://github.com/tiendan/OpenGazer/blob/master/CONTRIBUTORS.md) file for the list of contributors in this project and the [DEVELOP] (https://github.com/tiendan/OpenGazer/blob/master/DEVELOP.md) file for how to develop additional modules.

The automatic installation for [Mac OS X and Ubuntu] (#scripted-installation-for-mac-os-x-or-ubuntu-systems) are below. If you have problems with this script, you can try the manual installation steps for [Mac] (#manual-installation-using-macports-on-mac-os-x-systems) and [Ubuntu] (#manual-installation-on-ubuntu-1204) at the bottom.

After installation, check the [usage instructions] (#basic-usage-instructions).

Scripted Installation for Mac OS X or Ubuntu systems
-----

OS X only: Install Xcode, Xcode Command Line Tools, XQuartz and Homebrew

Open a terminal and run:
  - cd opengazer
  - sudo bash dependencies.sh
  - ./configure
  - make

Basic Usage Instructions
------------
1. Put the camera centrally under you monitor, so that the height of your face in the image is about half of the height of the image, and close to the centre of the image.  Make sure the background light level is reasonably constant across the image.
2. Following the onscreen instructions, press the X key to choose anchor points on your face (repeat until you are happy with the results).
3. Press the C key to start the calibration.
4. When the calibration ends, the sample frog game will start. Here, the aim is to find the hidden frog. You have to use your gaze to locate the frog and stare at it for a few seconds until the frog is teleported to another location. Remember to keep still and only use your eyes for playing (and for calibration too!).

How to Contribute
------------
One nice-to-have feature would be compilation on Windows. If anybody wants to attempt it, here are some links:

1. [boost library] (http://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html)
2. [Gsl for Windows] (http://gnuwin32.sourceforge.net/packages/gsl.htm)
3. [OpenCV + ffmpeg] (https://github.com/Itseez/opencv/tree/ddf82d0b154873510802ef75c53e628cd7b2cb13/3rdparty/ffmpeg)
4. [OpenCV + ffmpeg (additional)] (http://kronoskoders.logdown.com/posts/256664-installing-opencv-and-ffmpeg-on-windows)
5. [FANN (currently not used, but still in configuration)] (http://leenissen.dk/fann/wp/help/installing-fann/)

If you get it working, let me know! Apart from this, for developing another module (eye corner detectors, iris center detectors, gaze estimation methods, etc.), check the [DEVELOP] (https://github.com/tiendan/OpenGazer/blob/master/DEVELOP.md) file for instructions.

Manual Installation using MacPorts on Mac OS X systems
-----------------------------------------------
This has been tested on a MacBook with OS X 10.6, 10.8 and 10.9, both with the built-in 
iSight camera and a Logitech webcam.

1. Download and install [Xcode] (https://developer.apple.com/xcode/)
2. Download and install [Qt] (http://www.qt.io/download-open-source/)
3. Download and install the latest version of [MacPorts] (http://www.macports.org/)
4. Update the port definitions and install the required packages from MacPorts. Note that this
might take a long time since all these packages have a lot of dependencies:
  - sudo port selfupdate
  - sudo port install gsl cmake ffmpeg opencv fann boost
5. Download and install [XQuartz] (http://xquartz.macosforge.org/landing/)
6. Open a terminal and build the software:
  - cd opengazer
  - ./configure
  - make

**NOTES:** Version @2.4.6_1 of OpenCV from MacPorts has a bug which prevents access to the webcam. In such a
case, the preferred version (@2.4.6_0) can be installed with the following steps (steps from [here] (http://www.codedrop.ca/blog/archives/56)):

1. Modify the configuration file of MacPorts:
  - sudo nano /opt/local/etc/macports/sources.conf
  - Add **file:///Users/Shared/dports** before rsync line
  - Save (Conrol+O) and exit (Control+X)
2. Create the same folder in Terminal:
  - mkdir /Users/Shared/dports
3. Download the definitions for the old version of OpenCV:
   - cd /Users/Shared/dports && svn co -r 107771 http://svn.macports.org/repository/macports/trunk/dports/graphics/opencv/ graphics/opencv
4. Update the indices of MacPorts:
  - portindex /Users/Shared/dports
5. Verify that the old version (2.4.6) is now available in MacPorts:
  - port list opencv*
6. Install the old version:
  - sudo port install opencv @2.4.6

Manual Installation on Ubuntu 12.04
-------------------------------
Tested on a clean installation of Ubuntu 12.04 with a Logitech webcam.

1. Install required libraries
  - sudo apt-get install libopencv-* libboost-dev libgsl0-dev libfann2 libfann-dev libv4l-dev cmake libboost-system-dev libboost-filesystem-dev
2. Download and install [Qt] (http://www.qt.io/download-open-source/)
3. Install GSL from source
  - wget ftp://ftp.gnu.org/gnu/gsl/gsl-1.15.tar.gz
  - tar zxf gsl-1.15.tar.gz
  - cd gsl-1.15
  - ./configure
  - make
  - make install
4. [Build and install] (https://trac.ffmpeg.org/wiki/UbuntuCompilationGuide) FFMPEG 0.7.13
5. Build eyetracker
  - cd opengazer
  - ./configure
  - make