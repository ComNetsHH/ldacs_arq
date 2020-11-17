cmake_minimum_required(VERSION 3.13)
project(tuhh_intairnet_arq)

set(CMAKE_CXX_STANDARD 14)
include_directories(glue-lib-headers)

# Remove if not needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include -I/usr/local/include/ -L/usr/local/Cellar/cppunit/1.15.1/lib")
set(TARGET_SRC tuhh_intairnet_arq.cpp tuhh_intairnet_arq.h L2Segment.cpp L2Segment.hpp L2SegmentHeader.cpp L2SegmentHeader.hpp SequenceNumber.cpp SequenceNumber.hpp SelectiveRepeatArq.cpp SelectiveRepeatArq.hpp SelectiveRepeatArqProcess.cpp SelectiveRepeatArqProcess.hpp TYPES.h)
set(TARGET_INCL glue-lib-headers/CPRPosition.hpp glue-lib-headers/IArq.hpp glue-lib-headers/IMac.hpp glue-lib-headers/IRlc.hpp glue-lib-headers/L2Header.hpp glue-lib-headers/L2Packet.hpp glue-lib-headers/MacId.hpp glue-lib-headers/SequenceNumber.cpp glue-lib-headers/SequenceNumber.hpp)

add_library(tuhh_intairnet_arq SHARED ${TARGET_SRC} ${TARGET_INCL})
find_library(GLUE_LIBRARY intairnet_linklayer_glue HINTS glue-lib)
target_link_libraries(tuhh_intairnet_arq PUBLIC ${GLUE_LIBRARY})

add_executable(unittests tests/unittests.cpp tests/SelectiveRepeatArqTest.cpp tests/SequenceNumberTests.cpp tests/L2SegmentHeaderTest.cpp tests/SelectiveRepeatArqProcessTest.cpp)
target_link_libraries(unittests cppunit tuhh_intairnet_arq)
#target_link_libraries(unittests :libcppunit.so tuhh_intairnet_arq)