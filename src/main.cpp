#include <iostream>
#include "extractor.h"

using namespace std;

const string query1 = "/div id='item'/";
const string html1 =
"<html>\
<body>\
<div id=\"item\">Item content</div>\
</body>\
</html>";


int main() {
  Extractor e(query1, html1);
  list<string> res = e.getResult();
  for (const string& e : res)
    cout << e << endl;

  return 0;
}

