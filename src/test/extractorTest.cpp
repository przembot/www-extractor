#include "extractor.h"
#include <list>

const string test1[] = {"/div id='item'/",
"<html>\
<body>\
<div id=\"item\">Item content</div>\
</body>\
</html>"};

// sample page from docs
const string docsample =
"<html>\
<body>\
<div id=\"main\">\
  <div id=\"items\">\
    <div class=\"item\">\
      <a class=\"price\">54</a>zl\
    </div>\
    <div class=\"item\">\
      <a class=\"price\">12</a>zl\
    </div>\
    <div class=\"item\">\
      <a class=\"price\">34</a>zl\
    </div>\
  </div>\
  <div id=\"images\">\
    <img src=\"kot.jpg\"/>\
    <img src=\"pies.jpg\"/>\
    <img src=\"kurczak.jpg\"/>\
  </div>\
  <div lolcontent='true'>\
    <img src=\"ppap.jpg\"/>\
  </div>\
</div>\
</body>\
</html>";


const string test21[] = {"/div class='item'/a class='price'/", docsample};
const string test22[] = {"/div class='item'/* class='price'/", docsample};
const string test23[] = {"/div/* class='price'/", docsample};


BOOST_AUTO_TEST_CASE(extractor_1) {
  Extractor e(test1[0], test1[1]);
  list<string> res;
  list<string> expected = {"Item content"};
  BOOST_CHECK_NO_THROW(res = e.getResult());
  BOOST_TEST(res == expected);
}

BOOST_AUTO_TEST_CASE(extractor_2) {
  Extractor e1(test21[0], test21[1]);
  Extractor e2(test22[0], test22[1]);
  Extractor e3(test23[0], test23[1]);

  list<string> res;
  list<string> expected = {"54", "12", "34"};

  BOOST_CHECK_NO_THROW(res = e1.getResult());
  BOOST_CHECK(res == expected);

  BOOST_CHECK_NO_THROW(res = e2.getResult());
  BOOST_TEST(res == expected);

  BOOST_CHECK_NO_THROW(res = e3.getResult());
  BOOST_TEST(res == expected);
}
