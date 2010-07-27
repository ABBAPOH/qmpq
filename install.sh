#!/bin/bash

cp -R bin/ /opt/QMPQ
ln -s /opt/QMPQ/QMPQ /usr/bin/QMPQ
ln -s /opt/QMPQ/libCore.so.1 /usr/lib/libCore.so.1
ln -s /opt/QMPQ/libQMPQFileEngine.so.1 /usr/lib/libQMPQFileEngine.so.1
