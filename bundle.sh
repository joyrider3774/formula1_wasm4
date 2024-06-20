#!/bin/sh

w4 bundle --windows formula-1-windows.exe  --title "Formula 1" build/cart.wasm
w4 bundle --mac formula-1-mac  --title "Formula 1" build/cart.wasm
w4 bundle --linux formula-1-linux  --title "Formula 1" build/cart.wasm
w4 bundle --html index.html  --title "Formula 1" build/cart.wasm
cp build/cart.wasm formula-1.wasm
chmod +x ./formula-1-mac
chmod +x ./formula-1-linux
chmod +x ./formula-1-windows.exe
tar -czvf ./formula-1-windows.tar.gz ./formula-1-windows.exe
tar -czvf ./formula-1-mac.tar.gz ./formula-1-mac
tar -czvf ./formula-1-linux.tar.gz ./formula-1-linux
tar -czvf ./formula-1-html.tar.gz ./index.html
tar -czvf ./formula-1-cart.tar.gz ./formula-1.wasm
rm -f ./formula-1-windows.exe
rm -f ./formula-1-mac
rm -f ./formula-1-linux
