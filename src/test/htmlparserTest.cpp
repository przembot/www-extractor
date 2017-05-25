#include <vector>

#include "source.h"
#include "htmllexer.h"
#include "htmlparser.h"

const string sampleParseHtml1 =
"<html>\
<body>\
<img nojava src=\'kotek.jpg\' />\
Some content\
</body>\
</html>";

const string sampleParseHtml2 =
"<html>\
<script type='javascript>\
var i = \"lol\";\
for (var i = 0; i < 10; i++)\
    alert(i);\
</script>\
<body>\
some content\
</body>\
</html>";

const string sampleParseHtml3 =
"<html>\
<head>\
<meta charset>\
</head>\
<body>\
<div>\
some content\
<div>\
more content\
</div>\
</body>\
</html>";

BOOST_AUTO_TEST_CASE( html_parser_1 )
{
  HtmlLexer lexer(make_unique<StringSource>(sampleParseHtml1));
  HtmlParser parser(lexer);

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

  html.children.push_back(&body);
    body.children.push_back(&img);
    body.children.push_back(&text);
  sample.nodes.push_back(&html);

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_TEST(sample == result);
}

BOOST_AUTO_TEST_CASE( html_parser_2 )
{
  HtmlLexer lexer(make_unique<StringSource>(sampleParseHtml2));
  HtmlParser parser(lexer);

  Htmlstart sample;

  Htmlnode html;
  html.tag_name = "html";
  Htmlnode body;
  body.tag_name = "body";

  Textnode text;
  text.content = "some content";

  html.children.push_back(&body);
    body.children.push_back(&text);
  sample.nodes.push_back(&html);

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_TEST(sample == result);
}

BOOST_AUTO_TEST_CASE( html_parser_3 )
{
  HtmlLexer lexer(make_unique<StringSource>(sampleParseHtml3));
  HtmlParser parser(lexer);

  Htmlstart sample;

  Htmlnode html;
  html.tag_name = "html";
  Htmlnode body;
  body.tag_name = "body";
  Htmlnode head;
  head.tag_name = "head";

  Emptyhtmlnode meta;
  meta.tag_name = "meta";
  meta.attributes["charset"] = "";

  Htmlnode div1;
  div1.tag_name = "div";

  Htmlnode div2;
  div2.tag_name = "div";

  Textnode text1;
  text1.content = "some content";

  Textnode text2;
  text2.content = "more content";

  html.children.push_back(&head);
    head.children.push_back(&meta);
  html.children.push_back(&body);
    body.children.push_back(&div1);
      div1.children.push_back(&text1);
      div1.children.push_back(&div2);
        div2.children.push_back(&text2);
  sample.nodes.push_back(&html);

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_TEST(sample == result);
}
