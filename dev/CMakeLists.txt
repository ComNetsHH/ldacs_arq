cmake_minimum_required(VERSION 3.1)
project(tuhh_intairnet_arq)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(glue-lib-headers)
include_directories(glue-lib-headers)

set(TARGET_SRC SelectiveRepeatArq.cpp SelectiveRepeatArq.hpp SelectiveRepeatArqProcess.cpp SelectiveRepeatArqProcess.hpp)
set(TARGET_TEST_SRC tests/unittests.cpp tests/SelectiveRepeatArqTest.cpp tests/SelectiveRepeatArqProcessTest.cpp tests/End2EndTest.cpp tests/MockRlc.cpp tests/MockRlc.hpp)
set(TARGET_INCL glue-lib-headers/CPRPosition.hpp glue-lib-headers/IArq.hpp glue-lib-headers/IMac.hpp glue-lib-headers/IRlc.hpp glue-lib-headers/L2Header.hpp glue-lib-headers/L2Packet.hpp glue-lib-headers/MacId.hpp glue-lib-headers/SequenceNumber.cpp glue-lib-headers/SequenceNumber.hpp PacketUtils.cpp PacketUtils.hpp PacketFragment.hpp)

add_library(tuhh_intairnet_arq SHARED ${TARGET_SRC} ${TARGET_INCL} ${GLUE_SRC_HPP})
target_link_libraries(tuhh_intairnet_arq PUBLIC intairnet_linklayer_glue)

add_executable(unittests ${TARGET_TEST_SRC} ${TARGET_SRC} ${GLUE_SRC_HPP})
find_library(CPPUNITLIB cppunit)
target_include_directories(unittests PUBLIC /opt/homebrew/opt/cppunit/include)
target_link_libraries(unittests ${CPPUNITLIB} tuhh_intairnet_arq)
