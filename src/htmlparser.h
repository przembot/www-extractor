#ifndef _HTML_PARSER_H_
#define _HTML_PARSER_H_

#include <stdexcept>
#include <list>
#include <map>
#include <set>
#include <unordered_set>

#include "htmllexer.h"
#include "htmlvisitor.h"

class Visitor;

struct Node {
  virtual ~Node() {}
  virtual void accept(Visitor &v) const = 0;
};

wstring contentString(const Node*); // zwraca tylko content jako ciag znakow
wstring childrenString(const Node*); // zwraca tylko potomkow jako ciag znakow
wstring allString(const Node*); // zwraca potomkow oraz content jako ciag znakow

bool equalsNode(const Node* a, const Node* b);

struct Htmlstart {
  //~Htmlstart();

  list<unique_ptr<Node>> nodes;
  const bool operator== (const Htmlstart& rhs) const;
};

struct Htmlnode : public Node {
  //~Htmlnode();

  wstring tag_name;
  map<wstring, wstring> attributes;
  list<unique_ptr<Node>> children;
  const bool operator== (const Htmlnode& rhs) const;
  void accept(Visitor &v) const;
};

struct Emptyhtmlnode : public Node {
  //~Emptyhtmlnode();
  wstring tag_name;
  map<wstring, wstring> attributes;
  const bool operator== (const Emptyhtmlnode& rhs) const;
  void accept(Visitor &v) const;
};

struct Textnode : public Node {
  wstring content;
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
    void nextSymbolCheckText();
    void addParenthood(unique_ptr<Node>);
    void createHtmlNode(const wstring&);
    void createEmptyNode(const wstring&);
    void createTextNode(const wstring&);
    void parseStart();
    void parseNode();
    void parseNodes();
    void parseAttributes();
    bool tryParseTextContent();
    bool tryParseComment();
    bool tryParseNode();
    bool tryParseTagClose(wstring&);
    bool isVoidTagName(const wstring&);

    HtmlLexer &lexer;
    HtmlSymbol symbol; // aktualny atom
    Htmlstart* result;
    map<wstring, wstring> buffAttrs; // atrybuty wczytane
    wstring lastAttrName;
    vector<Htmlnode*> tagStack; // stos rozpatrywanych tagow

};


#endif
