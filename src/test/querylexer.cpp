#include "source.h"
#include "query.h"

const string& sampleQuery = "/* tag=!/";

BOOST_AUTO_TEST_CASE( query_lexer )
{
  QueryLexer<StringSource> q(sampleQuery);
  BOOST_TEST('a' == 'a');
}
