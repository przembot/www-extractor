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
    virtual ~Source() {}
    virtual wchar_t nextChar() = 0;
    virtual void error() = 0;
};

class FileSource : public Source {
  public:
    FileSource(const string &fname);
    ~FileSource();
    wchar_t nextChar();
    void error();

  private:
    const string fileName;
    int posCount;
    int lineCount;
    wifstream fileStream;
    bool iseof;
};


// Do testowania lexera
class StringSource : public Source {
  public:
    StringSource(const wstring &content) : content(content), length(content.length()), position(0) {} ;
    ~StringSource() {} ;
    wchar_t nextChar();
    void error();

  private:
    const wstring content;
    const int length;
    int posCount;
    int lineCount;
    int position;
    bool iseof;
};


#endif
