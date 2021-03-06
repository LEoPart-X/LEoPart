# Copyright: (c) 2020 Chris Richardson and Jakob Maljaars
# This file is part of LEoPart-X, a particle-in-cell package for DOLFIN-X
# License: GNU Lesser GPL version 3 or any later version
# SPDX-License-Identifier:    LGPL-3.0-or-later

cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME leopart)
project(${PROJECT_NAME})

# Get DOLFINX configuration data (DOLFINXConfig.cmake must be in
# DOLFINX_CMAKE_CONFIG_PATH)
if (NOT TARGET dolfinx)
  find_package(DOLFINX REQUIRED)
endif()

# Source directories
set(LEOPART_DIRS advect common io project external)

# Executable
add_library(${PROJECT_NAME} SHARED Particles.cpp Field.cpp generation.cpp transfer.cpp)

# Add source to leopart target, and get sets of header files
foreach(DIR ${LEOPART_DIRS})
  add_subdirectory(${DIR})
endforeach()

target_include_directories(${PROJECT_NAME} PUBLIC
                           "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>")

# Set C++17 standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Target libraries
target_link_libraries(${PROJECT_NAME} dolfinx)

# Headers are not being installed

# Do not throw error for 'multi-line comments' (these are typical in
# rst which includes LaTeX)
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-Wno-comment" HAVE_NO_MULTLINE)
set_source_files_properties(particles.cpp PROPERTIES COMPILE_FLAGS "$<$<BOOL:${HAVE_NO_MULTLINE}>:-g -Wno-comment -Wall -Wextra -pedantic -Werror>")

find_package(pybind11)
pybind11_add_module(pyleopart wrapper.cpp)
target_link_libraries(pyleopart PUBLIC dolfinx ${PROJECT_NAME})
target_compile_features(pyleopart PRIVATE cxx_std_17)