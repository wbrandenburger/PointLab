cmake_minimum_required(VERSION 3.7.0)
project(PointLab)

#------------------------------ Boost ------------------------------
find_package(BOOST REQUIRED)

#------------------------------ CUDA ------------------------------
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

# Pass options to NVCC
set(CUDA_64_BIT_DEVICE_CODE --ON)
set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -use_fast_math")

#------------------------------ Doxygen ------------------------------
find_package(Doxygen)

#------------------------------ Eigen ------------------------------
find_package(EIGEN REQUIRED)

#------------------------------ Flann ------------------------------
#find_package(FLANN)

#------------------------------ OpenGL ------------------------------
find_package(OpenGL REQUIRED)
find_package(FreeGlut REQUIRED)
find_package(GLEW REQUIRED)

#------------------------------ OpenCV ------------------------------
#find_package(OPENCV)

#------------------------------ OpenMP ------------------------------
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")	
endif (OPENMP_FOUND)

#------------------------------ RPly ------------------------------	
find_package(RPly REQUIRED)

#------------------------------ Sources ------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE TOOL_SOURCES "tools/*.cpp" "tools/*.c" "tools/*.cu")
file(GLOB_RECURSE TOOL_HEADERS "tools/*.hpp" "tools/*.h")

set(STANDARD_TARGETS pcsimp) #epivis gcptoheight gcpimgto3d gcptoimg

set(LIBRARY_TARGETS trees)

foreach (FORTARGETS ${LIBRARY_TARGETS})
	file(GLOB_RECURSE SOURCES "${FORTARGETS}/*.cpp" "${FORTARGETS}/*.c" "${FORTARGETS}/*.cu")
	file(GLOB_RECURSE HEADERS "${FORTARGETS}/*.hpp" "${FORTARGETS}/*.h")
	
	add_library(trees STATIC ${SOURCES} ${TOOL_SOURCES} ${HEADERS} ${TOOL_HEADERS})
	set_target_properties(trees PROPERTIES LINKER_LANGUAGE CXX)
endforeach()

foreach (FORTARGETS ${STANDARD_TARGETS})
	file(GLOB_RECURSE SOURCES "${FORTARGETS}/*.cpp" "${FORTARGETS}/*.c" "${FORTARGETS}/*.cu")
	file(GLOB_RECURSE HEADERS "${FORTARGETS}/*.hpp" "${FORTARGETS}/*.h")
	
	add_executable(${FORTARGETS} ${SOURCES} ${TOOL_SOURCES} ${HEADERS} ${TOOL_HEADERS})
endforeach()

foreach(FORSTANDARDTARGETS ${STANDARD_TARGETS})
	foreach(FORLIBRARYTARGETS ${LIBRARY_TARGETS})
		add_dependencies(${FORSTANDARDTARGETS} ${FORLIBRARYTARGETS})
	endforeach()
endforeach()

#set(CUDATARGETS xxx)

#foreach (FORTARGETS ${CUDATARGETS})
#   file(GLOB_RECURSE SOURCES "${FORTARGETS}/*.cpp" "${FORTARGETS}/*.c" "${FORTARGETS}/*.cu")
#	cuda_add_executable(${FORTARGETS} ${SOURCES} ${TOOL_SOURCES})
#endforeach()

#set(GCPTARGETS epivis gcptoheight gcpimgto3d gcptoimg)
#foreach (FORTARGETS ${GCPTARGETS})
#	set_target_properties(${FORTARGETS} PROPERTIES FOLDER "gcp")
#endforeach()

set(TARGETS ${STANDARD_TARGETS} ${LIBRARY_TARGETS} )

#------------------------------ Include ------------------------------ 
foreach(FORTARGETS ${TARGETS})
	#target_include_directories(${FORTARGETS} PUBLIC "${FORTARGETS}")
	target_include_directories(${FORTARGETS} PUBLIC "tools")
	target_include_directories(${FORTARGETS} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

# ------------------------------ Libraries ------------------------------
foreach(FORTARGETS ${TARGETS})
	
	# ---------------------------- Boost ----------------------------
	foreach (FOR_RELEASE ${BOOST_RELEASE_DIR})
		target_link_libraries(${FORTARGETS} optimized ${FOR_RELEASE})
	endforeach()

	# ---------------------------- CUDA ----------------------------
	target_link_libraries(${FORTARGETS} ${CUDA_cudart_static_LIBRARY})
	
	# ---------------------------- FLANN ----------------------------
	#target_link_libraries(${FORTARGETS} ${FLANN_LIBRARY})
	#target_link_libraries(${FORTARGETS} ${FLANN_LIBRARY_CPP} )
	#target_link_libraries(${FORTARGETS} ${FLANN_LIBRARY_CUDA} )
	
	# ---------------------------- OpenGL ----------------------------
	target_link_libraries(${FORTARGETS} ${OPENGL_gl_LIBRARY})
	
	# ---------------------------- FreeGlut ----------------------------
	target_link_libraries(${FORTARGETS} ${FREEGLUT_LIBRARY})
	target_link_libraries(${FORTARGETS} ${FREEGLUT_LIBRARY_STATIC})
	
	# ---------------------------- Glew ----------------------------
	target_link_libraries(${FORTARGETS} ${GLEW_LIBRARY_32})
	target_link_libraries(${FORTARGETS} ${GLEW_LIBRARY_32S})
	
	# ---------------------------- OpenCV ----------------------------
	#foreach (FOR_RELEASE ${OPENCV_RELEASE_DIR})
	#	target_link_libraries(${FORTARGETS} optimized ${FOR_RELEASE})
	#endforeach()
	
	# ---------------------------- RPly ----------------------------
	target_link_libraries(${FORTARGETS} ${RPLY_LIBRARY})
	
endforeach()

foreach(FORSTANDARDTARGETS ${STANDARD_TARGETS})
	foreach(FORLIBRARYTARGETS ${LIBRARY_TARGETS})
		target_link_libraries(${FORSTANDARDTARGETS} ${CMAKE_CURRENT_BINARY_DIR}/Release/${FORLIBRARYTARGETS}.lib)
	endforeach()
endforeach()
	
# ------------------------------ Install properties ------------------------------
include(GNUInstallDirs) #Defines the folders for installing the libraries

set(CMAKE_INSTALL_PREFIX $ENV{PROJECT_ROOT}/${CMAKE_PROJECT_NAME})

foreach (FORTARGETS ${LIBRARY_TARGETS})
install(TARGETS ${FORTARGETS} DESTINATION ${CMAKE_INSTALL_PREFIX}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/${FORTARGETS}
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	FILES_MATCHING PATTERN "*.h*")
endforeach()

install (
    DIRECTORY ${CMAKE_SOURCE_DIR}/tools
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h*")	

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc)
	
# ------------------------------ Addition ------------------------------
add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Preprocessor Definitions
 
# ------------------------------ Doxygen ------------------------------
SET(DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/index.html)
SET(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)

ADD_CUSTOM_COMMAND(
  OUTPUT ${DOXYGEN_OUTPUT}
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${DOXYGEN_INPUT}
  )
  
ADD_CUSTOM_TARGET(doc ALL DEPENDS ${DOXYGEN_OUTPUT})

set_target_properties(doc PROPERTIES EXCLUDE_FROM_ALL TRUE)
