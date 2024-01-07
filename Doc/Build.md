构建记录  

### DllLdr  
DllLdr 会修复重定位\导入表的一段代码，把这段代码放到一个DLL的前面，然后这整段代码可以变成shellcode直接调用。

构建命令  
x86_64-w64-mingw32-gcc -c Source/Entry.c -o DllLdr.bin -w -Wall -Wextra -masm=intel -fPIC -fno-asynchronous-unwind-tables -I Include  
自己构建是因为这样有符号，方便调试，teamserver在构建的时候是直接把.text区段的代码剥离出来，没调试符号的。

### Shellcode  


构建命令
nasm -f win64 Source/Asm/x64/Asm.s -o Bin/Asm.x64.o;x86_64-w64-mingw32-gcc Source/*.c Bin/Asm.x64.o -o Shellcode.x64.exe -Os -fno-asynchronous-unwind-tables -nostdlib -fno-ident -fpack-struct=8 -falign-functions=1 -s -ffunction-sections -falign-jumps=1 -w -falign-labels=1 -fPIC -Wl,-TScripts/Linker.ld -Wl,-s,--no-seh,--enable-stdcall-fixup -IInclude -masm=intel

