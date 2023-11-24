# Set library name
set(LIB_NAME orderbook)

# Define the library
add_library(${LIB_NAME} ${LIB_NAME}/${LIB_NAME}.cpp ${LIB_NAME}/${LIB_NAME}.hpp)

# Test name for the command library
set(TEST_TARGET ${LIB_NAME}_tests)

# Create executable
add_executable(${TEST_TARGET} ${LIB_NAME}/${LIB_NAME}.test.cpp)

# Link the library
target_link_libraries(${TEST_TARGET} PRIVATE ${LIB_NAME})
