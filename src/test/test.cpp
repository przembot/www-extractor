#define BOOST_TEST_MODULE tkom_tests
#include <boost/test/unit_test.hpp>

std::ostream& operator<< (std::ostream& stream, const std::wstring& str) {
    stream << std::string(str.begin(), str.end()) << "std::wstring";
    return stream;
}

#include "extractorTest.cpp"
#include "querylexerTest.cpp"
#include "queryparserTest.cpp"
#include "htmllexerTest.cpp"
#include "htmlparserTest.cpp"
