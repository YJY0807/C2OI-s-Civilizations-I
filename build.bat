@echo off
title Build CCI


set /p PROCESSOR=Enter your system digit (32 or 64):
if /i "%PROCESSOR%"=="64" ( goto :x64 )
goto :x86

:x64
echo Your G++ Version:
g++.exe -v

echo=
echo ---Compiling start-----------------

echo=
echo Compiling conkey.cpp:
echo     command: g++.exe -shared -g conkey.cpp -o conkey.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc
g++.exe -shared -g conkey.cpp -o conkey.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc

echo=
echo Compiling fastout.cpp:
echo     command: g++.exe -shared -g fastout.cpp -o fastout.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc
g++.exe -shared -g fastout.cpp -o fastout.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc

echo=
echo Compiling CCI.cpp:
echo     command: g++.exe fastout.dll conkey.dll -g CCI.cpp -o CCI.exe -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc
g++.exe fastout.dll conkey.dll -g CCI.cpp -o CCI.exe -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m64 -static-libgcc


echo ---Compiling end-------------------
pause
goto :end


:x86
echo Your G++ Version:
g++.exe -v

echo=
echo ---Compiling start-----------------

echo=
echo Compiling conkey.cpp:
echo     command: g++.exe -shared -g conkey.cpp -o conkey.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc
g++.exe -shared -g conkey.cpp -o conkey.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc

echo=
echo Compiling fastout.cpp:
echo     command: g++.exe -shared -g fastout.cpp -o fastout.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc
g++.exe -shared -g fastout.cpp -o fastout.dll -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc

echo=
echo Compiling CCI.cpp:
echo     command: g++.exe fastout.dll conkey.dll -g CCI.cpp -o CCI.exe -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc
g++.exe fastout.dll conkey.dll -g CCI.cpp -o CCI.exe -std=c++14 -Ofast -Wl,-stack=1073741824 -lm -I. -march=native -Wall -fno-asm -pipe -m32 -static-libgcc

echo ---Compiling end-------------------
pause
goto :end

:end
start CCI.exe
