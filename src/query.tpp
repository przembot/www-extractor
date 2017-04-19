template<typename T>
QueryLexer<T>::QueryLexer(const string filename) : sourceFile(filename) {
  nextChar();
};


template<typename T>
Symbol QueryLexer<T>::nextSymbol() {
  Symbol result;

  // Pomijanie bialych znakow
  while (isspace(c))
    nextChar();

  if (wasError || c == EOF)
    result.first = othertk;
  if (c == '/')
    result.first = slashtk;
  else if (c == '*')
    result.first = anytagtk;
  else if (c == '!')
    result.first = mustexisttk;
  else if (c == '\'')
    result.first = quotetk;
  else if (c == ':')
    result.first = colontk;
  else if (c == '=')
    result.first = equalstk;
  else {
    while (isalpha(c)) {
      result.second.push_back(c);
      nextChar();
    }
    if (result.second.length() > 0)
      result.first = stringtk;
    else {
      string e = "oczekiwano stringa, a napotkano ";
      e.push_back(c);
      error(e); // nierozpoznany znak, oczekiwany string
    }

    return result;
  }

  nextChar();
  return result;
}

template<typename T>
void QueryLexer<T>::error(string e) {
  cout << "QueryLexer error" << endl << e << endl;
  c = EOF;
  wasError = true;
}

template<typename T>
void QueryLexer<T>::nextChar() {
  c = sourceFile.nextChar();
}
