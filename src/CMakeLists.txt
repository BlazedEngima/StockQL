# Get the source file subdirectories
set(SUBDIRECTORIES
  command
  inparser
  lru
  orderbook
  pager
)

# Add the source files
foreach(subdir ${SUBDIRECTORIES})
  add_subdirectory(${subdir})
endforeach()

# Main executable
add_executable(${PROJECT_NAME} ${PROJECT_NAME}/${PROJECT_NAME}.cpp)

# Link libraries
foreach(lib ${SUBDIRECTORIES})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${lib})
endforeach()
