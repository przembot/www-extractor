#include "querylexer.h"


QueryLexer::QueryLexer(unique_ptr<Source> &source)
  : sourceFile(move(source)), wasError(0) {
  nextChar();
};


QueryLexer::QueryLexer(unique_ptr<Source> &&source)
  : sourceFile(move(source)), wasError(0) {
  nextChar();
};

Symbol QueryLexer::nextSymbol() {
  Symbol result;

  // Pomijanie bialych znakow
  while (isspace(c))
    nextChar();


  if (wasError || c == EOF || c == 0)
    result.first = othertk;
  else if (c == '/')
    result.first = slashtk;
  else if (c == '*')
    result.first = anytagtk;
  else if (c == '!')
    result.first = mustexisttk;
  else if (c == '?')
    result.first = attrquerytk;
  else if (c == '\'' || c == '"') {
    char quote = c;
    result.first = c == '"'?doublequotevaltk:singlequotevaltk;
    nextChar();
    while (isalpha(c)) {
      result.second.push_back(c);
      nextChar();
    }
    if (c != quote)
      error(L"zly znak zamkniecia nawiasu");
  } else if (c == '=')
    result.first = equalstk;
  else {
    while (isalpha(c)) {
      result.second.push_back(c);
      nextChar();
    }
    if (result.second.length() > 0)
      result.first = stringtk;
    else {
      wstring e = L"oczekiwano stringa, a napotkano ";
      e.push_back(c);
      error(e); // nierozpoznany znak, oczekiwany string
    }

    return result;
  }


  nextChar();
  return result;
}


void QueryLexer::error(wstring e) {
  cout << "QueryLexer error" << endl;
  wcerr << e << endl;
  c = EOF;
  wasError = true;
}

void QueryLexer::nextChar() {
  c = sourceFile->nextChar();
}
