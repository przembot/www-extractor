#include "source.h"
#include "htmllexer.h"
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
  , {htmlstringtk, "lolnoval"}
  , {htmlstringtk, "color"}, {equaltk, ""}, {htmlstringtk, "red"}
  , {htmlstringtk, "class"}, {equaltk, ""}, {doublequotetk, "pinky one"}
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
  HtmlLexer q(make_unique<StringSource>(sampleHtml1));

  vector<HtmlSymbol> log;

  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextTextSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());

  for (int i = 0; i < 7; ++i)
    BOOST_TEST(log[i] == sampleHtmlTokens1[i]);
}


BOOST_AUTO_TEST_CASE( html_lexer_2 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml2));
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_TEST(log == sampleHtmlTokens2[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_3 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml3));

  HtmlSymbol log;

  for (int i = 0; i < 9; ++i) {
    log = q.nextMetaSymbol();
    BOOST_TEST(log == sampleHtmlTokens3[i]);
  }
}

BOOST_AUTO_TEST_CASE( html_lexer_4 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml4));
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_TEST(log == sampleHtmlTokens4[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_5 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml5));
  HtmlSymbol log;

  q.nextMetaSymbol();
  log = q.nextMetaSymbol();
  BOOST_TEST(log.second == "script");
  q.skipTag("</script>");
  log = q.nextTextSymbol();
  BOOST_TEST(log == sampleHtmlTokens5[0]);
}
