// Modu� MP1SORC.CPP
// =================
// Funkcje sk�adowe klasy Source.
//
#include "mp1sorc.h"

Source::Source(const string& fnam): fn(fnam)
{
  istr.open(fn.c_str());
  if(!istr)
  { cout<<"MiniPasc: B��D FATALNY - nie mo�na otworzy� pliku \""<<fn<<"\"\n";
    exit(1);
  }
  cout<<"\nKompilator MiniPasc, v.1.5, (c) AP(IIPW)\n";
  cout<<((options&NOLIST)?"Skr�cony":"Pe�ny");
  cout<<" raport kompilacji dla pliku: \""<<fnam<<"\"\n\n";
  etotal = 0;
  NextLine();   // Pierwszy wiersz
}

Source::~Source()
{
  //LastMessage
  cout<<"\nMiniPasc: koniec raportu. Wykrytych b��d�w: "<<etotal<<'\n';
  istr.close();
}

bool Source::NextLine()	// Zwraca true je�li jest nast�pny wiersz
{
  if(istr.eof()) return false;
  getline(istr, Line);  // Pobiera wiersz (bez znaku '\n')
  Line.push_back('\n');

  tpos.ln++;	// Nast�pny wiersz
  tpos.cn=0;	// Przed pierwszym znakiem
  while(Line[tpos.cn]==' ' || Line[tpos.cn]=='\t') tpos.cn++;
  if( (options&NOLIST)==0 ) PrntLine();

  einline=0;	// 0 b��d�w w tym wierszu
  return true;
}

void Source::Error(int ec,const TextPos&tp,const char*mt,const char*at)
{ etotal++;
  if((options&NOLIST) && einline==0)	// Jest pierwszy b��d w wierszu
    cout<<setw(5)<<tpos.ln<<' '<<Line;	// Druk wiersza �r�d�owego
  einline=1;

  cout<<setw(2)<<ec<<"*** ";
  cout<<setw(tp.cn)<<setfill('-')<<'^'<<setfill(' ')<<mt<<at<<'\n';
}

int Source::NextChar()
{ bool r=true;
  if(tpos.cn==Line.size()) r=NextLine();
  if(r) return Line[tpos.cn++]; else return EOF;
}

