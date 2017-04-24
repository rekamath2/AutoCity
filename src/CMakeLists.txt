include_directories(${autocity_includes})

set(wxWidgets_USE_LIBS)

FIND_PACKAGE(wxWidgets COMPONENTS core base propgrid gl REQUIRED)
if(NOT wxWidgets_FOUND)
	message(SEND_ERROR "Couldn't find wxWidgets")
endif()

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(assimp REQUIRED)

set(HDRS
	./acApp.h
	./acFrame.h
)
 
set(SRCS
	./acApp.cxx
	./acFrame.cxx
)

INCLUDE("${wxWidgets_USE_FILE}")

add_executable(autocity ${HDRS} ${SRCS})
 
set_target_properties(autocity PROPERTIES DEBUG_POSTFIX _d)

include_directories(${OPENGL_INCLUDE_DIRS} ${assimp_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})

target_link_libraries(autocity ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY}
	${assimp_LIBRARY})
