cmake_minimum_required(VERSION 3.8)

project(POS_SEM VERSION 1.0)

# Set the APPLICATION_ID
set(APPLICATION_ID "org.gtkmm.ApplicationTemplate")
string(REPLACE "." "/" APPLICATION_PREFIX "/${APPLICATION_ID}/")

# Setup the resource installation files
list(INSERT CMAKE_MODULE_PATH 0 ${PROJECT_SOURCE_DIR}/cmake)
include(GlibUtils)
include(InstallScript)

# Set the origin files for the resource installation
set(GENERATED_FILES_DIR ${PROJECT_BINARY_DIR}/generated)
set(GENERATED_SOURCES_DIR ${GENERATED_FILES_DIR}/src)
set(GENERATED_DATA_DIR ${GENERATED_FILES_DIR}/data)

# Link the resource paths to the projectdefinitions
configure_file(cmake/projectdefinitions.h.in ${GENERATED_SOURCES_DIR}/projectdefinitions.h)

# Init gtkmm-4.0
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

# Link gtkmm-4.0 libraries
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# Include directories in the project
add_subdirectory(src)
add_subdirectory(data)

# Set the uninstall script to work
install(FILES cmake/uninstall.sh DESTINATION ${PROJECT_BINARY_DIR} PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE)
