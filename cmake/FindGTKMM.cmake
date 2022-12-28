find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
add_library(GTKMM INTERFACE IMPORTED)
set_property(TARGET GTKMM PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${GTKMM_INCLUDE_DIRS})
set_property(TARGET GTKMM PROPERTY INTERFACE_LINK_LIBRARIES ${GTKMM_LIBRARIES})
set_property(TARGET GTKMM PROPERTY INTERFACE_COMPILE_OPTIONS ${GTKMM_CFLAGS_OTHER})
