#ifndef _HTML_VISITOR_H_
#define _HTML_VISITOR_H_

#include <iostream>

#include "htmlparser.h"

struct Htmlnode;
struct Emptyhtmlnode;
struct Textnode;
struct Htmlstart;
struct Node;

class Visitor {
  public:
    virtual const void visit(const Htmlnode *n) = 0;
    virtual const void visit(const Emptyhtmlnode *n) = 0;
    virtual const void visit(const Textnode *n) = 0;
};


class PrintVisitor : public Visitor {
  public:
    PrintVisitor(wostream& stream);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
  private:
    wostream& stream;
};


class ToStringVisitor : public Visitor {
  public:
    ToStringVisitor(bool content, bool children);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
    const wstring& getResult();

  private:
    wstring result;
    bool content;
    bool children;
};

wostream& operator<<(wostream& stream, const Htmlstart& s);
wostream& operator<<(wostream& stream, const Node* s);
wostream& operator<<(wostream& stream, const Textnode& s);
wostream& operator<<(wostream& stream, const Htmlnode& s);
wostream& operator<<(wostream& stream, const Emptyhtmlnode& s);



#endif // _HTML_VISITOR_H_
