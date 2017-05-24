#include "htmlparser.h"

/*
Htmlstart::~Htmlstart() {
  for (auto &ptr : nodes)
    delete ptr;
}

Htmlnode::~Htmlnode() {
  for (auto &ptr : children)
    delete ptr;
}
*/


string contentString(const Node* node) {
  ToStringVisitor v(true, false);
  node->accept(v);
  return v.getResult();
}

string allString(const Node* node) {
  ToStringVisitor v(true, true);
  node->accept(v);
  return v.getResult();
}

string childrenString(const Node* node) {
  ToStringVisitor v(true, true);
  node->accept(v);
  return v.getResult();
}

const bool Htmlstart::operator== (const Htmlstart& rhs) const {
  if (nodes.size() == rhs.nodes.size()) {
    auto it1 = nodes.begin();
    auto it2 = rhs.nodes.begin();
    for (; it1 != nodes.end() && it2 != rhs.nodes.end(); ++it1, ++it2)
      if (!equalsNode(*it1, *it2))
        return false;
    return true;
  }
  return false;
}


const bool Htmlnode::operator== (const Htmlnode& rhs) const {
  if (tag_name == rhs.tag_name && attributes == rhs.attributes
      && children.size() == rhs.children.size()) {
    auto it1 = children.begin();
    auto it2 = rhs.children.begin();
    for (; it1 != children.end() && it2 != rhs.children.end(); ++it1, ++it2)
      if (!equalsNode(*it1, *it2))
        return false;
    return true;
  }
  return false;
}


const bool Emptyhtmlnode::operator== (const Emptyhtmlnode& rhs) const {
  return tag_name == rhs.tag_name && attributes == rhs.attributes;
}


const bool Textnode::operator== (const Textnode& rhs) const {
  return content == rhs.content;
}


void Htmlnode::accept(Visitor &v) const {
  v.visit(this);
}

void Textnode::accept(Visitor &v) const {
  v.visit(this);
}

void Emptyhtmlnode::accept(Visitor &v) const {
  v.visit(this);
}


bool equalsNode(const Node* a, const Node* b) {
  const Htmlnode *h1, *h2;
  const Emptyhtmlnode *e1, *e2;
  const Textnode *t1, *t2;
  if ((h1 = dynamic_cast<const Htmlnode*>(a)) && (h2 = dynamic_cast<const Htmlnode*>(b)))
    return *h1 == *h2;
  else if ((e1 = dynamic_cast<const Emptyhtmlnode*>(a)) && (e2 = dynamic_cast<const Emptyhtmlnode*>(b)))
    return *e1 == *e2;
  else if ((t1 = dynamic_cast<const Textnode*>(a)) && (t2 = dynamic_cast<const Textnode*>(b)))
    return *t1 == *t2;
  else
    return false;
}



void HtmlParser::accept(const HtmlSymType& stype) {
  if (symbol.first != stype) {
    //cout << "oczekiwano: " << stype << " a jest: " << symbol.first << endl;
    throw HtmlParseException("nieoczekiwany atom");
  }
}


void HtmlParser::accept(const SymSet& sset) {
  if (sset.find(symbol.first) == sset.end()) {
    throw HtmlParseException("nieoczekiwany atom");
  }
}


void HtmlParser::acceptNext(const HtmlSymType& stype) {
  nextMetaSymbol();
  if (symbol.first != stype) {
    //cout << "oczekiwano: " << stype << " a jest: " << symbol.first << endl;
    throw HtmlParseException("nieoczekiwany atom");
  }
}


void HtmlParser::acceptNext(const SymSet& sset) {
  nextMetaSymbol();
  if (sset.find(symbol.first) == sset.end()) {
    throw HtmlParseException("nieoczekiwany atom");
  }
}


void HtmlParser::nextMetaSymbol() {
  symbol = lexer.nextMetaSymbol();
  //cout << symbol.first << " " << symbol.second << endl;
}


void HtmlParser::nextTextSymbol() {
  symbol = lexer.nextTextSymbol();
  //cout << symbol.first << " " << symbol.second << endl;
}

HtmlParser::HtmlParser(HtmlLexer &inlexer)
  : lexer(inlexer) {
}


void HtmlParser::nextSymbolCheckText() {
  if (isalnum(lexer.currentChar()))
    nextTextSymbol();
  else
    nextMetaSymbol();
}

void HtmlParser::parseStart() {
  // doctype oraz odpalanie parsowanie nodow
  nextMetaSymbol();

  if (symbol.first == doctypetk)
    nextMetaSymbol();

  parseNodes();
}


void HtmlParser::parseNodes() {
  if (symbol.first == commenttk || symbol.first == tagopentk
      || symbol.first == textstringtk) {
    parseNode();
    parseNodes();
  }
}

bool HtmlParser::tryParseTextContent() {
  if (symbol.first == textstringtk) {
    createTextNode(symbol.second);
    nextSymbolCheckText();
    return true;
  }
  return false;
}

bool HtmlParser::tryParseComment() {
  if (symbol.first == commenttk) {
    nextSymbolCheckText();
    return true;
  }
  return false;
}

bool HtmlParser::tryParseNode() {
  if (symbol.first == tagopentk) {
    acceptNext(htmlstringtk); // nazwa noda
    string tagname = symbol.second;

    if (tagname == "script" || tagname == "style") {
      lexer.skipTag("</"+tagname+">");
      nextSymbolCheckText();
      return true;
    }

    nextMetaSymbol();
    parseAttributes();

    accept({tagselfclosetk, tagclosetk});
    if (symbol.first == tagselfclosetk) {
      createEmptyNode(tagname);
      nextSymbolCheckText();
    } else {
      // utworz wezel z atrybutami i odloz na stosie
      createHtmlNode(tagname);

      nextSymbolCheckText();
      parseNodes();

      accept(closingtagopentk);
      acceptNext(htmlstringtk);
      string closetagname = symbol.second;

      acceptNext(tagclosetk);

      if (tagname != closetagname)
        throw HtmlParseException(
            "wrong closing tag, "+tagname+" expected but "+symbol.second+" occured");

      // sprawdz czy jest jakis tekst
      nextSymbolCheckText();
      // wyrzuc tag ze stosu
      tagStack.pop_back();
    }
    return true;
  }
  return false;
}

void HtmlParser::parseNode() {
  // text/node/comment
  // symbol jest aktualnym symbolem do rozpatrzenia

  if (!tryParseTextContent())
    if (!tryParseComment())
      tryParseNode();

}


void HtmlParser::parseAttributes() {
  buffAttrs.clear();

  string attrname;
  // dopoki mozemy natrafic na atrybuty
  while (symbol.first != tagclosetk && symbol.first != tagselfclosetk) {
    accept(htmlstringtk);
    attrname = symbol.second;
    nextMetaSymbol();
    if (symbol.first == equaltk) {
      acceptNext({htmlstringtk, singlequotetk, doublequotetk});
      buffAttrs[attrname] = symbol.second;
      nextMetaSymbol();
    } else // no val attribute
      buffAttrs[attrname] = "";
  }

}


void HtmlParser::addParenthood(Node *child) {
  if (tagStack.size() >= 1) {
    Htmlnode* prelast = tagStack[tagStack.size()-1];
    prelast->children.push_back(child);
  } else // rodzicem jest htmlstart
    result->nodes.push_back(child);
}


void HtmlParser::createTextNode(const string &textcontent) {
  Textnode *node = new Textnode();
  node->content = textcontent;
  addParenthood(node);
}


void HtmlParser::createHtmlNode(const string &tagname) {
  Htmlnode *node = new Htmlnode();
  node->tag_name = tagname;
  node->attributes = buffAttrs;
  addParenthood(node);
  tagStack.push_back(node);
}


void HtmlParser::createEmptyNode(const string& tagname) {
  Emptyhtmlnode* node = new Emptyhtmlnode();
  node->tag_name = tagname;
  node->attributes = buffAttrs;
  addParenthood(node);
}


void HtmlParser::parse(Htmlstart* tree) {
  // inicjacja drzewa
  if (tree == nullptr)
    throw HtmlParseException("tree uninitialized");
  result = tree;
  tagStack.clear();
  buffAttrs.clear();

  parseStart();
}
