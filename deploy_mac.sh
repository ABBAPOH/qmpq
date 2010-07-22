#!/bin/bash

rm -R QMPQ.app
cp -R bin/QMPQ.app QMPQ.app

echo "deploying QMPQ.app"
macdeployqt QMPQ.app

echo "removing useless frameworks and/or libraries"
rm -R QMPQ.app/Contents/Frameworks/QtDBus.framework/
rm -R QMPQ.app/Contents/Frameworks/QtMediaServices.framework/
rm -R QMPQ.app/Contents/Frameworks/QtMultimedia.framework/
rm -R QMPQ.app/Contents/Frameworks/QtNetwork.framework/
rm -R QMPQ.app/Contents/Frameworks/QtOpenGL.framework/
rm QMPQ.app/Contents/MacOS/libCore.*
rm -R QMPQ.app/Contents/MacOS/QMPQFileEngine.framework/
mv QMPQ.app/Contents/MacOS/qmpqplugins/ QMPQ.app/Contents/PlugIns
mv QMPQ.app/Contents/MacOS/imageformats/libQblpImagePlugin.1.0.0.dylib QMPQ.app/Contents/PlugIns/imageformats/libQblpImagePlugin.dylib
mv QMPQ.app/Contents/MacOS/imageformats/libQtgaImagePlugin.1.0.0.dylib QMPQ.app/Contents/PlugIns/imageformats/libQtgaImagePlugin.dylib
mv QMPQ.app/Contents/MacOS/imageformats/libQddsImagePlugin.1.0.0.dylib QMPQ.app/Contents/PlugIns/imageformats/libQddsImagePlugin.dylib
rm -R QMPQ.app/Contents/MacOS/imageformats

echo "deploying imageformats"
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/imageformats/libQblpImagePlugin.dylib
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/imageformats/libQtgaImagePlugin.dylib 
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/imageformats/libQddsImagePlugin.dylib 

echo "deploying qmpqplugins"
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/qmpqplugins/libCodeEditor.dylib
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/qmpqplugins/libImageViewer.dylib
./deployLib_mac.sh QMPQ.app/Contents/PlugIns/qmpqplugins/libQXMLViewer.dylib
