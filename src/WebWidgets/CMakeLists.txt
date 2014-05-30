#Link Directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

#Get Files Lists
FILE(GLOB_RECURSE sources_cmake CMakeFiles/*.cmake)
FILE(GLOB_RECURSE sources_objects Objects/*.cpp Objects/*.h)
FILE(GLOB_RECURSE sources_dbo Dbo/*.cpp Dbo/*.h)
FILE(GLOB_RECURSE sources_dbodatabase DboDatabase/*.cpp DboDatabase/*.h)
FILE(GLOB_RECURSE sources_dboinstaller DboInstaller/*.cpp DboInstaller/*.h)
FILE(GLOB_RECURSE sources_application Application/*.cpp Application/*.h)
FILE(GLOB sources_localdir *.cpp *.h)
#Group File
SOURCE_GROUP(CMake FILES CMakeLists.txt ${sources_cmake})
SOURCE_GROUP(Objects FILES ${sources_objects})
SOURCE_GROUP(Dbo FILES ${sources_dbo})
SOURCE_GROUP(DboDatabase FILES ${sources_dbodatabase})
SOURCE_GROUP(DboInstaller FILES ${sources_dboinstaller})
SOURCE_GROUP(Application FILES ${sources_application})
SOURCE_GROUP("" FILES ${sources_localdir})

#Concatenate Files Lists
SET(WebWidgets_SRCS
	${sources_cmake}
	${sources_objects}
	${sources_dbo}
	${sources_dbodatabase}
	${sources_dboinstaller}
	${sources_application}
	${sources_localdir}
)
#Set Target/Project name
SET(WT_PROJECT_TARGET WebWidgets)

#Add Program CXX files
ADD_EXECUTABLE(${WT_PROJECT_TARGET} ${WebWidgets_SRCS})

#Parallel compile
IF(MSVC)
	SET_TARGET_PROPERTIES(WebWidgets PROPERTIES COMPILE_FLAGS "${BUILD_PARALLEL}")
ENDIF(MSVC)

#Link Libraries
TARGET_LINK_LIBRARIES(${WT_PROJECT_TARGET}
	wthttp
	wt
	wtdbo
	wtdbosqlite3
	wtdbomysql
#	wtdbofirebird
#	wtdbopostgres
)