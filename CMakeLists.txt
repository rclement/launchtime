cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

# if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#     set(CMAKE_OSX_ARCHITECTURES $(ARCHS_STANDARD_32_64_BIT))
#     set(CMAKE_OSX_DEPLOYMENT_TARGET 10.7)
# endif()

# Generate source file groups according to file paths
function(generate_source_group files toplevel)
    foreach(FILE ${files})
        get_filename_component(PARENT_DIR "${FILE}" PATH)
    
        # skip src or include and changes /'s to \\'s
        string(REGEX REPLACE "(\\./)?(src|include)/?" "" GROUP "${PARENT_DIR}")
        string(REPLACE "/" "\\" GROUP "${GROUP}")
   
        set(GROUP "${toplevel}\\${GROUP}")

        source_group("${GROUP}" FILES "${FILE}")
    endforeach()
endfunction()

project(Launchtime
    VERSION 0.1.0
    LANGUAGES C CXX
)

# Define this for out-of-source build
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(external)
add_subdirectory(src)

