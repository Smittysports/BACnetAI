
cmake_minimum_required(VERSION 3.10)

project(BACnetAI VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(BACnetAI
	src/main.cpp
	src/Networking.cpp
	bacnet_services/remote_device_management/src/WhoIsIAm.cpp
)

target_include_directories(BACnetAI PRIVATE
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/bacnet_services/remote_device_management/include
)

# Link libraries (add here if needed)
# target_link_libraries(BACnetAI PRIVATE some_library)


