template<typename T>
HtmlLexer<T>::HtmlLexer(const string filename) : sourceFile(filename), state(0), wasError(false) {
  nextChar();
};

/*
enum HtmlSymType {
    htmlslashtk // /
  , tagopentk // <
  , tagclosetk // >
  , unknowntk // koniec pliku
  , tagstartcommenttk // !--
  , tagendcommenttk // -->
  , doctypebegintk // !DOCTYPE
  , htmlstringtk // tagname, attrname, attrval
  , texthtmlstringtk // string wraz z & # ;
  , doctypehtmlstringtk // string wraz z / - "
  , singlequotetk // '
  , doublequotetk // "
  , equaltk // =
};
*/

template<typename T>
HtmlSymbol HtmlLexer<T>::nextSymbol() {
  HtmlSymbol result;

  // Pomijanie bialych znakow
  while (isspace(c))
    nextChar();

  if (c == EOF || wasError)
    result.first = unknowntk;
  else if (state == 0) {
    // oczekuje na < lub inny dowolny znak
    while (c != '<' && c != EOF) {
      result.second.push_back(c);
      nextChar();
    }

    if (result.second.size() > 0) // przetworzono stringa
      result.first = textstringtk;
    else {
      result.first = tagopentk;
      state = 1;
      nextChar();
    }
  } else if (state == 1) {
    // oczekuje na !DOCTYPE, !--, /, lub nazwe taga
    if (c == '/') {
      result.first = htmlslashtk;
      state = 5;
      nextChar();
    } else if (c == '!') {
      // moze pojawic sie !-- albo !DOCTYPE
      result.second = "!";
      nextChar(); result.second.push_back(c);
      nextChar(); result.second.push_back(c);
      if (result.second == "!--") {
        result.first = tagstartcommenttk;
        result.second = "";
        state = 3;
        nextChar();
      } else {
        for (int i = 0; i < 5; ++i) {
          nextChar(); result.second.push_back(c);
        }
        if (result.second == "!DOCTYPE") {
          result.first = doctypebegintk;
          state = 2;
        } else {
          string e = "oczekiwano !DOCTYPE, a znaleziono "+result.second;
          error(e); // BLAD, oczekiwano !DOCTYPE a jest result.second
        }
        nextChar();
      }
    } else if (isalpha(c)) {
      while (isalpha(c)) {
        result.second.push_back(c);
        nextChar();
      }
      result.first = htmlstringtk;
      state = 5;
      // TODO: sprawdz czy to <script>
    } else {
      string e = "oczekiwano !, /, lub litere, a jest ";
      e.push_back(c);
      error(e); // oczekiwano !, / lub znaku alfabetu, a jest c
    }
  } else if (state == 2) {
    // oczekuje doctypestring albo >
    if (c == '>') {
      result.first = tagclosetk;
      nextChar();
    } else {
      while (isprint(c) && c != '>') {
        result.second.push_back(c);
        nextChar();
      }
      result.first = doctypestringtk;
    }
  } else if (state == 3) {
    // oczekuje komentarza lub --
    while (c != '>' && isprint(c)) {
      result.second.push_back(c);
      nextChar();
    }
    if (c == EOF) {
      error("oczekiwano > przed koncem pliku"); // expected > before EOF
    } else if (result.second.substr(result.second.size() - 2) == "--") {
      // czy to rzuca jakims wyjatkiem?
      result.first = htmlstringtk;
      result.second = result.second.substr(0, result.second.size() - 2);
      state = 4;
    } else {
      string e = "oczekiwano > a jest ";
      e.push_back(c);
      error(e); // oczekiwano >
    }
  } else if (state == 4) {
    result.first = tagendcommenttk;
    nextChar();
    state = 0;
  } else if (state == 5) {
    // wewnatrz taga, oczekuje liter i " ' =
    switch (c) {
      case '\'':
        result.first = singlequotetk;
        nextChar();
        break;
      case '"':
        result.first = doublequotetk;
        nextChar();
        break;
      case '=':
        result.first = equaltk;
        nextChar();
        break;
      case '/':
        result.first = htmlslashtk;
        nextChar();
        state = 6;
        break;
      case '>':
        result.first = tagclosetk;
        nextChar();
        state = 0;
        break;
      default:
        while (isalnum(c) || c == '.') {
          result.second.push_back(c);
          nextChar();
        }
        result.first = htmlstringtk;
        break;
    }
  } else if (state == 6) {
    // oczekuje > albo error
    if (c == '>') {
      result.first = tagclosetk;
      state = 0;
      nextChar();
    } else
      error("oczekiwano > a znaleziono" + c); // expecting >
  } else {
    error("niepoprawny stan lexera"); // invalid lexer state
  }

  return result;
}

template<typename T>
void HtmlLexer<T>::error(string e) {
  cout << "HtmlLexer error" << endl << e << endl;
  c = EOF;
  wasError = true;
}

template<typename T>
void HtmlLexer<T>::nextChar() {
  c = sourceFile.nextChar();
}
