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
typedef pair<HtmlSymType, string> HtmlSymbol;


// T - klasa obslugujaca wczytywanie kodu zrodlowego
// musi zawierac nextChar, error
// a jej konstruktor przyjmowac stringa
class HtmlLexer {
  public:
    HtmlLexer(unique_ptr<Source> &source);
    HtmlLexer(unique_ptr<Source> &&source);
    ~HtmlLexer() {}
    HtmlSymbol nextMetaSymbol();
    HtmlSymbol nextTextSymbol();
    string skipTag(string tagname);
    void error(string e);
    char currentChar();
    bool errorOccured();

  private:
    unique_ptr<Source> sourceFile;
    void nextChar();
    char c; // pierwszy nieprzetworzony znak
    bool wasError; // czy wystapil blad
    void ignoreSpaces();
};


// util
string char2str(char c);

#endif
