cmake_minimum_required(VERSION 3.10)
set(MODULE NodeFlow)
project (${MODULE})

set(CMAKE_INSTALL_PREFIX ${PIROOT}/usr/local/MRL5/lib)

set(CMAKE_CXX_STANDARD 17)
add_definitions (-DUNICODE -D_UNICODE)

## build settings
set(VERSION_MAJOR 1)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})


file (GLOB HeaderFiles *.h)
file (GLOB SourceFiles *.cpp)



add_library(${MODULE} SHARED ${SourceFiles} )

target_link_libraries(${MODULE} ${wxWidgets_LIBRARIES}  ${Boost_LIBRARIES} ${EXTRA_LIB_FLAGS} ${PROJECT_LIB_FLAGS})

install(TARGETS ${MODULE} DESTINATION lib)

install(FILES ${HeaderFiles} DESTINATION include/${MODULE})



