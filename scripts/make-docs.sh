#!/bin/bash
cd doc
echo "Running doxygen to generate HTML documentation in doc/ ..."
doxygen doxygen.ini >/dev/null 2>&1

echo "Post-processing HTML docs ..."
for FILE in `ls -1B html/*html`; do
	./postprocess-doc-file.pl $FILE
done

# use the list of classes as the "index" page
cp -f html/annotated.html html/index.html

# update ZIP of all docs
echo "Zipping to sgl-docs.zip ..."
cd ..
rm -f sgl-docs.zip
zip -qr sgl-docs.zip doc/

echo "Done."
