#
# Copyright (c) 2020 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

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

if(GEN_UF2)
  find_package(Python3 REQUIRED COMPONENTS Interpreter)

  set(UF2CONV ${CMAKE_CURRENT_SOURCE_DIR}/uf2conv.py)
  set(ZEPHYR_BIN ${CMAKE_BINARY_DIR}/zephyr/zephyr.bin)
  set(ZEPHYR_UF2 ${CMAKE_BINARY_DIR}/zephyr/zephyr.uf2)

  if(NOT EXISTS ${UF2CONV})
    message(FATAL_ERROR "uf2conv.py not found: ${UF2CONV}")
  endif()

  add_custom_command(
    OUTPUT ${ZEPHYR_UF2}
    COMMAND ${Python3_EXECUTABLE}
            ${UF2CONV}
            ${ZEPHYR_BIN}
            -c
            -b 0x1000
            -f NRF52840
            -o ${ZEPHYR_UF2}
    DEPENDS ${ZEPHYR_BIN}
    COMMENT "Generating zephyr.uf2 from zephyr.bin"
    VERBATIM
  )

  add_custom_target(gen_uf2 ALL
    DEPENDS ${ZEPHYR_UF2}
  )
endif()

