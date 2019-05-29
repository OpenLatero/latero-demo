#!/bin/sh

# check if dylibbundler is installed
command -v dylibbundler >/dev/null 2>&1 || {
  echo >&2 "This script requires dylibbundler. If using MacPorts, type 'sudo port install dylibbundler' to install it. See https://github.com/auriamg/macdylibbundler for more information."; exit 1;
}

# check if .app already exists, if so confirm before deleting
if [ -d "LateroDemo.app" ]; then
  read -p "LateroDemo.app already exists. Overwrite? (y/n) " -n 1 -r
  if [[ $REPLY =~ ^[Yy]$ ]]
  then
    rm -rf LateroDemo.app
  fi
  echo "\n"
fi

echo "Creating .app folder structure..."
mkdir LateroDemo.app
mkdir LateroDemo.app/Contents
mkdir LateroDemo.app/Contents/MacOS

echo "Copying resources..."
cp -r resources LateroDemo.app/Contents/Resources

echo "Copying Info.plist..."
cp appbundle/Info.plist LateroDemo.app/Contents/

echo "Copying icons..."
cp appbundle/latero-demo.icns LateroDemo.app/Contents/Resources/

echo "Copying executable..."
if [ ! -f src/Debug/latero-demo ]; then
    echo "Executable not found in src/Debug/latero-demo, aborting."
    rm -rf LateroDemo.app
fi
cp src/Debug/latero-demo LateroDemo.app/Contents/MacOS/

echo "Copying libraries..."
dylibbundler -od -b -x ./LateroDemo.app/Contents/MacOS/latero-demo -d ./LateroDemo.app/Contents/libs/
