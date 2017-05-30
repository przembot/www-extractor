#include <vector>

#include "source.h"
#include "htmllexer.h"
#include "htmlparser.h"

const wstring sampleParseHtml1 =
L"<html>\
<body>\
<img nojava src=\'kotek.jpg\' />\
Some content\
</body>\
</html>";

const wstring sampleParseHtml2 =
L"<html>\
<script type='javascript>\
var i = \"lol\";\
for (var i = 0; i < 10; i++)\
    alert(i);\
</script>\
<body>\
some content\
</body>\
</html>";

const wstring sampleParseHtml3 =
L"<html>\
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

  unique_ptr<Htmlnode> html = make_unique<Htmlnode>();
  html->tag_name = L"html";
  unique_ptr<Htmlnode> body = make_unique<Htmlnode>();
  body->tag_name = L"body";

  unique_ptr<Emptyhtmlnode> img = make_unique<Emptyhtmlnode>();
  img->tag_name = L"img";
  img->attributes[L"nojava"] = L"";
  img->attributes[L"src"] = L"kotek.jpg";

  unique_ptr<Textnode> text = make_unique<Textnode>();
  text->content = L"Some content";

    body->children.push_back(move(img));
    body->children.push_back(move(text));
  html->children.push_back(move(body));
  sample.nodes.push_back(move(html));

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_CHECK(sample == result);
}

BOOST_AUTO_TEST_CASE( html_parser_2 )
{
  HtmlLexer lexer(make_unique<StringSource>(sampleParseHtml2));
  HtmlParser parser(lexer);

  Htmlstart sample;

  unique_ptr<Htmlnode> html = make_unique<Htmlnode>();
  html->tag_name = L"html";
  unique_ptr<Htmlnode> body = make_unique<Htmlnode>();
  body->tag_name = L"body";

  unique_ptr<Textnode> text = make_unique<Textnode>();
  text->content = L"some content";

    body->children.push_back(move(text));
  html->children.push_back(move(body));
  sample.nodes.push_back(move(html));

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_CHECK(sample == result);
}

BOOST_AUTO_TEST_CASE( html_parser_3 )
{
  HtmlLexer lexer(make_unique<StringSource>(sampleParseHtml3));
  HtmlParser parser(lexer);

  Htmlstart sample;

  unique_ptr<Htmlnode> html = make_unique<Htmlnode>();
  html->tag_name = L"html";
  unique_ptr<Htmlnode> body = make_unique<Htmlnode>();
  body->tag_name = L"body";
  unique_ptr<Htmlnode> head = make_unique<Htmlnode>();
  head->tag_name = L"head";

  unique_ptr<Emptyhtmlnode> meta = make_unique<Emptyhtmlnode>();
  meta->tag_name = L"meta";
  meta->attributes[L"charset"] = L"";

  unique_ptr<Htmlnode> div1 = make_unique<Htmlnode>();
  div1->tag_name = L"div";

  unique_ptr<Htmlnode> div2 = make_unique<Htmlnode>();
  div2->tag_name = L"div";

  unique_ptr<Textnode> text1 = make_unique<Textnode>();
  text1->content = L"some content";

  unique_ptr<Textnode> text2 = make_unique<Textnode>();
  text2->content = L"more content";

        div2->children.push_back(move(text2));
      div1->children.push_back(move(text1));
      div1->children.push_back(move(div2));
    body->children.push_back(move(div1));
    head->children.push_back(move(meta));
  html->children.push_back(move(head));
  html->children.push_back(move(body));
  sample.nodes.push_back(move(html));

  Htmlstart result;
  BOOST_CHECK_NO_THROW(parser.parse(&result));
  BOOST_CHECK(sample == result);
}
