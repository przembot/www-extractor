#include <iostream>
#include "source.h"
#include "query.h"

using namespace std;

const string& sampleQuery = "/* tag=!/";

int main() {
  //QueryLexer<Source> q("text.quer");
  QueryLexer<StringSource> q(sampleQuery);

  return 0;
}

