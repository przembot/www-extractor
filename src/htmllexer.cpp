#include "htmllexer.h"

string char2str(char c) {
  string a;
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


char HtmlLexer::currentChar() {
  ignoreSpaces();
  return c;
}


bool HtmlLexer::errorOccured() {
  return wasError;
}


string HtmlLexer::skipTag(string tag) {
  // omija wszystko do nazwy name
  // zwraca to co bylo do czasu napotkania name
  string buf;
  auto pos = string::npos;
  while (pos == string::npos && isprint(c)) {
    //if (!isspasce(c))
    buf.push_back(c);
    pos = buf.find(tag);
    nextChar();
  }
  if (pos == string::npos) // napotkano koniec pliku zanim znaleziono tag
    error("nie znaleziono "+tag);
  else
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
          error("oczekiwano - a napotkano "+char2str(c));
        } else {
          nextChar();
          result.second = skipTag("-->");
          if (wasError)
            result.first = unknowntk;
          else
            result.first = commenttk;
        }
      } else if (c == 'D') { // doctype
        for (int i = 0; i < 7; ++i) { result.second.push_back(c); nextChar(); }
        if (result.second == "DOCTYPE") {
          result.second = skipTag(">");
          result.first = doctypetk;
        } else {
          result.first = unknowntk;
          error("oczekiwano DOCTYPE, a napotkano "+result.second);
        }
      } else { // nieoczekiwany symbol
        error("oczekiwano !-- lub !DOCTYPE, a napotkano "+char2str(c));
      }
    } else // tag otwierajacy
      result.first = tagopentk;
  }
  // slowo
  else if (isalpha(c)) {
    result.first = htmlstringtk;
    while (isalnum(c)) {
      result.second.push_back(c);
      nextChar();
    }
  }
  // wartosc quotowana
  else if (c == '\'' || c == '"') {
    char quotekind = c;
    nextChar();
    while (c != EOF && c != quotekind) {
      result.second.push_back(c);
      nextChar();
    }
    if (c != quotekind) {
      result.first = unknowntk;
      error("oczekiwano "+char2str(quotekind)+" a otrzymano "+char2str(c));
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
      error("oczekiwano > a napotkano"+char2str(c));
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
    error("oczekiwano metaznaku a napotkano "+char2str(c));
  }

  //cout << "typ: " << result.first << " wartosc: " << result.second << endl;
  return result;
}


HtmlSymbol HtmlLexer::nextTextSymbol() {
  // podana funkcja nie moze skonczys sie bledem
  // co najwyzej text bedzie pusty
  HtmlSymbol result;

  //ignoreSpaces();

  if (c == EOF || wasError)
    result.first = unknowntk;
  else {
    result.first = textstringtk;
    // TODO: &gt; etc
    while (c != '<' && (isprint(c) || isspace(c))) {
      result.second.push_back(c);
      nextChar();
    }
  }
  return result;
}


void HtmlLexer::error(string e) {
  cout << "HtmlLexer error" << endl << e << endl;
  c = EOF;
  wasError = true;
}


void HtmlLexer::nextChar() {
  c = sourceFile->nextChar();
}
