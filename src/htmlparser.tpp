template<typename T>
void HtmlParser<T>::accept(const HtmlSymType& stype) {
  nextMetaSymbol();
  if (symbol.first != stype) {
    //cout << "Stan: " << state << " oczekiwano: " << stype << " a jest: " << symbol.first << endl;
    throw HtmlParseException("nieoczekiwany atom");
  }
}


template<typename T>
void HtmlParser<T>::accept(const SymSet& sset) {
  nextMetaSymbol();
  if (sset.find(symbol.first) == sset.end()) {
    //cout << "Stan: " << state << endl;
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
  : lexer(inlexer), state(0) {
}


template<typename T>
void HtmlParser<T>::addParenthood() {
  if (tagStack.size() >= 2) {
    pair<string, Node*> &prelast = tagStack[tagStack.size()-2];
    if (Htmlnode *n = dynamic_cast<Htmlnode*>(prelast.second))
      n->children.push_back(tagStack.back().second);
  } else { // rodzicem jest htmlstart
    result->nodes.push_back(tagStack.back().second);
  }
}


template<typename T>
void HtmlParser<T>::createHtmlNode() {
    Htmlnode *tag = new Htmlnode();
    tagStack.back().second = tag;
    tag->tag_name = tagStack.back().first;
    tag->attributes = buffAttrs;
    addParenthood();
    buffAttrs.clear();
}


template<typename T>
void HtmlParser<T>::createEmptyNode() {
    Emptyhtmlnode* tag = new Emptyhtmlnode();
    tagStack.back().second = tag;
    tag->tag_name = tagStack.back().first;
    tag->attributes = buffAttrs;
    addParenthood();
    buffAttrs.clear();
    tagStack.pop_back();
}


template<typename T>
void HtmlParser<T>::parse(Htmlstart* tree) {
  // inicjacja drzewa
  if (tree == nullptr)
    throw HtmlParseException("tree uninitialized");
  result = tree;
  tagStack.clear();
  buffAttrs.clear();

  state = 0;
  parseInternal();
}


template<typename T>
void HtmlParser<T>::parseInternal() {
  switch (state) {
    case 0:
      parseStartState();
      break;
    case 1:
      parseState1();
      break;
    case 2:
      parseState2();
      break;
    case 4:
      parseState4();
      break;
    case 5:
      parseState5();
      break;
    case 6:
      parseState6();
      break;
    default:
      throw HtmlParseException("Internal error, unknown parser state");
      break;
  }
  if (state != 9000)
    parseInternal();
}


template<typename T>
void HtmlParser<T>::parseStartState() {
  // stan 0, moze byc doctype

  const SymSet followers = {
    doctypetk, closingtagopentk, tagopentk, commenttk
  };

  accept(followers);

  switch (symbol.first) {
    case doctypetk: case commenttk:
      state = 1;
      break;
    case closingtagopentk:
      state = 2;
      break;
    case tagopentk:
      state = 4;
      break;
    default:
      throw HtmlParseException("internal error");
      break;
  }

}


template<typename T>
void HtmlParser<T>::parseState1() {

  nextTextSymbol();
  if (symbol.second != "") {
    Htmlnode *node;
    if (!tagStack.empty() && (node = dynamic_cast<Htmlnode*>(tagStack.back().second))) {
      Textnode *text = new Textnode();
      text->content = symbol.second;
      node->children.push_back(text);
    } else
      throw HtmlParseException("text in invalid place");
  }

  const SymSet followers = {
    closingtagopentk, tagopentk, commenttk, unknowntk
  };

  accept(followers);

  switch (symbol.first) {
    case commenttk:
      //state = 1;
      break;
    case closingtagopentk:
      state = 2;
      break;
    case tagopentk:
      state = 4;
      break;
    case unknowntk:
      // end of parsing
      state = 9000;
      break;
    default:
      throw HtmlParseException("internal error");
      break;
  }
}


template<typename T>
void HtmlParser<T>::parseState2() {
  // tag zamykajacy do obsluzenia
  accept(htmlstringtk);

  if (symbol.second != tagStack.back().first)
    throw HtmlParseException("wrong tag close, expecting"+tagStack.back().first);

  tagStack.pop_back();

  accept(tagclosetk);

  state = 1;

}


template<typename T>
void HtmlParser<T>::parseState4() {
  accept(htmlstringtk);
  // tworzy sie nowy tag, nie wiadomo jeszcze czy to zwykly czy selfclosing
  tagStack.push_back({symbol.second, nullptr});

  state = 5;
}


template<typename T>
void HtmlParser<T>::parseState5() {
  const SymSet followers = {
    tagclosetk, tagselfclosetk,
    htmlstringtk
  };

  accept(followers);

  switch (symbol.first) {
    case htmlstringtk:
      state = 6;
      lastAttrName = symbol.second;
      break;
    case tagclosetk:
      createHtmlNode();
      state = 1;
      break;
    case tagselfclosetk:
      createEmptyNode();
      state = 1;
      break;
    default:
      throw HtmlParseException("internal error");
      break;
  }
}


template<typename T>
void HtmlParser<T>::parseState6() {
  const SymSet followers = {
    tagclosetk, tagselfclosetk,
    equaltk,
    htmlstringtk
  };

  accept(followers);

  switch (symbol.first) {
    case tagclosetk:
      buffAttrs[lastAttrName] = ""; // pusty tag htmlowy
      createHtmlNode();
      state = 1;
      break;
    case tagselfclosetk:
      buffAttrs[lastAttrName] = ""; // pusty tag htmlowy
      createEmptyNode();
      state = 1;
      break;
    case equaltk: {
      // pobierz wartosc taga i go wstaw
      const SymSet valfollowers = {
        singlequotetk, doublequotetk, htmlstringtk
      };
      accept(valfollowers);
      buffAttrs[lastAttrName] = symbol.second;
      state = 5;
      break;
    }
    case htmlstringtk:
      buffAttrs[lastAttrName] = ""; // pusty tag htmlowy
      // nowy tag
      lastAttrName = symbol.second;
      state = 6;
      break;
    default:
      throw HtmlParseException("internal error");
      break;
  }
}
