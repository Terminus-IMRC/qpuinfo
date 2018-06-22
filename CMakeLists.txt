# For CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT
cmake_minimum_required(VERSION 3.7.2)

project(qpuinfo)
include(CTest)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "Sugizaki Yukimasa <ysugi@idein.jp>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
        "Show information of VideoCore IV V3D on Raspberry Pi")

# See https://semver.org/ and https://www.pakin.org/~scott/ltver.html .
set(CPACK_PACKAGE_VERSION_MAJOR 0)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)

# CMAKE_INSTALL_PREFIX and CPACK_PACKAGING_INSTALL_PREFIX must be identical
# because the install prefix is used in the pkg-config file which will be
# auto-generated.
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install prefix" FORCE)
endif()
set(CPACK_PACKAGING_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
message(STATUS "Install prefix is set to ${CMAKE_INSTALL_PREFIX}")
include(CPack)

find_package(PkgConfig)

pkg_check_modules(VC4REGMAP libvc4regmap>=0.1.0)
if (NOT VC4REGMAP_FOUND)
    message(STATUS "Adding /opt/vc/lib/pkgconfig to PKG_CONFIG_PATH")
    set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/opt/vc/lib/pkgconfig")
    pkg_check_modules(VC4REGMAP libvc4regmap>=0.1.0)
    if (NOT VC4REGMAP_FOUND)
        message (FATAL_ERROR "libvc4regmap not found even in /opt/vc/lib. "
                             "Building on non-RPi host? "
                             "Please specify PKG_CONFIG_PATH.")
    endif ()
endif ()

pkg_check_modules(MAILBOX REQUIRED libmailbox>=2.0.0)

add_subdirectory(src)
