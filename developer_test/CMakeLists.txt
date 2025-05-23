add_executable(OBSCURE_DEV_APP "testapp.cpp")

target_link_libraries(OBSCURE_DEV_APP PRIVATE obscure)

FILE(COPY "texture.jpg" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
