#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hiredis::hiredis" for configuration "Release"
set_property(TARGET hiredis::hiredis APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(hiredis::hiredis PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhiredis.so.1.1.0"
  IMPORTED_SONAME_RELEASE "libhiredis.so.1.1.0"
  )

list(APPEND _cmake_import_check_targets hiredis::hiredis )
list(APPEND _cmake_import_check_files_for_hiredis::hiredis "${_IMPORT_PREFIX}/lib/libhiredis.so.1.1.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
