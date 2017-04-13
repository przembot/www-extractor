/**
 * Modul wczytywania kodu zrodlowego z pliku
 * mozliwosc generalizacji - czytanie z internetu etc.
 * Przemyslaw Kopanski
 */
#ifndef _source_h_
#define _source_h_

#include <iostream>
#include <fstream>
#include <exception>

using namespace std;

class Source {
  public:
    Source(const string &fname);
    ~Source();
    char nextChar();
    void error(); // TODO: typ bledu, informacja o bledzie

  private:
    const string fileName;
    int posCount;
    int lineCount;
    ifstream fileStream;
    bool iseof;
};


// Do testowania lexera
class StringSource {
  public:
    StringSource(const string &content) : content(content), length(content.length()), position(0) {} ;
    ~StringSource() {} ;
    char nextChar();
    void error();

  private:
    const string content;
    const int length;
    int posCount;
    int lineCount;
    int position;
    bool iseof;
};


#endif
