#include "htmllexer.h"

wstring char2str(wchar_t c) {
  wstring a;
  a.push_back(c);
  return a;
}


HtmlLexer::HtmlLexer(unique_ptr<Source> &source)
  : sourceFile(move(source)), wasError(false) {
  nextChar();
};


HtmlLexer::HtmlLexer(unique_ptr<Source> &&source)
  : sourceFile(move(source)), wasError(false) {
  nextChar();
};

void HtmlLexer::ignoreSpaces() {
  // Pomijanie bialych znakow
  while (isspace(c))
    nextChar();
}


wchar_t HtmlLexer::currentChar() {
  ignoreSpaces();
  return c;
}


bool HtmlLexer::errorOccured() {
  return wasError;
}


wstring HtmlLexer::skipTag(wstring tag) {
  // omija wszystko do nazwy name
  // zwraca to co bylo do czasu napotkania name
  wstring buf;
  auto pos = wstring::npos;
  while (pos == wstring::npos && (isprint(c) || isspace(c))) {
    //if (!isspasce(c))
    buf.push_back(c);
    pos = buf.find(tag);
    nextChar();
  }
  if (pos == wstring::npos) { // napotkano koniec pliku zanim znaleziono tag
    error(L"nie znaleziono "+tag);
    cerr << c << endl;
    wcerr << buf << endl;
  } else
    buf.erase(buf.size() - tag.size());
  return buf;
}


HtmlSymbol HtmlLexer::nextMetaSymbol() {
  HtmlSymbol result;
  // wylapanie otwarcie tag
  // <
  // </
  // <!-- az do -->
  // <!DOCTYPE az do >
  // =
  // slowo (word)
  // wartosc quotowana
  // close symbol

  if (!tokenBuffer.empty()) {
    result = tokenBuffer.front();
    tokenBuffer.pop_front();
    return result;
  }

  ignoreSpaces();

  if (c == EOF || c == 0 || wasError)
    result.first = unknowntk;
  else if (c == '<') {
    nextChar();
    if (c == '/') {
      result.first = closingtagopentk;
      nextChar();
    } else if (c == '!') {
      nextChar();
      if (c == '-') {// comment
        nextChar();
        if (c != '-') {
          result.first = unknowntk;
          error(L"oczekiwano - a napotkano "+char2str(c));
        } else {
          nextChar();
          result.second = skipTag(L"-->");
          if (wasError)
            result.first = unknowntk;
          else
            result.first = commenttk;
        }
      } else if (c == 'D' || c == 'd') { // doctype
        for (int i = 0; i < 7; ++i) { result.second.push_back(c); nextChar(); }
        transform(result.second.begin(), result.second.end(), result.second.begin(), ::tolower);
        if (result.second == L"doctype") {
          result.second = skipTag(L">");
          result.first = doctypetk;
        } else {
          result.first = unknowntk;
          error(L"oczekiwano DOCTYPE, a napotkano "+result.second);
        }
      } else { // nieoczekiwany symbol
        error(L"oczekiwano !-- lub !DOCTYPE, a napotkano "+char2str(c));
      }
    } else // tag otwierajacy
      result.first = tagopentk;
  }
  // slowo
  else if (isalpha(c)) {
    result.first = htmlstringtk;
    while (isalnum(c) || c == '-') {
      result.second.push_back(c);
      nextChar();
    }
  }
  // wartosc quotowana
  else if (c == '\'' || c == '"') {
    wchar_t quotekind = c;
    nextChar();
    while (c != EOF && c != quotekind) {
      result.second.push_back(c);
      nextChar();
    }
    if (c != quotekind) {
      result.first = unknowntk;
      error(L"oczekiwano "+char2str(quotekind)+L" a otrzymano "+char2str(c));
    } else
      result.first = quotekind=='\''?singlequotetk:doublequotetk;
    nextChar();
  }
  // zamkniecie tagu
  else if (c == '/') {
    nextChar();
    ignoreSpaces();
    if (c == '>')
      result.first = tagselfclosetk;
    else {
      result.first = unknowntk;
      error(L"oczekiwano > a napotkano"+char2str(c));
    }
    nextChar();
  } else if (c == '>') {
    result.first = tagclosetk;
    nextChar();
  }
  // znak rownosci
  else if (c == '=') {
    result.first = equaltk;
    nextChar();
  } else {
    result.first = unknowntk;
    error(L"oczekiwano metaznaku a napotkano "+char2str(c));
  }

  //cout << "typ: " << result.first << " wartosc: " << result.second << endl;
  return result;
}


bool HtmlLexer::tryNextTextSymbol(HtmlSymbol &result) {
  // true - udalo sie wytworzyc tekst - wynik w &result
  // false - nie udalo sie zdobyc tekstu, &result niezdefiniowana wartosc

  if (!tokenBuffer.empty()) {
    result = tokenBuffer.front();
    if (result.first == textstringtk) {
      tokenBuffer.pop_front();
      return true;
    } else
      return false;
  }
  //ignoreSpaces();

  if (c == EOF || wasError)
    result.first = unknowntk;
  else if (c != '<' && (isprint(c) || isspace(c))) {
    result.first = textstringtk;
    result.second = L"";
    while (c != '<' && (isprint(c) || isspace(c))) {
      result.second.push_back(c);
      nextChar();
    }
    return true;
  }

  return false;
}

void HtmlLexer::pushBackTokens(initializer_list<HtmlSymbol> il) {
  tokenBuffer.insert(tokenBuffer.end(), il);
}

void HtmlLexer::error(wstring e) {
  cerr << "HtmlLexer error" << endl;
  wcerr << e << endl;
  sourceFile->error();
  c = EOF;
  wasError = true;
}


void HtmlLexer::nextChar() {
  c = sourceFile->nextChar();
}
