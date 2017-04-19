// Modu� MP1SORC.H
// ===============
// Definicja klasy Source.
//
#ifndef MP_SORC_H
#define MP_SORC_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;
/*
   Plik definiuje klas� Source zawieraj�c� struktury
   danych i funkcje us�ugowe zwi�zane z obs�ug� pliku �r�d�owego
   i generacj� raportu kompilacji dla kompilatora MiniPasc.
   Opcja NOLIST powoduje generacj� raportu skr�conego (tylko
   wiersze, w kt�rych wykryto b��dy kompilacji). Pomocnicza klasa
   TextPos jest wykorzystywana wewn�trz klasy Source i w innych
   klasach do rejestracji pozycji w pliku tekstowym, mi�dzy
   innymi w komunikatach o wykrytych b��dach.

   Interfejs publiczny modu�u Source zawiera:
	 NextChar()	- zwraca nast�pny znak strumienia lub EOF
	 GetPos()	- daje pozycj� w pliku ostatniego znaku z NextChar()
	 Error(ercode, txtpos, message1, message2) - do zg�aszania b��d�w.

   Uwaga: Funkcja Error(....) mo�e by� wywo�ywana z 2, 3 lub 4 parametrami.
   =====  Komunikat tekstowy message1 jest traktowany jako g��wny;
   message2 jako komunikat dodatkowy. Mo�liwo�� ta jest wykorzystywana
   w sygnalizacji b��d�w sk�adniowych postaci:
	  "Spodziewany symbol <symbol>" gdzie <...> jest przekazywane
   przez message2 przy ustalonym message1.
*/

enum Options{ NOLIST  = 1,	// NOLIST ==> tylko wiersze z b��dami
			        SHOWIDS = 2,	// Drukuj tabele symboli
              //NOCODE  = 4   // Bez generacji kodu
			};

extern int options;	// Definicja w funkcji main() 

struct TextPos	// Pomocnicza klasa: pozycja w pliku tekstowym
{ int   ln;     // Numer wiersza (od 1)
  int   cn;     // Numer znaku w wierszu (od 1)
  TextPos(int l=0, int c=0): ln(l), cn(c) {}
};

class Source
{
  int etotal, einline;// Liczniki b��d�w: globalny, w wierszu

  const string fn;    // Nazwa pliku
  ifstream istr;
  string Line;        // Bufor bie��cego wiersza

  TextPos tpos;     	// Bie��ca pozycja w pliku

  void PrntLine()		  // Drukowanie bie��cego wiersza
  { cout<<setw(5)<<tpos.ln<<' '<<Line; }

  bool  NextLine();		// Nast�pny wiersz do bufora

public:
  //int options;
  Source(const string& file);
 ~Source();
  void  Error(int ec,const TextPos&tp,const char*mt="",const char*at="");
//  static void  Message(const string& s1,const string& s2="",const string& s3="")
//  { cout<<"CODE  "<<s1<<s2<<s3<<endl; }
  int   NextChar();
  const TextPos& GetPos()const { return tpos; }
};
#endif
