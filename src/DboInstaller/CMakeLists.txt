#Link directories
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../WebWidgets
)

#Get Files Lists
FILE(GLOB_RECURSE DboInstaller_SRCS *.cpp *.h)

#Set Target/Project name
SET(WW_PROJECT_TARGET DboInstaller)

#Add Program CXX files
ADD_LIBRARY(${WW_PROJECT_TARGET} ${DboInstaller_SRCS})

#Parallel compile
IF(MSVC)
	SET_TARGET_PROPERTIES(${WW_PROJECT_TARGET} PROPERTIES COMPILE_FLAGS "${BUILD_PARALLEL} /bigobj /wd4297")
ENDIF(MSVC)
