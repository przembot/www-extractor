// Modu� MP1SCAN.H
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
   Klasa Scan zawiera struktury danych i us�ugi zwi�zane z analiz�
   leksykaln�. Wykorzystywany jest publiczny interface klasy Source.
   Obiekt typu Scan zawiera referencj� na obiekt typu Source, kt�ry
   dostarcza strumienia znak�w i zajmuje si� wsp�prac� z plikami.
   Rozpoznawane symbole leksykalne: identyfikatory, s�owa kluczowe,
   sta�e ca�kowite i znakowe, operatory i ograniczniki; wykaz atom�w
   zawarty jest w typie wyliczeniowym SymType. MAXSYM okre�la liczb�
   r�nych atom�w rozpoznawanych przez g��wn� funkcj� interfejsu
   publicznego NextSymbol().
   W strumieniu wej�ciowym analizatora leksykalnego pomijane s�
   komentarze postaci {...bez zagnie�d�e�...} oraz //...<new_line>.
   Separatorem atom�w leksykalnych jest dowolny niepusty ci�g znak�w
   "bia�ych"; komentarz, z tego punktu widzenia, jest traktowany jak
   pojedy�czy odst�p.
   Powi�zanie modu��w Source i Scan jest nast�puj�ce:
   ----------\                   ---------------\
   |         |====>NextChar()===>| c            |====>NextSymbol()
   | SOURCE  |====>GetPos()=====>| atompos SCAN |====>IntConst()
   |         |<====Error()======<|              |====>Spell()
   ----------/                   ---------------/
*/

class Scan
{
  struct KeyRec   // Pomocniczy deskryptor s�owa kluczowego
  { string kw;		// S�owo kluczowe
    SymType atom; // Odpowiadaj�cy mu atom
  };
  static KeyRec KT[NKEYS];	// Tabela s��w kluczowych

  int     c;			    // Aktualny znak z src
  TextPos atompos;	  // Pozycja atomu w tekscie
  int     intconstant;// Warto�� ostatniej sta�ej liczbowej
  string  spell;      // Ostatnio wczytany ident

  void Nextc() { c=src.NextChar(); }  // Pobieranie znak�w
  
  unsigned hash(const string& s)	// Funkcja mieszaj�ca dla KT
  //
  // Funkcja okre�lona eksperymentalnie: dla tabeli NKEYS==18
  // elementowej (wszystkie pozycje zaj�te) jest 1 kolizja:
  //     hash("not") == hash("else") == 7; brak kodu 0
  // Kolizj� usuni�to eksplicite.
  //
  { size_t len = s.size();
    unsigned short h;

    if(s == "not") return 0; // Korekta kolizji
    h = (s[0]>>1) * s[1] * 1241;
    if(len>=3) h += s[len-1];
    return h%NKEYS;
  }

public:
  Source& src;			  // Referencja na strumie� znak�w

  Scan(Source &s):src(s) { Nextc(); }
  SymType NextSymbol();		// Zwraca nast�pny atom leksykalny
  int IntConst() { return intconstant; }
  const string& Spell() { return spell; }
  void ScanError(int ecode, const char *mtxt="", const char *atxt="")
  { src.Error(ecode, atompos, mtxt, atxt); }
};

#endif
