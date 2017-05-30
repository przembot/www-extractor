/*
 * Modul zawierajacy drzewo zapytania oraz tokeny dla HTML
 * Przemyslaw Kopanski
 */
#ifndef _HTML_LEXER_H_
#define _HTML_LEXER_H_

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <ctype.h>

#include "source.h"


using namespace std;


enum HtmlSymType {
    tagopentk // <
  , closingtagopentk // </
  , tagclosetk // >
  , tagselfclosetk // />
  , commenttk // komentarz
  , htmlstringtk // tagname, attrname, attrval
  , textstringtk // string wraz z & # ;
  , doctypetk // doctype, wraz z zawartoscia
  , equaltk // znak rownosci
  , singlequotetk // '<wartosc>'
  , doublequotetk // "<wartosc>"
  , novaltk // wartosc atrybutu niezdefiniowana
  , unknowntk // koniec pliku
};


// Czy uzyc variant<char, string> zamiast string?
typedef pair<HtmlSymType, wstring> HtmlSymbol;


// T - klasa obslugujaca wczytywanie kodu zrodlowego
// musi zawierac nextChar, error
// a jej konstruktor przyjmowac stringa
class HtmlLexer {
  public:
    HtmlLexer(unique_ptr<Source> &source);
    HtmlLexer(unique_ptr<Source> &&source);
    ~HtmlLexer() {}
    HtmlSymbol nextMetaSymbol();
    bool tryNextTextSymbol(HtmlSymbol&);
    wstring skipTag(wstring tagname);
    void error(wstring e);
    wchar_t currentChar();
    bool errorOccured();
    void pushBackTokens(initializer_list<HtmlSymbol> il);

  private:
    unique_ptr<Source> sourceFile;
    list<HtmlSymbol> tokenBuffer;
    void nextChar();
    wchar_t c; // pierwszy nieprzetworzony znak
    bool wasError; // czy wystapil blad
    void ignoreSpaces();
};


// util
wstring char2str(wchar_t c);

#endif
