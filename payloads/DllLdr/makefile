CCX64	= x86_64-w64-mingw32-gcc
CCX86	= i686-w64-mingw32-gcc

INCLUDE = -I Include
OPTIONS = -w -Wall -Wextra -masm=intel -fPIC -fno-asynchronous-unwind-tables $(INCLUDE)

EXECUTABLE_X64 = DllLdr.x64.o
RAWBINARY_X64  = ../DllLdr.x64.bin

all: x64
# 反射加载是不支持x86的,最好不用x86的东西了
x64: clean
	@ echo "[*] Compile x64 object file..."

	@ $(CCX64) -c Source/Entry.c -o $(EXECUTABLE_X64) $(OPTIONS)

#   if want to get symbol coff file, comment three line codes below to debug them
	@ echo "[*] Extract shellcode: $(RAWBINARY_X64)"
	@ python3 Scripts/extract.py -f $(EXECUTABLE_X64) -o $(RAWBINARY_X64)
	@ rm $(EXECUTABLE_X64)

clean:
	@ rm -rf Bin/*.o
	@ rm -rf Bin/*.bin
