find_package(spdlog REQUIRED)

file(GLOB thread_pool_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB hello_sources ${CMAKE_CURRENT_SOURCE_DIR}/test/*cc)


add_library(thread_pool STATIC ${thread_pool_sources})
target_include_directories(thread_pool PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(thread_pool PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(thread_pool PRIVATE spdlog::spdlog)

add_executable(hello ${hello_sources})
target_include_directories(hello PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(hello thread_pool)
