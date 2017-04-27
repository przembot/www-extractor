#include "source.h"
#include "html.h"
#include <vector>

const string& sampleHtml1 = "<html/ >some content=</html>";
const HtmlSymbol sampleHtmlTokens1[] = {
    {tagopentk, ""} , {htmlstringtk, "html"}, {tagselfclosetk, ""}
  , {textstringtk, "some content="}
  , {closingtagopentk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
};

const string& sampleHtml2 = "<!-- some comment stuff > lolz -->";
const HtmlSymbol sampleHtmlTokens2[] = {
    {commenttk, " some comment stuff > lolz "}
};


const string& sampleHtml3 = "<body lolnoval color=red class=\"pinky one\">";
const HtmlSymbol sampleHtmlTokens3[] = {
    {tagopentk, ""} , {htmlstringtk, "body"}
  , {htmlstringtk, "lolnoval"}, {novaltk, ""}
  , {htmlstringtk, "color"}, {noquoteval, "red"}
  , {htmlstringtk, "class"}, {doublequotetk, "pinky one"}
  , {tagclosetk, ""}
};


const string& sampleHtml4 = "<!DOCTYPE html>";
const HtmlSymbol sampleHtmlTokens4[] = {
    {doctypetk, " html"}
};

const string& sampleHtml5 = "<script> var i = 1; <lolz> </script>some content";
const HtmlSymbol sampleHtmlTokens5[] = {
    {textstringtk, "some content"}
};


ostream& operator<<(ostream& stream, const HtmlSymbol& s) {
  stream << s.first << " " << s.second;
  return stream;
}


BOOST_AUTO_TEST_CASE( html_lexer_1 )
{
  HtmlLexer<StringSource> q(sampleHtml1);

  vector<HtmlSymbol> log;

  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextCloseSymbol());
  log.push_back(q.nextTextSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextCloseSymbol());

  for (int i = 0; i < 7; ++i)
    BOOST_TEST(log[i] == sampleHtmlTokens1[i]);
}


BOOST_AUTO_TEST_CASE( html_lexer_2 )
{
  HtmlLexer<StringSource> q(sampleHtml2);
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_TEST(log == sampleHtmlTokens2[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_3 )
{
  HtmlLexer<StringSource> q(sampleHtml3);

  vector<HtmlSymbol> log;
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextValSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextValSymbol());
  log.push_back(q.nextWordSymbol());
  log.push_back(q.nextValSymbol());
  log.push_back(q.nextCloseSymbol());

  for (int i = 0; i < 9; ++i)
    BOOST_TEST(log[i] == sampleHtmlTokens3[i]);
}

BOOST_AUTO_TEST_CASE( html_lexer_4 )
{
  HtmlLexer<StringSource> q(sampleHtml4);
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_TEST(log == sampleHtmlTokens4[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_5 )
{
  HtmlLexer<StringSource> q(sampleHtml5);
  HtmlSymbol log;

  q.nextMetaSymbol();
  log = q.nextWordSymbol();
  BOOST_TEST(log.second == "script");
  q.skipTag("</script>");
  log = q.nextTextSymbol();
  BOOST_TEST(log == sampleHtmlTokens5[0]);
}
