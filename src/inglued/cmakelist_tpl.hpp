#ifndef INGLUED_CMAKELIST_TPL_HPP
#define INGLUED_CMAKELIST_TPL_HPP

namespace inglued {

  constexpr auto cmakelist_tpl = R"(
cmake_minimum_required(VERSION 3.5.2)

project({{project}} VERSION "0.0.1")
enable_testing()

set(CMAKE_CXX_STANDARD 14)

# Compile with shipped-with headers or without 
option(INGLUED "Enable use of #inglued shipped with dependencies." ON)

# Compile unit tests
option(UNIT_TESTS "Enable Unit Testing" OFF)


# Warning as errors to ensure {{project}} quality
string(TOUPPER "${CMAKE_CXX_COMPILER_ID}" COMPILER_IN_USE)
if ("${COMPILER_IN_USE}" STREQUAL "GNU" OR "${COMPILER_IN_USE}" MATCHES "CLANG")
	add_definitions(
    -Wall
		-Werror
		-Wno-unused-local-typedefs
		-Wno-unused-variable
  )
endif()



{{#deps}}
# {{name}}
if (INGLUED)
  include_directories(AFTER deps/{{org}}/{{name}}/{{include_path}})
else ()
  find_package({{cmake_package_name}} REQUIRED)
endif()

{{/deps}}

# Define header only library
add_library({{project}} INTERFACE)

set(include_install_dir "include")

if (INGLUED)

  target_include_directories({{project}} INTERFACE 
    # Transitive libraries location once installed
    $<INSTALL_INTERFACE:${include_install_dir}/{{project_srcs}}/deps> 
  )


else()

  target_link_libraries({{project}} INTERFACE 
    {{#deps}}
      {{cmake_target_name}}
    {{/deps}}
  )

endif()


if( UNIT_TESTS )
 add_subdirectory(test)
endif()

# IDE Supports (XCode, VS, QtCreator they don't list headers)
file (GLOB_RECURSE HPP_FILES {{project_srcs}}/*.h*)
add_custom_target(sources SOURCES ${HPP_FILES})

# Installing

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/modules/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)


# Targets:
install(
    TARGETS {{project}}
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY {{project_srcs}}
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.[ih]*"
)

# Install the deps when run in INGLUED mode
if (INGLUED)
  {{=<% %>=}}

  <%#deps%>

  install(
      DIRECTORY deps/<%org%>/<%name%>/<%include_path_end_backslash%>
      DESTINATION ${include_install_dir}/<%project_srcs%>/deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/<%org%>/<%name%>/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )

  <%/deps%>

  <%={{ }}=%>
endif(INGLUED)

# Config
#   * <prefix>/lib/cmake/{{project}}/{{project}}Config.cmake
#   * <prefix>/lib/cmake/{{project}}/{{project}}ConfigVersion.cmake
#   * <prefix>/lib/cmake/{{project}}/{{project}}Targets.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

)";


  constexpr auto cmakelist_package_config_tpl = R"(
@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/@targets_export_name@.cmake")
check_required_components("@PROJECT_NAME@")
)";

}

#endif
