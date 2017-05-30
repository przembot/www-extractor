#include "extractor.h"
#include <list>

const wstring test1[] = {L"/div id='item'/",
L"<html>\
<body>\
<div id=\"item\">Item content</div>\
</body>\
</html>"};

// sample page from docs
const wstring docsample =
L"<html>\
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


const wstring test21[] = {L"/div class='item'/a class='price'/", docsample};
const wstring test22[] = {L"/div class='item'/* class='price'/", docsample};
const wstring test23[] = {L"/div/* class='price'/", docsample};

const wstring test3[] = {L"/img src=?/", docsample};
const wstring test4[] = {L"/* lolcontent=!/img src=?/", docsample};


BOOST_AUTO_TEST_CASE(extractor_1) {
  Extractor e(test1[0], test1[1]);
  list<wstring> res;
  list<wstring> expected = {L"Item content"};
  BOOST_CHECK_NO_THROW(res = e.getResult());
  BOOST_CHECK(res == expected);
}

BOOST_AUTO_TEST_CASE(extractor_2) {
  Extractor e1(test21[0], test21[1]);
  Extractor e2(test22[0], test22[1]);
  Extractor e3(test23[0], test23[1]);

  list<wstring> res;
  list<wstring> expected = {L"54", L"12", L"34"};

  BOOST_CHECK_NO_THROW(res = e1.getResult());
  BOOST_CHECK(res == expected);

  BOOST_CHECK_NO_THROW(res = e2.getResult());
  BOOST_CHECK(res == expected);

  BOOST_CHECK_NO_THROW(res = e3.getResult());
  BOOST_CHECK(res == expected);
}

BOOST_AUTO_TEST_CASE(extractor_3) {
  list<wstring> res;
  list<wstring> expected = {L"kot.jpg", L"pies.jpg", L"kurczak.jpg", L"ppap.jpg"};

  BOOST_CHECK_NO_THROW(res = findInfo(test3[1], test3[0]));
  BOOST_CHECK(res == expected);
}

BOOST_AUTO_TEST_CASE(extractor_4) {
  list<wstring> res;
  list<wstring> expected = {L"ppap.jpg"};

  BOOST_CHECK_NO_THROW(res = findInfo(test4[1], test4[0]));
  BOOST_CHECK(res == expected);
}
