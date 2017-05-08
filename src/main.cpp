#include <iostream>
#include "source.h"
#include "queryparser.h"

using namespace std;

const string& sampleQuery = "/* tag=!/";

int main() {
  QueryLexer<StringSource> lexer(sampleQuery);
  QueryParser<StringSource> parser(lexer);

  qstart qtree;
  try {
    parser.parse(&qtree);
  } catch (QueryParseException e) {
    cout << e.what() << endl;
  }

  return 0;
}

