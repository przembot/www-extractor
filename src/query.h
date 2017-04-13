/**
 * Modul zawierajacy drzewo zapytania oraz tokeny dla zapytan
 * Przemyslaw Kopanski
 */
#ifndef _QUERY_H_
#define _QUERY_H_

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <ctype.h>
#include "source.h"

using namespace std;

struct qnode {
  map<string, string> read_attributes;
  vector<string> unknown_attributes; // wartosci atrybutow do wypisania
  string tagname;
  bool tagNameKnown; // czy nazwa tagu moze byc dowolna
  string content;
};

struct qstart {
  int questionType;
  list<qnode*> children;
};


enum SymType {
    slashtk // /
  , attrquery // ?
  , mustexist // !
  , anytag // 'all'
  , tagname // string
  , attrval // string
  , attrname // string
};


// Czy uzyc variant<char, string> zamiast string?
typedef pair<SymType, string> Symbol;

/* Tokeny
 TOK_SLASH - /
 TOK_ATTRQUERY - ?
 TOK_MUSTEXIST - !
 TOK_ANYTAG - *
 TOK_QUERYALL - all
 TOK_TAGNAME - string
 TOK_ATTRNAME - string
 TOK_ATTRVAL - string

// Tokeny wyrzucone
 TOK_QUOTE - '
 TOK_ATTRVAL - :
 */

// T - klasa obslugujaca wczytywanie kodu zrodlowego
// musi zawierac nextChar, error
// a jej konstruktor przyjmowac stringa
template<typename T>
class QueryLexer {
  public:
    QueryLexer(const string filename);
    ~QueryLexer() {};
    Symbol nextSymbol();
    void error();

  private:
    T sourceFile;
    char nextChar();
};

#include "query.cpp"

#endif
