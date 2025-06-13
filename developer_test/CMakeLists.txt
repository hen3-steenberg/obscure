add_executable(OBSCURE_DEV_APP "testapp.cpp")

target_link_libraries(OBSCURE_DEV_APP PRIVATE obscure)
target_link_libraries(OBSCURE_DEV_APP PRIVATE imgui)

FILE(COPY "viking_room.obj" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
FILE(COPY "viking_room.png" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
