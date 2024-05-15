#!/bin/bash

function configurate_SDL()
{
	local SDL_FOLDER="${1}"
	local SDL_URL="${2}"
	local SDL_FILE="${3}"
	local SDL_MD5="${4}"
	local SDL_LIB="${5}"
	local SDL_CONFIGURE=${6}
	
	echo "SDL_FOLDER: ${SDL_FOLDER}"
	echo "SDL_URL: ${SDL_URL}"
	echo "SDL_FILE: ${SDL_FILE}"
	echo "SDL_LIB: ${SDL_LIB}"
	
	if [ ! -d "${SDL_FOLDER}" ]; then	
		if [ ! -f "${SDL_FILE}" ]; then
			echo "Download ${SDL_FOLDER}"
			echo ${SDL_URL} ${SDL_FILE}
			wget -O "${SDL_FILE}" "${SDL_URL}" 
		fi

		if [ -f "${SDL_FILE}" ]; then
			echo "Check MD5"
			md5sum -c <<< "${SDL_MD5}  ${SDL_FILE}"
			if [ ! $? = 0 ]; then
				echo "MD5 check failed!"
				exit
			fi

		   	echo "Uncompress ${SDL_FILE}"
			tar -xvzf ${SDL_FILE}
		fi
	fi

	if [ -d "${SDL_FOLDER}" ]; then
		cd ${SDL_FOLDER}
		if [ ! -f "${SDL_LIB}" ]; then
			./configure --host=aarch64-linux-gnu --prefix=/usr/aarch64-linux-gnu ${SDL_CONFIGURE}
			
			make clean
			make -j8
			if [ ! $? = 0 ]; then
				echo "Build failed!"
				exit
			fi
		fi
		make install
		cd ..
	fi
}

export SDL2_CONFIG=/usr/aarch64-linux-gnu/bin/sdl2-config

configurate_SDL "SDL2-2.28.1" "https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/SDL2-2.28.1.tar.gz" "SDL2-2.28.1.tar.gz" "ccd2cd02b2ad4f71c2588747028953c3" "build/.libs/libSDL2.a"
configurate_SDL "SDL2_image-2.6.3" "https://github.com/libsdl-org/SDL_image/releases/download/release-2.6.3/SDL2_image-2.6.3.tar.gz" "SDL2_image-2.6.3.zip" "191c3eb49cc2dce30abff81398a92e81" ".libs/libSDL2_image.a" "--disable-webp --disable-tif --disable-tif-shared"
configurate_SDL "SDL2_ttf-2.20.2" "https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.20.2/SDL2_ttf-2.20.2.tar.gz" "SDL_ttf-release-2.20.2.tar.gz" "4815838c87410346226983f4e0a85fd4" ".libs/libSDL2_ttf.a" " --enable-freetype-builtin  --enable-harfbuzz-builtin"
configurate_SDL "SDL2_mixer-2.6.3" "https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.6.3/SDL2_mixer-2.6.3.tar.gz" "SDL2_mixer-2.6.3.tar.gz" "42c50ace7099b43b630d09d871f054c3" "build/.libs/libSDL2_mixer.a"
