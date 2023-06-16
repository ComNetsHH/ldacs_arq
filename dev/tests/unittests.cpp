// The L-Band Digital Aeronautical Communications System (LDACS) Automatic Repeat Request (ARQ) implements the ARQ protocol for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "SelectiveRepeatArqTest.cpp"
#include "SelectiveRepeatArqProcessTest.cpp"
#include "PacketUtilsTest.cpp"
#include "End2EndTest.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    CppUnit::TextUi::TestRunner runner;

    //runner.addTest(PacketUtilsTest::suite());
    //runner.addTest(SelectiveRepeatArqProcessTest::suite());
    runner.addTest(End2EndTest::suite());

    runner.run();
    return runner.result().wasSuccessful() ? 0 : 1;
}
