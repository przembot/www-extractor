#include "source.h"
#include "htmllexer.h"
#include <vector>

const wstring& sampleHtml1 = L"<html/ >some content=</html>";
const HtmlSymbol sampleHtmlTokens1[] = {
    {tagopentk, L""} , {htmlstringtk, L"html"}, {tagselfclosetk, L""}
  , {textstringtk, L"some content="}
  , {closingtagopentk, L""} , {htmlstringtk, L"html"}, {tagclosetk, L""}
};

const wstring& sampleHtml2 = L"<!-- some comment stuff > lolz -->";
const HtmlSymbol sampleHtmlTokens2[] = {
    {commenttk, L" some comment stuff > lolz "}
};


const wstring& sampleHtml3 = L"<body lolnoval color=red class=\"pinky one\">";
const HtmlSymbol sampleHtmlTokens3[] = {
    {tagopentk, L""} , {htmlstringtk, L"body"}
  , {htmlstringtk, L"lolnoval"}
  , {htmlstringtk, L"color"}, {equaltk, L""}, {htmlstringtk, L"red"}
  , {htmlstringtk, L"class"}, {equaltk, L""}, {doublequotetk, L"pinky one"}
  , {tagclosetk, L""}
};


const wstring& sampleHtml4 = L"<!DOCTYPE html>";
const HtmlSymbol sampleHtmlTokens4[] = {
    {doctypetk, L" html"}
};

const wstring& sampleHtml5 = L"<script> var i = 1; <lolz> </script>some content";
const HtmlSymbol sampleHtmlTokens5[] = {
    {textstringtk, L"some content"}
};


wostream& operator<<(wostream& stream, const HtmlSymbol& s) {
  stream << s.first << L" " << s.second;
  return stream;
}


BOOST_AUTO_TEST_CASE( html_lexer_1 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml1));

  vector<HtmlSymbol> log;
  HtmlSymbol tmp;

  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  q.tryNextTextSymbol(tmp);
  log.push_back(tmp);
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());
  log.push_back(q.nextMetaSymbol());

  for (int i = 0; i < 7; ++i)
    BOOST_CHECK(log[i] == sampleHtmlTokens1[i]);
}


BOOST_AUTO_TEST_CASE( html_lexer_2 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml2));
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_CHECK(log == sampleHtmlTokens2[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_3 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml3));

  HtmlSymbol log;

  for (int i = 0; i < 9; ++i) {
    log = q.nextMetaSymbol();
    BOOST_CHECK(log == sampleHtmlTokens3[i]);
  }
}

BOOST_AUTO_TEST_CASE( html_lexer_4 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml4));
  HtmlSymbol log;

  log = q.nextMetaSymbol();
  BOOST_CHECK(log == sampleHtmlTokens4[0]);
}

BOOST_AUTO_TEST_CASE( html_lexer_5 )
{
  HtmlLexer q(make_unique<StringSource>(sampleHtml5));
  HtmlSymbol log;

  q.nextMetaSymbol();
  log = q.nextMetaSymbol();
  BOOST_CHECK(log.second == L"script");
  q.skipTag(L"</script>");
  q.tryNextTextSymbol(log);
  BOOST_CHECK(log == sampleHtmlTokens5[0]);
}
