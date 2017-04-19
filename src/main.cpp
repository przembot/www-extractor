#include <iostream>
#include "source.h"
#include "query.h"
#include "html.h"

using namespace std;

const string& sampleQuery = "/* tag=!/";
const string& sampleHtml = "\
  <html>\
  <head>\
  </head>\
  <body>\
  Sample Text\
  </body>\
  </html>\
  ";

int main() {
  /*
  //QueryLexer<Source> q("text.quer");
  QueryLexer<StringSource> q(sampleQuery);
  for (int i = 0; i < 7; ++i) {
    Symbol a = q.nextSymbol();
    cout << a.first << " " << a.second << endl;
  }
  */
  HtmlLexer<StringSource> q(sampleHtml);
  for (int i = 0; i < 22; ++i) {
    HtmlSymbol a = q.nextSymbol();
    cout << a.first << " " << a.second << endl;
  }

  return 0;
}

