#include "source.h"
#include "htmllexer.h"
#include "htmlparser.h"
#include <vector>

const string& sampleParseHtml1 =
"<html>\
<body>\
<img nojava src=\'kotek.jpg\' />\
Some content\
</body>\
</html>";

BOOST_AUTO_TEST_CASE( html_parser_1 )
{
  HtmlLexer<StringSource> lexer(sampleParseHtml1);
  HtmlParser<StringSource> parser(lexer);

  Htmlstart sample;

  Htmlnode html;
  html.tag_name = "html";
  Htmlnode body;
  body.tag_name = "body";

  Emptyhtmlnode img;
  img.tag_name = "img";
  img.attributes["nojava"] = "";
  img.attributes["src"] = "kotek.jpg";

  Textnode text;
  text.content = "Some content";

  body.children.push_back(&img);
  body.children.push_back(&text);
  html.children.push_back(&body);
  sample.nodes.push_back(&html);

  Htmlstart result;
  try {
    parser.parse(&result);
    BOOST_TEST(sample == result);
  } catch (HtmlParseException e) {
    cout << e.what() << endl;
    BOOST_TEST(false);
  }
}
