template<typename T>
void HtmlParser<T>::accept(const HtmlSymType& stype) {
  if (symbol.first != stype) {
    //cout << "oczekiwano: " << stype << " a jest: " << symbol.first << endl;
    throw HtmlParseException("nieoczekiwany atom");
  }
}


template<typename T>
void HtmlParser<T>::accept(const SymSet& sset) {
  if (sset.find(symbol.first) == sset.end()) {
    throw HtmlParseException("nieoczekiwany atom");
  }
}


template<typename T>
void HtmlParser<T>::acceptNext(const HtmlSymType& stype) {
  nextMetaSymbol();
  if (symbol.first != stype) {
    //cout << "oczekiwano: " << stype << " a jest: " << symbol.first << endl;
    throw HtmlParseException("nieoczekiwany atom");
  }
}


template<typename T>
void HtmlParser<T>::acceptNext(const SymSet& sset) {
  nextMetaSymbol();
  if (sset.find(symbol.first) == sset.end()) {
    throw HtmlParseException("nieoczekiwany atom");
  }
}


template<typename T>
void HtmlParser<T>::nextMetaSymbol() {
  symbol = lexer.nextMetaSymbol();
  //cout << symbol.first << " " << symbol.second << endl;
}


template<typename T>
void HtmlParser<T>::nextTextSymbol() {
  symbol = lexer.nextTextSymbol();
  //cout << symbol.first << " " << symbol.second << endl;
}

template<typename T>
HtmlParser<T>::HtmlParser(HtmlLexer<T> &inlexer)
  : lexer(inlexer) {
}


template<typename T>
void HtmlParser<T>::nextSymbolCheckText() {
  nextMetaSymbol();
  HtmlSymbol tmp = symbol;
  if (symbol.first == htmlstringtk) { // wylapano slowo jako metasymbol
    nextTextSymbol();
    symbol.second = tmp.second + symbol.second;
  } else if (symbol.first != tagopentk && symbol.first != closingtagopentk) {
    nextTextSymbol();
    if (symbol.second == "")
      symbol = tmp;
  }
  //cout << symbol.first << " " << symbol.second << endl;
}

template<typename T>
void HtmlParser<T>::parseStart() {
  // doctype oraz odpalanie parsowanie nodow
  nextMetaSymbol();

  if (symbol.first == doctypetk)
    nextMetaSymbol();

  parseNodes();
}


template<typename T>
void HtmlParser<T>::parseNodes() {
  if (symbol.first == commenttk || symbol.first == tagopentk
      || symbol.first == textstringtk) {
    parseNode();
    parseNodes();
  }
}


template<typename T>
void HtmlParser<T>::parseNode() {
  // text/node/comment
  // symbol jest aktualnym symbolem do rozpatrzenia

  string tagname;

  if (symbol.first == textstringtk) {
    // przetworz text node
    createTextNode(symbol.second);
    nextSymbolCheckText();
  } else if (symbol.first == commenttk) { // zignorowanie komentarza
      nextSymbolCheckText();
  } else if (symbol.first == tagopentk) { // tagopentk
    acceptNext(htmlstringtk); // nazwa noda
    tagname = symbol.second;

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
      if (tagname != symbol.second)
        throw HtmlParseException(
            "wrong closing tag, "+tagname+" expected but "+symbol.second+" occured");

      acceptNext(tagclosetk);

      // sprawdz czy jest jakis tekst
      nextSymbolCheckText();
      // wyrzuc tag ze stosu
      tagStack.pop_back();
    }
  }

}


template<typename T>
void HtmlParser<T>::parseAttributes() {
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


template<typename T>
void HtmlParser<T>::addParenthood(Node *child) {
  if (tagStack.size() >= 1) {
    Htmlnode* prelast = tagStack[tagStack.size()-1];
    prelast->children.push_back(child);
  } else // rodzicem jest htmlstart
    result->nodes.push_back(child);
}


template<typename T>
void HtmlParser<T>::createTextNode(const string &textcontent) {
  Textnode *node = new Textnode();
  node->content = textcontent;
  addParenthood(node);
}


template<typename T>
void HtmlParser<T>::createHtmlNode(const string &tagname) {
  Htmlnode *node = new Htmlnode();
  node->tag_name = tagname;
  node->attributes = buffAttrs;
  addParenthood(node);
  tagStack.push_back(node);
}


template<typename T>
void HtmlParser<T>::createEmptyNode(const string& tagname) {
  Emptyhtmlnode* node = new Emptyhtmlnode();
  node->tag_name = tagname;
  node->attributes = buffAttrs;
  addParenthood(node);
}


template<typename T>
void HtmlParser<T>::parse(Htmlstart* tree) {
  // inicjacja drzewa
  if (tree == nullptr)
    throw HtmlParseException("tree uninitialized");
  result = tree;
  tagStack.clear();
  buffAttrs.clear();

  parseStart();
}
