include_directories(${autocity_includes})

set(wxWidgets_USE_LIBS)

FIND_PACKAGE(wxWidgets COMPONENTS core base propgrid gl REQUIRED)
if(NOT wxWidgets_FOUND)
	message(SEND_ERROR "Couldn't find wxWidgets")
endif()

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
 
target_link_libraries(autocity ${wxWidgets_LIBRARIES} ${GDK_LIBRARY})
