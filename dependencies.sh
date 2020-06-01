#!/bin/bash

exit_script () {
  echo $1
  popd 2>&1 > /dev/null
  exit 1
}

pushd ~ 2>&1 > /dev/null

echo "Installing dependencies for OpenGazer...this may take a while..."

if [[ "$(uname)" == "Linux" ]] ; then
  echo "Installing libraries for OpenCV, Boost, GSL, FANN and CMake"
  sudo apt-get update
  sudo apt-get install -y libopencv-dev libboost-dev libboost-system-dev libboost-filesystem-dev libgsl0-dev libfann2 libfann-dev libv4l-dev cmake wget

  echo "Installing GSL from source"
  wget ftp://ftp.gnu.org/gnu/gsl/gsl-1.15.tar.gz
  tar zxvf gsl-1.15.tar.gz
  pushd gsl-1.15 2>&1 > /dev/null
  ./configure
  make
  sudo make install

  echo "Installing FFMPEG from source"
  { popd; popd; } 2>&1 > /dev/null
  bash ffmpeg.sh

  echo "Installing QT from Ubuntu repositories"
  sudo apt-get install -y qt5-default

  echo "Installing Xlib to mouse events"
  sudo apt-get install libx11-dev libxtst-dev

  echo "Installing xdotoll"
  sudo apt-get install libxdo-dev libxdo2


elif [[ "$(uname)" == "Darwin" ]] ; then
  echo "OSX detected - this script hasn't yet been tested on OSX, but it should be fine. Press any key to continue or kill it now with Ctrl-C..."
  read

  if ! type brew 2>&1 > /dev/null ; then
    exit_script "You must first install Homebrew"
  fi

  if ! type xcodebuild 2>&1 > /dev/null ; then
    exit_script "You must first install Xcode"
  fi

  if [[ ! -d /Applications/Utilities/XQuartz.app ]] ; then
    exit_script "You must first install XQuartz"
  fi

  brew tap homebrew/science
  brew install gsl cmake ffmpeg opencv fann boost

  brew install qt5
  brew link --force qt5
fi

popd 2>&1 /dev/null

echo "\n\nCompleted installing dependencies for OpenGazer!"
