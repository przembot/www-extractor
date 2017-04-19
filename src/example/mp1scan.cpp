// Modu� mp1scan.cpp
// =================
// Defnicje funkcji sk�adowych klasy Scan, tabeli leksem�w AT
// i tabeli s��w kluczowych.
//
#include "mp1scan.h"

char *AT[MAXSYM+1] =	// Tabela Atom�w (dla komunikat�w o b��dach
{
  "do",    "if",    "of",     "or",    "and",  "div",
  "end",   "not",   "var",    "else",  "read", "then",
  "array", "begin", "while",  "write", "prog", "proc",
  "ident", "intconst","charconst",

  "*", "+", "-", "<", "<=", ">", ">=", "<>", "=", ")", "(",
  "[", "]", ",", ";", ".",  ":", ":=", "..",  "others"
};

Scan::KeyRec Scan::KT[NKEYS]= 	// Kolejno�� wg funkcji hash(keyword)
{
// Keyword		 Atom			hash(keyword)
//-------------------------------------------
  { "not",       notop  }, 	//  0
  { "write",     writesy},  //  1
  { "of",        ofsy   },  //  2
  { "array",     arraysy},  //  3
  { "and",       andop  },  //  4
  { "procedure", procsy },  //  5
  { "do",        dosy   },  //  6
  { "var",       varsy  },  //  7
  { "if",        ifsy   },  //  8
  { "begin",     beginsy},  //  9
  { "end",       endsy  },  //  10
  { "while",     whilesy},  //  11
  { "then",      thensy },  //  12
  { "program",   progsy },  //  13
  { "or",        orop   },  //  14
  { "read",      readsy },  //  15
  { "div",       divop  },  //  16
  { "else",      elsesy }   //  17
};

SymType Scan::NextSymbol()
{ 
  do
  {
    while(isspace(c)) Nextc();	// Pomi� znaki bia�e
    if(c==EOF) return others;
    if(c=='{') { do Nextc(); while(c!='}'); Nextc(); } else
    if(c=='/')
	  { atompos=src.GetPos(); // Mo�e by� divop albo komentarz

      Nextc();
	    if(c!='/') return divop; else
        do Nextc(); while(c!='\n');
    }
  } while(isspace(c) || c=='{' || c=='/');

  atompos=src.GetPos(); // B�dzie jaki� atom leksykalny

  //---Identyfikator lub s�owo kluczowe
  if(isalpha(c))
  { unsigned len, h;
    spell.clear();

    do
    { spell.push_back(c);
      Nextc();
    } while(isalnum(c));
    len = spell.size();
    h = hash(spell);
    if(KT[h].kw == spell)
      return KT[h].atom; 
    else return ident;
  }
  else

  //---Sta�a ca�kowita
  if(isdigit(c))
  { bool big=false; unsigned long long ul=0;
    do
    { ul = ul*10+(c-'0');
      big = big || ul>INT_MAX;
      Nextc();
    } while(isdigit(c));
    intconstant = (int)ul;
    if(big) 
      ScanError(ICONST2BIG, "Przekroczony zakres sta�ej ca�kowitej");
    return intconst;
  }
  else

  //---Pozosta�e atomy
  switch(c)
  {
    //----Sta�a znakowa
    case '"': Nextc();
		    if(c=='"')
		    { Nextc();
		      if(c!='"') ScanError(CARCONSTWRONG, "B��dna sta�a znakowa");
		    }
		    intconstant=c;
		    Nextc();
		    if(c!='"') ScanError(CARCONSTWRONG, "B��dna sta�a znakowa");
		    else Nextc();
		    return charconst;
    //----Operatory 2 i 1 znakowe
    case ':': Nextc();
		    if (c=='=') { Nextc(); return becomes; }
		    return colon;
    case '.': Nextc();
		    if(c=='.') { Nextc(); return thru;}
		    return period;
    case '<': Nextc();
		    if(c=='=') { Nextc(); return leop;}
		    else if(c=='>') { Nextc(); return neop; }
		    return ltop;
    case '>': Nextc();
		    if(c=='=') { Nextc(); return geop; }
		    return gtop;

    //----Operatory 1 znakowe
    case '+': Nextc(); return plus;
    case '-': Nextc(); return minus;
    case '*': Nextc(); return times;
    case '=': Nextc(); return eqop;
    case '/': Nextc(); return divop;
    case '(': Nextc(); return lparent;
    case ')': Nextc(); return rparent;
    case '[': Nextc(); return lbracket;
    case ']': Nextc(); return rbracket;
    case ',': Nextc(); return comma;
    case ';': Nextc(); return semicolon;

    //----Nielegalne znaki
    default : Nextc(); return others;
  }
}
