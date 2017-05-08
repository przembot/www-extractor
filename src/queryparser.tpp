template<typename T>
void QueryParser<T>::accept(const SymType& stype) {
  nextSymbol();
  if (symbol.first != stype) {
    //cout << "Stan: " << state << " oczekiwano: " << stype << endl;
    throw QueryParseException("nieoczekiwany atom");
  }
}


template<typename T>
void QueryParser<T>::accept(const SymSet& sset) {
  nextSymbol();
  if (sset.find(symbol.first) == sset.end()) {
    //cout << state << endl;
    throw QueryParseException("nieoczekiwany atom");
  }
}


template<typename T>
void QueryParser<T>::nextSymbol() {
  symbol = lexer.nextSymbol();
  //cout << symbol.first << " " << symbol.second << endl;
}


template<typename T>
QueryParser<T>::QueryParser(QueryLexer<T> &inlexer)
  : lexer(inlexer), state(0) {
}


template<typename T>
void QueryParser<T>::parse(qstart* tree) {
  // inicjacja drzewa
  if (tree == nullptr)
    throw QueryParseException("tree uninitialized");
  result = tree;
  buffor.clear();

  accept(slashtk);
  state = 1;
  parseInternal();
}

template<typename T>
void QueryParser<T>::parseInternal() {
  switch (state) {
    case 1:
      parseState1();
      break;
    case 2:
      parseState2();
      break;
    case 3:
      parseState3();
      break;
    case 4:
      parseState4();
      break;
    case 5:
      parseState5();
      break;
    default:
      cout << "Internal error, unknown parser state" << endl;
      break;
  }
  if (state != 9000)
    parseInternal();
}


template<typename T>
void QueryParser<T>::parseState1() {
  const SymSet followers = {
    // to state 3, push atom to buffer
    othertk,
    // to state 2, push atom to buffer
    slashtk,
    // to state 4, push atom to buffer
    anytagtk, stringtk
  };

  accept(followers);

  switch (symbol.first) {
    case othertk:
      state = 3;
      break;
    case slashtk:
      state = 2;
      buffor.push_back(symbol);
      break;
    case anytagtk: case stringtk:
      state = 4;
      buffor.push_back(symbol);
      break;
    default:
      cout << "internal error" << endl;
      break;
  }
}


template<typename T>
void QueryParser<T>::parseState2() {
  const SymSet followers = {
    // to state 3, push atom to buffer
    othertk,
    // to state 3, push atom to buffer
    slashtk,
  };

  accept(followers);

  switch (symbol.first) {
    case othertk:
      state = 3;
      break;
    case slashtk:
      state = 3;
      buffor.push_back(symbol);
      break;
    default:
      cout << "internal error" << endl;
      break;
  }
}


template<typename T>
void QueryParser<T>::parseState3() {
  // end of parsing
  state = 9000;

  // set proper questiontype
  if (buffor.size() < 4)
    result->questionType = buffor.size();
  else
    cout << "internal error" << endl;
}


template<typename T>
void QueryParser<T>::parseState4() {
  nodebuilder = new qnode();
  result->children.push_back(nodebuilder);
  if (symbol.first != anytagtk)
    nodebuilder->tagNameKnown = 1;
  else {
    nodebuilder->tagNameKnown = 0;
    nodebuilder->tagname = "";
  }

  const SymSet followers = {
    // to state 1, save and clear buffer
    slashtk,
    // to state 5, push atom to buffer
    stringtk
  };

  accept(followers);


  switch (symbol.first) {
    case slashtk:
      state = 1;
      // TODO: parse error - if last atom was anytagtk - invalid token?
      // TODO: test
      if (buffor.back().first == stringtk)
        nodebuilder->tagname = buffor.back().second;
      buffor.clear();
      break;
    case stringtk:
      state = 5;
      buffor.push_back(symbol);
      break;
    default:
      cout << "internal error" << endl;
      break;
  }
}


template<typename T>
void QueryParser<T>::parseState5() {
  // parsing the whole node..
  accept(equalstk);

  const SymSet valfollowers = {
    stringtk, singlequotevaltk, doublequotevaltk, mustexisttk, attrquerytk
  };

  accept(valfollowers);

  // fill the node with data
  // TODO: test
  if (symbol.first == mustexisttk)
    nodebuilder->read_attributes[buffor.back().second] = "!";
  else if (symbol.first == attrquerytk)
    nodebuilder->unknown_attributes.push_back(buffor.back().second);
  else
    nodebuilder->read_attributes[buffor.back().second] = symbol.second;

  buffor.clear();

  const SymSet outfollowers = {
    stringtk, slashtk
  };

  accept(outfollowers);

  switch (symbol.first) {
    case slashtk:
      state = 1;
      nodebuilder = nullptr;
      buffor.push_back(symbol);
      break;
    case stringtk:
      state = 5;
      buffor.push_back(symbol);
      break;
    default:
      cout << "internal error" << endl;
      break;
  }

}
