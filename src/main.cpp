#include <iostream>
#include <memory>
#include "source.h"
#include "querylexer.h"
#include "queryparser.h"

using namespace std;

const string& sampleQuery = "/* tag=!/";

int main() {
  unique_ptr<Source> source = make_unique<StringSource>(sampleQuery);
  QueryLexer lexer(source);
  QueryParser parser(lexer);

  qstart qtree;
  try {
    parser.parse(&qtree);
  } catch (QueryParseException e) {
    cout << e.what() << endl;
  }

  return 0;
}

