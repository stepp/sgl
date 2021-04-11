#!/bin/bash
projectzipfile=dist/sgl-sample-project.zip

echo "Zipping to $projectzipfile ..."
rm -f $projectzipfile
zip -qr $projectzipfile CMake* lib/ res/ sgl-project.pro src/welcome.cpp

echo "Done."
