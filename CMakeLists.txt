# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE $ENV{PSN00BSDK_LIBS}/cmake/sdk.cmake)

project(
	pone_quest
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "My Little PSX"
	HOMEPAGE_URL "https://muriloucolouco.github.io/"
)

psn00bsdk_add_executable(pone_quest STATIC main.c system.c texture.c model.c files.c player.c level.c)

psn00bsdk_add_cd_image(
	pone_quest_iso
	pone_quest
	iso.xml
	DEPENDS pone_quest
)