#ifndef _QUERY_PARSER_H_
#define _QUERY_PARSER_H_

#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <set>

#include "querylexer.h"

using namespace std;

enum class QuestionType {
  CONTENT_ONLY,
  CHILDREN_ONLY,
  EVERYTHING
};

struct qnode {
  map<wstring, wstring> read_attributes;
  vector<wstring> unknown_attributes; // wartosci atrybutow do wypisania
  wstring tagname;
  bool tagNameKnown; // czy nazwa tagu jest znana (1), czy moze byc dowolna (0)
  const bool operator== (const qnode& rhs) const;
};

struct qstart {
  QuestionType questionType;
  vector<qnode> children;
  const bool operator== (const qstart& rhs) const;
};

wostream& operator<<(wostream& stream, const qstart& s);
wostream& operator<<(wostream& stream, const qnode& n);
wostream& operator<<(wostream& stream, const QuestionType& s);

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
    void acceptNext(const SymType& stype);
    void acceptNext(const SymSet& sset);
    void nextSymbol();
    void parseStart();
    void parseQuery();
    void parseExpressions();
    bool tryParseExpression();
    void parseAttributes();
    bool tryParseAttribute();

    QueryLexer &lexer;
    Symbol symbol; // aktualny atom
    qstart* result;
    qnode nodebuilder;
    vector<Symbol> buffor; // log ostatnich dzialan
};

#endif
