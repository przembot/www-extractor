#include "source.h"
#include "query.h"
#include <vector>

const string& sampleQuery1 = "/* tag=!/";
const Symbol sampleQueryTokens1[] = {
    {slashtk, ""}
  , {anytagtk, ""}
  , {stringtk, "tag"}
  , {equalstk, ""}
  , {mustexisttk, ""}
  , {slashtk, ""}
  , {othertk, ""}
  };


const string& sampleQuery2 = "/div id='item'/a class='price'/";
const Symbol sampleQueryTokens2[] = {
    {slashtk, ""}
  , {stringtk, "div"}
  , {stringtk, "id"}
  , {equalstk, ""}
  , {quotetk, ""}
  , {stringtk, "item"}
  , {quotetk, ""}

  , {slashtk, ""}
  , {stringtk, "a"}
  , {stringtk, "class"}
  , {equalstk, ""}
  , {quotetk, ""}
  , {stringtk, "price"}
  , {quotetk, ""}
  , {slashtk, ""}
  , {othertk, ""}
  };

ostream& operator<<(ostream& stream, const Symbol& s) {
  stream << s.first << " " << s.second;
  return stream;
}

BOOST_AUTO_TEST_CASE( query_lexer_1 )
{
  QueryLexer<StringSource> q(sampleQuery1);

  Symbol a;
  for (int i = 0; i < 7; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleQueryTokens1[i]);
  }
}

BOOST_AUTO_TEST_CASE( query_lexer_2 )
{
  QueryLexer<StringSource> q(sampleQuery2);

  Symbol a;
  for (int i = 0; i < 15; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleQueryTokens2[i]);
  }
}
