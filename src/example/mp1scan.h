// Modu³ MP1SCAN.H
// ===============
// Definicja klasy Scan i typu SymType.
//
#ifndef MP1SCAN_H
#define MP1SCAN_H

#include <climits>
#include <cctype>
#include "mp1sorc.h"

enum SymType	// Atomy leksykalne Mini Pascal'a
{ dosy,    ifsy,    ofsy,     orop,    andop,  divop,
  endsy,   notop,   varsy,    elsesy,  readsy, thensy,
  arraysy, beginsy, whilesy,  writesy, progsy, procsy,	// Keywords

  NKEYS,   MAXKEY=procsy,

  ident,   intconst,charconst,times,   plus,   minus,
  ltop,    leop,    gtop,     geop,    neop,   eqop,
  rparent, lparent, lbracket, rbracket,comma,  semicolon,
  period,  colon,   becomes,  thru,    others, MAXSYM=others+1
};

enum ScanErrors{ CARCONSTWRONG=1, ICONST2BIG };
/*
   Klasa Scan
   ==========
   Klasa Scan zawiera struktury danych i us³ugi zwi¹zane z analiz¹
   leksykaln¹. Wykorzystywany jest publiczny interface klasy Source.
   Obiekt typu Scan zawiera referencjê na obiekt typu Source, który
   dostarcza strumienia znaków i zajmuje siê wspó³prac¹ z plikami.
   Rozpoznawane symbole leksykalne: identyfikatory, s³owa kluczowe,
   sta³e ca³kowite i znakowe, operatory i ograniczniki; wykaz atomów
   zawarty jest w typie wyliczeniowym SymType. MAXSYM okrežla liczbê
   ró§nych atomów rozpoznawanych przez g³ówn¹ funkcjê interfejsu
   publicznego NextSymbol().
   W strumieniu wejžciowym analizatora leksykalnego pomijane s¹
   komentarze postaci {...bez zagnie§d§e¤...} oraz //...<new_line>.
   Separatorem atomów leksykalnych jest dowolny niepusty ci¹g znaków
   "bia³ych"; komentarz, z tego punktu widzenia, jest traktowany jak
   pojedy¤czy odstêp.
   Powi¹zanie modu³ów Source i Scan jest nastêpuj¹ce:
   ----------\                   ---------------\
   |         |====>NextChar()===>| c            |====>NextSymbol()
   | SOURCE  |====>GetPos()=====>| atompos SCAN |====>IntConst()
   |         |<====Error()======<|              |====>Spell()
   ----------/                   ---------------/
*/

class Scan
{
  struct KeyRec   // Pomocniczy deskryptor s³owa kluczowego
  { string kw;		// S³owo kluczowe
    SymType atom; // Odpowiadaj¹cy mu atom
  };
  static KeyRec KT[NKEYS];	// Tabela s³ów kluczowych

  int     c;			    // Aktualny znak z src
  TextPos atompos;	  // Pozycja atomu w tekscie
  int     intconstant;// Wartož ostatniej sta³ej liczbowej
  string  spell;      // Ostatnio wczytany ident

  void Nextc() { c=src.NextChar(); }  // Pobieranie znaków
  
  unsigned hash(const string& s)	// Funkcja mieszaj¹ca dla KT
  //
  // Funkcja okreœlona eksperymentalnie: dla tabeli NKEYS==18
  // elementowej (wszystkie pozycje zajête) jest 1 kolizja:
  //     hash("not") == hash("else") == 7; brak kodu 0
  // Kolizjê usuniêto eksplicite.
  //
  { size_t len = s.size();
    unsigned short h;

    if(s == "not") return 0; // Korekta kolizji
    h = (s[0]>>1) * s[1] * 1241;
    if(len>=3) h += s[len-1];
    return h%NKEYS;
  }

public:
  Source& src;			  // Referencja na strumie¤ znaków

  Scan(Source &s):src(s) { Nextc(); }
  SymType NextSymbol();		// Zwraca nastêpny atom leksykalny
  int IntConst() { return intconstant; }
  const string& Spell() { return spell; }
  void ScanError(int ecode, const char *mtxt="", const char *atxt="")
  { src.Error(ecode, atompos, mtxt, atxt); }
};

#endif
