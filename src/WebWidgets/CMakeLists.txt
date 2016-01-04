#Link Directories
INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/..
)

#Get Files Lists
FILE(GLOB_RECURSE sources_objects Objects/*.cpp Objects/*.h)
FILE(GLOB_RECURSE sources_dbo Dbo/*.cpp Dbo/*.h)
FILE(GLOB_RECURSE sources_dbodatabase DboDatabase/*.cpp DboDatabase/*.h)
FILE(GLOB_RECURSE sources_dboinstaller DboInstaller/*.cpp DboInstaller/*.h)
FILE(GLOB_RECURSE sources_application Application/*.cpp Application/*.h)
FILE(GLOB_RECURSE sources_pages Pages/*.cpp Pages/*.h)
FILE(GLOB_RECURSE sources_widgets Widgets/*.cpp Widgets/*.h)
FILE(GLOB sources_localdir *.cpp *.h)
#Group File
SOURCE_GROUP(Objects FILES ${sources_objects})
SOURCE_GROUP(Dbo FILES ${sources_dbo})
SOURCE_GROUP(DboDatabase FILES ${sources_dbodatabase})
SOURCE_GROUP(DboInstaller FILES ${sources_dboinstaller})
SOURCE_GROUP(Application FILES ${sources_application})
SOURCE_GROUP(PageHandlers FILES ${sources_pages})
SOURCE_GROUP(Widgets FILES ${sources_widgets})
SOURCE_GROUP("" FILES ${sources_localdir})

#Concatenate Files Lists
SET(WebWidgets_SRCS
	${sources_objects}
	${sources_dbo}
	${sources_dbodatabase}
	${sources_dboinstaller}
	${sources_application}
	${sources_pages}
	${sources_widgets}
	${sources_localdir}
)
#Set Target/Project name
SET(WW_PROJECT_TARGET WebWidgets)

#Add Program CXX files
ADD_EXECUTABLE(${WW_PROJECT_TARGET} ${WebWidgets_SRCS})

#Parallel compile
IF(MSVC)
	SET_TARGET_PROPERTIES(${WW_PROJECT_TARGET} PROPERTIES COMPILE_FLAGS "${BUILD_PARALLEL} /bigobj /wd4297")
ENDIF(MSVC)

#Link Libraries
TARGET_LINK_LIBRARIES(${WW_PROJECT_TARGET} DboInstaller ${MYSQL_LIBRARIES})

IF(Wt_FOUND)
	TARGET_LINK_LIBRARIES(
		optimized ${Wt_LIBRARY}
		optimized ${Wt_HTTP_LIBRARY}
		optimized ${Wt_DBO_LIBRARY}
		optimized ${Wt_DBOSQLITE3_LIBRARY}
		optimized ${Wt_DBOMYSQL_LIBRARY}
	)
ENDIF(Wt_FOUND)

IF(Wt_DEBUG_FOUND)
	TARGET_LINK_LIBRARIES(
		debug ${Wt_DEBUG_LIBRARY}
		debug ${Wt_HTTP_DEBUG_LIBRARY}
		debug ${Wt_DBO_DEBUG_LIBRARY}
		debug ${Wt_DBOSQLITE3_DEBUG_LIBRARY}
		debug ${Wt_DBOMYSQL_DEBUG_LIBRARY} ${MYSQL_LIBRARIES}
	)
ENDIF(Wt_DEBUG_FOUND)