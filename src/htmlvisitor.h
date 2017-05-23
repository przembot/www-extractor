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
    PrintVisitor(ostream& stream);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
  private:
    ostream& stream;
};


class ToStringVisitor : public Visitor {
  public:
    ToStringVisitor(bool content, bool children);
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
    const string& getResult();

  private:
    string result;
    bool content;
    bool children;
};

ostream& operator<<(ostream& stream, const Htmlstart& s);
ostream& operator<<(ostream& stream, const Node* s);
ostream& operator<<(ostream& stream, const Textnode& s);
ostream& operator<<(ostream& stream, const Htmlnode& s);
ostream& operator<<(ostream& stream, const Emptyhtmlnode& s);



#endif // _HTML_VISITOR_H_
