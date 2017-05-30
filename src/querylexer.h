/**
 * Modul zawierajacy drzewo zapytania oraz tokeny dla zapytan
 * Przemyslaw Kopanski
 */
#ifndef _QUERY_LEXER_H_
#define _QUERY_LEXER_H_

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <ctype.h>

#include "source.h"

using namespace std;


enum SymType {
    slashtk // /
  , attrquerytk // ?
  , mustexisttk // !
  , anytagtk // *
  , stringtk // string
  , singlequotevaltk // "wartosc"
  , doublequotevaltk // "wartosc"
  , equalstk // =
  , othertk // koniec pliku
};


// Czy uzyc variant<char, string> zamiast string?
typedef pair<SymType, wstring> Symbol;


class QueryLexer {
  public:
    QueryLexer(unique_ptr<Source> &sourceFile);
    QueryLexer(unique_ptr<Source> &&sourceFile);
    ~QueryLexer() {}
    Symbol nextSymbol();
    void error(wstring e);
    QueryLexer(const QueryLexer&) = delete;
    QueryLexer& operator=(const QueryLexer&) = delete;

  private:
    unique_ptr<Source> sourceFile;
    void nextChar();
    wchar_t c; // pierwszy nieprzetworzony znak
    bool wasError;
};


#endif
