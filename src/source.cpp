#include "source.h"

FileSource::FileSource(const string& fname)
  : fileName(fname), posCount(0), lineCount(1), iseof(false) {
  try {
    fileStream.open(fname, ifstream::in);
  } catch (wifstream::failure e) {
    cerr << "Nieudany odczyt pliku" << endl;
    cerr << e.what() << endl;
    exit(1);
  }

}

FileSource::~FileSource() {
  fileStream.close();
}

wchar_t FileSource::nextChar() {
  wchar_t c;
  if (!fileStream.eof()) {
    fileStream.get(c);

    ++posCount;
    if (c == '\n') {
      ++lineCount;
      posCount = 0;
    }

    return c;
  } else
    return EOF;
}

void FileSource::error() {
  cerr << "line: " << lineCount << " pos: " << posCount << endl;
  return;
}


wchar_t StringSource::nextChar() {
  wchar_t c;
  if (!iseof || position < length) {
    c = content[position];
    ++position;

    if (c == EOF)
      iseof = true;

    ++posCount;
    if (c == '\n') {
      ++lineCount;
      posCount = 0;
    }

    return c;
  }
  return EOF;
}

void StringSource::error() {
  return;
}
