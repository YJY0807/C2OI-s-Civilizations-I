@echo off
title Build CCI

echo Your G++ Version:
g++.exe -v

echo=
echo ---Compiling start-----------------

echo=
echo Compiling console.hpp:
echo     command: g++.exe -shared -g c:\Users\lilli\Documents\cpp\CCI\console.cpp -o c:\Users\lilli\Documents\cpp\CCI\console.dll -std=c++20 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc
g++.exe -shared -g c:\Users\lilli\Documents\cpp\CCI\console.cpp -o c:\Users\lilli\Documents\cpp\CCI\console.dll -std=c++20 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc

echo=
echo Compiling CCI.cpp:
echo     command: g++.exe console.dll -g c:\Users\lilli\Documents\cpp\CCI\CCI.cpp -o c:\Users\lilli\Documents\cpp\CCI\CCI.exe -std=c++20 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc
g++.exe console.dll -g c:\Users\lilli\Documents\cpp\CCI\CCI.cpp -o c:\Users\lilli\Documents\cpp\CCI\CCI.exe -std=c++20 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc

echo ---Compiling end-------------------
pause