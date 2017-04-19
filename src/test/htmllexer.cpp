#include "source.h"
#include "html.h"
#include <vector>

const string& sampleHtml1 = "\
<html>\
<head>\
</head>\
<body>\
Sample Text\
</body>\
</html>\
";
const HtmlSymbol sampleHtmlTokens1[] = {
    {tagopentk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "head"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "head"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}
  , {textstringtk, "Sample Text"}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {unknowntk, ""}
};

const string& sampleHtml2 = "\
<html>\
<body>\
<div id=\"main\">\
  <div id=\"items\">\
    <div class=\"item\">\
      <a class=\"price\">54</a>zł\
    </div>\
  </div>\
  <div id=\"images\">\
    <img src=\"kot.jpg\"/>\
    <img src=\"pies.jpg\"/>\
  </div>\
  <!-- commenting -- stuff -->\
</body>\
</html>\
";
const HtmlSymbol sampleHtmlTokens2[] = {
    {tagopentk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "div"}, {htmlstringtk, "id"}, {equaltk, ""}
  , {doublequotetk, ""}, {htmlstringtk, "main"}, {doublequotetk, ""}, {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "div"}, {htmlstringtk, "id"}, {equaltk, ""}
  , {doublequotetk, ""}, {htmlstringtk, "items"}, {doublequotetk, ""},  {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "div"}, {htmlstringtk, "class"}, {equaltk, ""}
  , {doublequotetk, ""}, {htmlstringtk, "item"}, {doublequotetk, ""},  {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "a"}, {htmlstringtk, "class"}, {equaltk, ""}
  , {doublequotetk, ""}, {htmlstringtk, "price"}, {doublequotetk, ""},  {tagclosetk, ""}
  , {textstringtk, "54"} 
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "a"}, {tagclosetk, ""}
  , {textstringtk, "zł    "}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "div"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "div"}, {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "div"}, {htmlstringtk, "id"}, {equaltk, ""}
  , {doublequotetk, ""}, {htmlstringtk, "images"}, {doublequotetk, ""},  {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "img"}, {htmlstringtk, "src"}, {equaltk, ""}
  , {doublequotetk, ""} , {htmlstringtk, "kot.jpg"}, {doublequotetk, ""} 
  , {htmlslashtk, ""}, {tagclosetk, ""}

  , {tagopentk, ""} , {htmlstringtk, "img"}, {htmlstringtk, "src"}, {equaltk, ""}
  , {doublequotetk, ""} , {htmlstringtk, "pies.jpg"}, {doublequotetk, ""} 
  , {htmlslashtk, ""}, {tagclosetk, ""}

  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "div"}, {tagclosetk, ""}

  , {tagopentk, ""} , {tagstartcommenttk, ""}, {htmlstringtk, "commenting -- stuff "}, {tagendcommenttk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {unknowntk, ""}
};


const string& sampleHtml3 = "\
<html>\
<script>\
var i = 2 < 1;\
</script>\
<body>\
</body>\
<style type=\"asd\">\
.ul {\
  color: red;\
};\
</style>\
</html>\
";

const HtmlSymbol sampleHtmlTokens3[] = {
    {tagopentk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "script"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "script"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "body"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlstringtk, "style"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "style"}, {tagclosetk, ""}
  , {tagopentk, ""} , {htmlslashtk, ""} , {htmlstringtk, "html"}, {tagclosetk, ""}
  , {unknowntk, ""}
};

ostream& operator<<(ostream& stream, const HtmlSymbol& s) {
  stream << s.first << " " << s.second;
  return stream;
}

BOOST_AUTO_TEST_CASE( html_lexer_1 )
{
  HtmlLexer<StringSource> q(sampleHtml1);

  HtmlSymbol a, endsymbol;
  for (int i = 0; i < 23; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleHtmlTokens1[i]);
  }
  /*
    if (a != sampleHtmlTokens1[i]) {
      cout << "Has: " << a << endl;
      cout << "Expected: " << sampleHtmlTokens1[i] << endl;
    } else
      cout << "Good: " << a << endl;
  */
}


BOOST_AUTO_TEST_CASE( html_lexer_2 )
{
  HtmlLexer<StringSource> q(sampleHtml2);

  HtmlSymbol a, endsymbol;
  for (int i = 0; i < 95; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleHtmlTokens2[i]);
    /*
    if (a != sampleHtmlTokens2[i]) {
      cout << "Has: " << a << endl;
      cout << "Expected: " << sampleHtmlTokens2[i] << endl;
    } else
      cout << "Good: " << a << endl;
    */
  }
}


BOOST_AUTO_TEST_CASE( html_lexer_3 )
{
  HtmlLexer<StringSource> q(sampleHtml3);

  HtmlSymbol a, endsymbol;
  for (int i = 0; i < 22; ++i) {
    a = q.nextSymbol();
    BOOST_TEST(a == sampleHtmlTokens3[i]);
    /*
    if (a != sampleHtmlTokens3[i]) {
      cout << "Has: " << a << endl;
      cout << "Expected: " << sampleHtmlTokens3[i] << endl;
    } else
      cout << "Good: " << a << endl;
    */
  }
}
