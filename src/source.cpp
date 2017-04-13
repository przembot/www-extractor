#include "source.h"

Source::Source(const string& fname) : fileName(fname), posCount(0), lineCount(0), iseof(false) {
  fileStream.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
  try {
    fileStream.open(fname, ifstream::in);
  } catch (ifstream::failure e) {
    cerr << "Nieudany odczyt pliku" << endl;
    cerr << e.what() << endl;
    exit(1);
  }

}

Source::~Source() {
  fileStream.close();
}

char Source::nextChar() {
  char c;
  if (!iseof) {
    fileStream.get(c);

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

void Source::error() {
  return;
}


char StringSource::nextChar() {
  char c;
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
