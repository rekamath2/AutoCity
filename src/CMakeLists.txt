include_directories(${autocity_includes})

set(wxWidgets_USE_LIBS)

FIND_PACKAGE(wxWidgets COMPONENTS core base propgrid gl REQUIRED)
if(NOT wxWidgets_FOUND)
	message(SEND_ERROR "Couldn't find wxWidgets")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(ASSIMP REQUIRED)
FIND_PACKAGE(CGAL REQUIRED)
FIND_PACKAGE(GLM REQUIRED)

set(HDRS
	./MainWindow.h
	./Mesh130.h
	./Mesh.h
	./Model130.h
	./Model.h
	./MwxGLCanvas_Mouse.h
	./OpenGLWindow.h
	./Shader.h
	./acHouseEngine.h
	./Constraints.h
)
 
set(SRCS
	./main.cxx
	./MainWindow.cxx
	./MwxGLCanvas_Mouse.cxx
	./OpenGLWindow.cxx
	./acHouseEngine.cxx
	./Constraints.cxx
)

INCLUDE("${wxWidgets_USE_FILE}")

add_executable(autocity ${HDRS} ${SRCS})
 
set_target_properties(autocity PROPERTIES DEBUG_POSTFIX _d)

include_directories(${OPENGL_INCLUDE_DIRS} ${ASSIMP_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${CGAL_INCLUDE_DIR})

target_link_libraries(autocity ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARY}
	${ASSIMP_LIBRARIES} ${CGAL_LIBRARIES})
