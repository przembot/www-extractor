template<typename T>
QueryLexer<T>::QueryLexer(const string filename) : sourceFile(filename) {

};


template<typename T>
Symbol QueryLexer<T>::nextSymbol() {
  Symbol result;
  char c;

  // Pomijanie bialych znakow
  do
    c = nextChar();
  while (isspace(c));



}


template<typename T>
char QueryLexer<T>::nextChar() {
  return sourceFile.nextChar();
}
