// Modu³ MP1SORC.H
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
   Plik definiuje klasê Source zawieraj¹c¹ struktury
   danych i funkcje us³ugowe zwi¹zane z obs³ug¹ pliku Ÿród³owego
   i generacj¹ raportu kompilacji dla kompilatora MiniPasc.
   Opcja NOLIST powoduje generacjê raportu skróconego (tylko
   wiersze, w których wykryto b³êdy kompilacji). Pomocnicza klasa
   TextPos jest wykorzystywana wewn¹trz klasy Source i w innych
   klasach do rejestracji pozycji w pliku tekstowym, miêdzy
   innymi w komunikatach o wykrytych b³êdach.

   Interfejs publiczny modu³u Source zawiera:
	 NextChar()	- zwraca nastêpny znak strumienia lub EOF
	 GetPos()	- daje pozycjê w pliku ostatniego znaku z NextChar()
	 Error(ercode, txtpos, message1, message2) - do zg³aszania b³êdów.

   Uwaga: Funkcja Error(....) mo¿e byæ wywo³ywana z 2, 3 lub 4 parametrami.
   =====  Komunikat tekstowy message1 jest traktowany jako g³ówny;
   message2 jako komunikat dodatkowy. Mo¿liwoœæ ta jest wykorzystywana
   w sygnalizacji b³êdów sk³adniowych postaci:
	  "Spodziewany symbol <symbol>" gdzie <...> jest przekazywane
   przez message2 przy ustalonym message1.
*/

enum Options{ NOLIST  = 1,	// NOLIST ==> tylko wiersze z b³êdami
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
  int etotal, einline;// Liczniki b³êdów: globalny, w wierszu

  const string fn;    // Nazwa pliku
  ifstream istr;
  string Line;        // Bufor bie¿¹cego wiersza

  TextPos tpos;     	// Bie¿¹ca pozycja w pliku

  void PrntLine()		  // Drukowanie bie¿¹cego wiersza
  { cout<<setw(5)<<tpos.ln<<' '<<Line; }

  bool  NextLine();		// Nastêpny wiersz do bufora

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
