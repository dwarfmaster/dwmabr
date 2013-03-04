
# On s'assure d'avoir la bonne version
cmake_minimum_required(VERSION 2.6)

# Déclaration du projet
project(wmabr)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(executable dwmabr)

# Inclusion de x11
include(FindX11)
find_package(X11 REQUIRED)
link_directories(${X11_LIBRARY_DIRS})
include_directories(${X11_INCLUDE_DIRS})

# On trouve les fichiers sources
file(
	GLOB_RECURSE
	source_files
	src/*.c
	src/*.h
	)

# L'exécutable et les sources
add_executable(
	${executable}
	${source_files}
	)

# Le linkage
target_link_libraries(
	${executable}
	${X11_LIBRARIES}
	)

