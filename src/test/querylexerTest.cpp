#include <vector>
#include <memory>

#include "source.h"
#include "querylexer.h"

const wstring& sampleQuery1 = L"/* tag=!/";
const Symbol sampleQueryTokens1[] = {
    {slashtk, L""}
  , {anytagtk, L""}
  , {stringtk, L"tag"}
  , {equalstk, L""}
  , {mustexisttk, L""}
  , {slashtk, L""}
  , {othertk, L""}
  };


const wstring& sampleQuery2 = L"/div id='item'/a class='price'/";
const Symbol sampleQueryTokens2[] = {
    {slashtk, L""}
  , {stringtk, L"div"}
  , {stringtk, L"id"}
  , {equalstk, L""}
  , {singlequotevaltk, L"item"}

  , {slashtk, L""}
  , {stringtk, L"a"}
  , {stringtk, L"class"}
  , {equalstk, L""}
  , {singlequotevaltk, L"price"}
  , {slashtk, L""}
  , {othertk, L""}
  };

wostream& operator<<(wostream& stream, const Symbol& s) {
  stream << s.first << L" " << s.second;
  return stream;
}

BOOST_AUTO_TEST_CASE( query_lexer_1 )
{
  QueryLexer q(make_unique<StringSource>(sampleQuery1));

  Symbol a;
  for (int i = 0; i < 7; ++i) {
    a = q.nextSymbol();
    BOOST_CHECK(a == sampleQueryTokens1[i]);
  }
}

BOOST_AUTO_TEST_CASE( query_lexer_2 )
{
  QueryLexer q(make_unique<StringSource>(sampleQuery2));

  Symbol a;
  for (int i = 0; i < 12; ++i) {
    a = q.nextSymbol();
    BOOST_CHECK(a == sampleQueryTokens2[i]);
  }
}
