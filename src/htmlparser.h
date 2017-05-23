#ifndef _HTML_PARSER_H_
#define _HTML_PARSER_H_

#include <stdexcept>
#include <list>
#include <map>
#include <set>
#include "htmllexer.h"

class Visitor;
  
struct Node {
  virtual ~Node() {}
  virtual void accept(Visitor &v) const = 0;
};

string contentString(const Node*); // zwraca tylko content jako ciag znakow
string childrenString(const Node*); // zwraca tylko potomkow jako ciag znakow
string allString(const Node*); // zwraca potomkow oraz content jako ciag znakow

bool equalsNode(const Node* a, const Node* b);

struct Htmlstart {
  //~Htmlstart();

  list<Node*> nodes;
  const bool operator== (const Htmlstart& rhs) const;
};

struct Htmlnode : public Node {
  //~Htmlnode();

  string tag_name;
  map<string, string> attributes;
  list<Node*> children;
  const bool operator== (const Htmlnode& rhs) const;
  void accept(Visitor &v) const;
};

struct Emptyhtmlnode : public Node {
  //~Emptyhtmlnode();
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
    virtual const void visit(const Htmlnode *n) = 0;
    virtual const void visit(const Emptyhtmlnode *n) = 0;
    virtual const void visit(const Textnode *n) = 0;
};


class PrintVisitor : public Visitor {
  public:
    PrintVisitor(ostream& stream);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
  private:
    ostream& stream;
};


class ToStringVisitor : public Visitor {
  public:
    ToStringVisitor(bool content, bool children);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
    const string& getResult();

  private:
    string result;
    bool content;
    bool children;
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


class HtmlParser {
  public:
    typedef set<HtmlSymType> SymSet;
    HtmlParser(HtmlLexer &lexer);
    void parse(Htmlstart* tree); // odpala parsowanie,
                                 // moze rzucic wyjatek HtmlParseException


  private:
    // funkcje accept rzuacaja wyjatek HtmlParseException
    void accept(const HtmlSymType& stype);
    void accept(const SymSet& sset);
    void acceptNext(const HtmlSymType& stype);
    void acceptNext(const SymSet& sset);
    void nextMetaSymbol();
    void nextTextSymbol();
    void nextSymbolCheckText();
    void addParenthood(Node*);
    void createHtmlNode(const string&);
    void createEmptyNode(const string&);
    void createTextNode(const string&);
    void parseStart();
    void parseNode();
    void parseNodes();
    void parseAttributes();
    bool tryParseTextContent();
    bool tryParseComment();
    bool tryParseNode();

    HtmlLexer &lexer;
    HtmlSymbol symbol; // aktualny atom
    Htmlstart* result;
    map<string, string> buffAttrs; // atrybuty wczytane
    string lastAttrName;
    vector<Htmlnode*> tagStack; // stos rozpatrywanych tagow

};


#endif
