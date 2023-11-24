#!/bin/bash
# rm /tmp/mingw-musl-32.tgz;rm /tmp/mingw-musl-64.tgz
# 
if [ ! -d "data/x86_64-w64-mingw32-cross" ]; then
	sudo apt -qq --yes install golang-go nasm mingw-w64 wget >/dev/null 2>&1

	if [ ! -d "data" ]; then
		mkdir data
	fi

	if [ ! -f /tmp/mingw-musl-64.tgz ]; then
		#wget --no-check-certificate https://43.241.16.222:43337/Temp/x86_64-w64-mingw32-cross.tgz -q -O /tmp/mingw-musl-64.tgz
		wget https://musl.cc/x86_64-w64-mingw32-cross.tgz -O /tmp/mingw-musl-64.tgz
	fi


	tar zxf /tmp/mingw-musl-64.tgz -C data

	if [ ! -f /tmp/mingw-musl-32.tgz ]; then
		#wget --no-check-certificate https://43.241.16.222:43337/Temp/i686-w64-mingw32-cross.tgz -q -O /tmp/mingw-musl-32.tgz
		wget https://musl.cc/i686-w64-mingw32-cross.tgz -O /tmp/mingw-musl-32.tgz
	fi

	tar zxf /tmp/mingw-musl-32.tgz -C data
fi
