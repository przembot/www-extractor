#ifndef _QUERY_PARSER_H_
#define _QUERY_PARSER_H_

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <set>
#include "querylexer.h"

using namespace std;

struct qnode {
  map<string, string> read_attributes;
  vector<string> unknown_attributes; // wartosci atrybutow do wypisania
  string tagname;
  bool tagNameKnown; // czy nazwa tagu jest znana (1), czy moze byc dowolna (0)
  const bool operator== (const qnode& rhs) const;
};

struct qstart {
  int questionType;
  list<qnode*> children;
  const bool operator== (const qstart& rhs) const;
};

ostream& operator<<(ostream& stream, const qstart& s);
ostream& operator<<(ostream& stream, const qnode& n);

class QueryParseException : exception {
  public:
    QueryParseException(string a) : exception(), err(a) {};
    const char* what() const throw() {
      return err.c_str();
    }

  private:
  string err;
};


class QueryParser {
  public:
    typedef set<SymType> SymSet;
    QueryParser(QueryLexer &lexer);
    void parse(qstart* tree); // odpala parsowanie,
                               // moze rzucic wyjatek QueryParseException


  private:
    // funkcje accept rzuacaja wyjatek QueryParseException
    void accept(const SymType& stype);
    void accept(const SymSet& sset);
    void nextSymbol();
    void parseInternal(); // rozpoczyna parsowanie
    void parseState1();
    void parseState2();
    void parseState3();
    void parseState4();
    void parseState5();

    QueryLexer &lexer;
    Symbol symbol; // aktualny atom
    qstart* result;
    qnode* nodebuilder;
    vector<Symbol> buffor; // log ostatnich dzialan
    int state;

};

#endif
