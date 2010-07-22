#!/bin/bash

function deploylib
{
    install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore $1
    install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui $1
    install_name_tool -change QtXml.framework/Versions/4/QtXml @executable_path/../Frameworks/QtXml.framework/Versions/4/QtXml $1

    install_name_tool -change libCore.1.dylib @executable_path/../Frameworks/libCore.1.dylib $1
} 

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
deploylib QMPQ.app/Contents/PlugIns/imageformats/libQblpImagePlugin.dylib
deploylib QMPQ.app/Contents/PlugIns/imageformats/libQtgaImagePlugin.dylib 
deploylib QMPQ.app/Contents/PlugIns/imageformats/libQddsImagePlugin.dylib 

echo "deploying qmpqplugins"
deploylib QMPQ.app/Contents/PlugIns/qmpqplugins/libCodeEditor.dylib
deploylib QMPQ.app/Contents/PlugIns/qmpqplugins/libImageViewer.dylib
deploylib QMPQ.app/Contents/PlugIns/qmpqplugins/libQXMLViewer.dylib
