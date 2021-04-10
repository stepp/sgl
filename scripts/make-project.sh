#!/bin/bash
zipfile=dist/sgl-sample-project.zip

echo "Zipping to $zipfile ..."
rm -f $zipfile
zip -qr $zipfile CMake* lib/ res/ *.pro src/welcome.cpp

echo "Done."
