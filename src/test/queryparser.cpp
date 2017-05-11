#include "source.h"
#include "querylexer.h"
#include "queryparser.h"
#include <vector>

const string& sampleParseQuery1 = "/* tag=!/";

BOOST_AUTO_TEST_CASE( query_parser_1 )
{
  QueryLexer lexer(make_unique<StringSource>(sampleParseQuery1));
  QueryParser parser(lexer);

  qstart tree;
  tree.questionType = 1;

  qnode node;
  node.tagNameKnown = 0;
  node.tagname = "";
  node.read_attributes["tag"] = "!";

  tree.children.push_back(&node);

  qstart result;
  try {
    parser.parse(&result);
    BOOST_TEST(tree == result);
  } catch (QueryParseException e) {
    cout << e.what() << endl;
    BOOST_TEST(false);
  }
}
