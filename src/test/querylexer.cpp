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

ostream& operator<<(ostream& stream, const Symbol& s) {
  stream << s.first << " " << s.second;
  return stream;
}

BOOST_AUTO_TEST_CASE( query_lexer )
{
  QueryLexer<StringSource> q(sampleQuery1);

  Symbol a;
  for (int i = 0; i < 6; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleQueryTokens1[i]);
  }

}
