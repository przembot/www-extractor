#include <iostream>
#include "extractor.h"

using namespace std;

int main() {
  HtmlLexer lexer(make_unique<FileSource>("page.html"));
  HtmlParser parser(lexer);
  Htmlstart result;

  parser.parse(&result);
  return 0;
}

