#ifndef _HTML_PARSER_H_
#define _HTML_PARSER_H_

#include <stdexcept>
#include <list>
#include <map>
#include <set>
#include "htmllexer.h"

class Visitor;

struct Node {
  /*
  virtual string contentString() = 0; // zwraca tylko content jako ciag znakow
  virtual string childrenString() = 0; // zwraca tylko potomkow jako ciag znakow
  virtual string allString() = 0; // zwraca potomkow oraz content jako ciag znakow
  */
  virtual ~Node() {}
  virtual void accept(Visitor &v) const = 0;
};

bool equalsNode(const Node* a, const Node* b);

struct Htmlstart {
  list<Node*> nodes;
  const bool operator== (const Htmlstart& rhs) const;
};

struct Htmlnode : public Node {
  Htmlnode() {};
  string tag_name;
  map<string, string> attributes;
  list<Node*> children;
  const bool operator== (const Htmlnode& rhs) const;
  void accept(Visitor &v) const;
};

struct Emptyhtmlnode : public Node {
  string tag_name;
  map<string, string> attributes;
  const bool operator== (const Emptyhtmlnode& rhs) const;
  void accept(Visitor &v) const;
};

struct Textnode : public Node {
  string content;
  const bool operator== (const Textnode& rhs) const;
  void accept(Visitor &v) const;
};


class Visitor {
  public:
    virtual const void visit(const Htmlnode *n) const = 0;
    virtual const void visit(const Emptyhtmlnode *n) const = 0;
    virtual const void visit(const Textnode *n) const = 0;
};


class PrintVisitor : public Visitor {
  public:
    PrintVisitor(ostream& stream);
    const void visit(const Htmlnode *n) const;
    const void visit(const Emptyhtmlnode *n) const;
    const void visit(const Textnode *n) const;
  private:
    ostream& stream;
};


ostream& operator<<(ostream& stream, const Htmlstart& s);
ostream& operator<<(ostream& stream, const Node* s);
ostream& operator<<(ostream& stream, const Textnode& s);
ostream& operator<<(ostream& stream, const Htmlnode& s);
ostream& operator<<(ostream& stream, const Emptyhtmlnode& s);


class HtmlParseException : exception {
  public:
    HtmlParseException(string a) : exception(), err(a) {};
    const char* what() const throw() {
      return err.c_str();
    }

  private:
  string err;
};


template<typename T>
class HtmlParser {
  public:
    typedef set<HtmlSymType> SymSet;
    HtmlParser<T>(HtmlLexer<T> &lexer);
    void parse(Htmlstart* tree); // odpala parsowanie,
                                 // moze rzucic wyjatek HtmlParseException


  private:
    // funkcje accept rzuacaja wyjatek HtmlParseException
    void accept(const HtmlSymType& stype);
    void accept(const SymSet& sset);
    void nextMetaSymbol();
    void nextTextSymbol();
    void parseInternal();
    void parseStartState();
    void parseState1();
    void parseState2();
    void parseState4();
    void parseState5();
    void parseState6();
    void addParenthood();
    void createHtmlNode();
    void createEmptyNode();

    HtmlLexer<T> &lexer;
    HtmlSymbol symbol; // aktualny atom
    Htmlstart* result;
    map<string, string> buffAttrs; // atrybuty wczytane
    string lastAttrName;
    //vector<string> buffor; // log ostatnich atomow
    vector<pair<string, Node*>> tagStack; // stos niedomknietych tagow
    int state;

};

#include "htmlparser.tpp"

#endif