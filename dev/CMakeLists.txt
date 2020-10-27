cmake_minimum_required(VERSION 3.13)
project(tuhh_intairnet_arq)

set(CMAKE_CXX_STANDARD 14)

# Remove if not needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include -I/usr/local/include/ -L/usr/local/Cellar/cppunit/1.15.1/lib")

add_library(tuhh_intairnet_arq SHARED tuhh_intairnet_arq.cpp tuhh_intairnet_arq.h L2Segment.cpp L2Segment.hpp L2SegmentHeader.cpp L2SegmentHeader.hpp SequenceNumber.cpp SequenceNumber.hpp SelectiveRepeatArq.cpp SelectiveRepeatArq.hpp SelectiveRepeatArqProcess.cpp SelectiveRepeatArqProcess.hpp TYPES.h IRlc.h)

add_executable(unittests tests/unittests.cpp tests/SelectiveRepeatArqTest.cpp tests/SequenceNumberTests.cpp tests/L2SegmentHeaderTest.cpp tests/SelectiveRepeatArqProcessTest.cpp tests/E2ETest.cpp tests/mock/MockHost.h tests/mock/MockHost.cpp tests/mock/MockRlc.h tests/mock/MockRlc.cpp tests/mock/MockMac.h tests/mock/MockMac.cpp tests/mock/Channel.h tests/mock/Channel.cpp)
target_link_libraries(unittests cppunit tuhh_intairnet_arq)
#target_link_libraries(unittests :libcppunit.so tuhh_intairnet_arq)