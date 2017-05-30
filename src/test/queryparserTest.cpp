#include "source.h"
#include "querylexer.h"
#include "queryparser.h"
#include <vector>

const wstring sampleParseQuery1 = L"/* tag=!/";
const wstring sampleParseQuery2 = L"/div id='images'/img src=?///";

BOOST_AUTO_TEST_CASE( query_parser_1 )
{
  QueryLexer lexer(make_unique<StringSource>(sampleParseQuery1));
  QueryParser parser(lexer);

  qstart tree;
  tree.questionType = QuestionType::CONTENT_ONLY;

  qnode node;
  node.tagNameKnown = 0;
  node.tagname = L"";
  node.read_attributes[L"tag"] = L"!";

  tree.children.push_back(node);

  qstart result;
  try {
    parser.parse(&result);
    BOOST_CHECK(tree == result);
  } catch (QueryParseException e) {
    cout << e.what() << endl;
    BOOST_CHECK(false);
  }
}


BOOST_AUTO_TEST_CASE( query_parser_2 )
{
  QueryLexer lexer(make_unique<StringSource>(sampleParseQuery2));
  QueryParser parser(lexer);

  qstart tree;
  tree.questionType = QuestionType::EVERYTHING;

  qnode div;
  div.tagname = L"div";
  div.tagNameKnown = 1;
  div.read_attributes[L"id"] = L"images";

  qnode img;
  img.tagname = L"img";
  img.tagNameKnown = 1;
  img.unknown_attributes.push_back(L"src");

  tree.children.push_back(div);
  tree.children.push_back(img);

  qstart result;

  try {
    parser.parse(&result);
    BOOST_CHECK(tree == result);
  } catch (QueryParseException e) {
    cout << e.what() << endl;
    BOOST_CHECK(false);
  }
}
