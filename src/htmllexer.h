/*
 * Modul zawierajacy drzewo zapytania oraz tokeny dla HTML
 * Przemyslaw Kopanski
 */
#ifndef _HTML_H_
#define _HTML_H_

#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <ctype.h>
#include "source.h"

using namespace std;

/*
- TOK_TAGOPEN - `<`
- TOK_TAGCLOSE - `>`
- TOK_TAGCLOSESLASH - `/`
- TOK_ATTRVAL - `=`
- TOK_SINGLEQUOTE - `'`
- TOK_DOUBLEQUOTE - `"`
- TOK_COMMENTBEGIN - `!--`
- TOK_COMMENTEND - `--`
- TOK_DOCTYPEBEGIN - `!DOCTYPE`
- TOK_TAGNAME - string
- TOK_ATTRNAME - string
- TOK_ATTRVAL - string
- TOK_TEXTCONTENT - string
*/

struct node {
  virtual string contentString() = 0; // zwraca tylko content jako ciag znakow
  virtual string childrenString() = 0; // zwraca tylko potomkow jako ciag znakow
  virtual string allString() = 0; // zwraca potomkow oraz content jako ciag znakow
};

struct htmlstart {
  list<node*> nodes;
};

struct htmlnode : node {
  string tag_name;
  map<string, string> attributes;
  list<node*> children;
};

struct emptyhtmlnode : node {
  string tag_name;
  map<string, string> attributes;
};

struct textnode : node {
  string content;
};


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
template<typename T>
class HtmlLexer {
  public:
    HtmlLexer(const string filename);
    ~HtmlLexer() {};
    HtmlSymbol nextMetaSymbol();
    HtmlSymbol nextTextSymbol();
    string skipTag(string tagname);
    void error(string e);
    char currentChar();
    bool errorOccured();

  private:
    T sourceFile;
    void nextChar();
    char c; // pierwszy nieprzetworzony znak
    bool wasError; // czy wystapil blad
    void ignoreSpaces();
};

#include "htmllexer.tpp"

#endif
