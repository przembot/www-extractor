#ifndef _HTML_PARSER_H_
#define _HTML_PARSER_H_

#include <stdexcept>
#include <list>
#include <map>
#include <set>

#include "htmllexer.h"
#include "htmlvisitor.h"

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
