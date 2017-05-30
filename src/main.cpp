#include <iostream>
#include "extractor.h"

using namespace std;

int main() {
  Extractor e("query", "page.html");
  for (const auto &elem : e.getResult()) {
    wcout << elem << endl;
  }
  return 0;
}

