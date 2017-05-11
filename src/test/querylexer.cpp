#include <vector>
#include <memory>

#include "source.h"
#include "querylexer.h"

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
  , {singlequotevaltk, "item"}

  , {slashtk, ""}
  , {stringtk, "a"}
  , {stringtk, "class"}
  , {equalstk, ""}
  , {singlequotevaltk, "price"}
  , {slashtk, ""}
  , {othertk, ""}
  };

ostream& operator<<(ostream& stream, const Symbol& s) {
  stream << s.first << " " << s.second;
  return stream;
}

BOOST_AUTO_TEST_CASE( query_lexer_1 )
{
  QueryLexer q(make_unique<StringSource>(sampleQuery1));

  Symbol a;
  for (int i = 0; i < 7; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleQueryTokens1[i]);
  }
}

BOOST_AUTO_TEST_CASE( query_lexer_2 )
{
  QueryLexer q(make_unique<StringSource>(sampleQuery2));

  Symbol a;
  for (int i = 0; i < 12; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleQueryTokens2[i]);
  }
}
