#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

# Copy uf2/uf2.conf to the project root to enable the bootloader/UF2 build.
# A pristine build is required after adding or removing the root uf2.conf.
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/uf2.conf")
  list(APPEND EXTRA_CONF_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/uf2.conf"
  )
endif()

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(zigbee)

# NORDIC SDK APP START
target_sources(app PRIVATE
  src/main.c
  src/zephyr_drivers.c
  src/zb_zcl_struct.c
  #sdk/zigbee_app_utils.c
)

target_include_directories(app PRIVATE include)
