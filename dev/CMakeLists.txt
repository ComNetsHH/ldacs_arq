cmake_minimum_required(VERSION 3.13)
project(tuhh_intairnet_arq)

set(CMAKE_CXX_STANDARD 14)

add_library(tuhh_intairnet_arq SHARED tuhh_intairnet_arq.cpp tuhh_intairnet_arq.h L2Segment.cpp L2Segment.hpp L2SegmentHeader.cpp L2SegmentHeader.hpp SequenceNumber.cpp SequenceNumber.hpp SelectiveRepeatArq.cpp SelectiveRepeatArq.hpp)

add_executable(unittests tests/unittests.cpp tests/ArqTests.cpp tests/SequenceNumberTests.cpp)
target_link_libraries(unittests :libcppunit.so tuhh_intairnet_arq)