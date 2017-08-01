file(GLOB SOURCES "*.cpp")
add_library( SystemCommon ${SOURCES} )
target_link_libraries( SystemCommon config++ )