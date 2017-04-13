#include <iostream>
#include "source.h"
#include "query.h"

using namespace std;

const string& sampleQuery = "/* tag=!/";

int main() {
  //QueryLexer<Source> q("text.quer");
  QueryLexer<StringSource> q(sampleQuery);
  for (int i = 0; i < 7; ++i) {
    Symbol a = q.nextSymbol();
    cout << a.first << " " << a.second << endl;
  }

  return 0;
}

