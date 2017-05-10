#include "htmlparser.h"


const bool Htmlstart::operator== (const Htmlstart& rhs) const {
  if (nodes.size() == rhs.nodes.size()) {
    auto it1 = nodes.begin();
    auto it2 = rhs.nodes.begin();
    for (; it1 != nodes.end() && it2 != rhs.nodes.end(); ++it1, ++it2)
      if (!equalsNode(*it1, *it2))
        return false;
    return true;
  }
  return false;
}


const bool Htmlnode::operator== (const Htmlnode& rhs) const {
  if (tag_name == rhs.tag_name && attributes == rhs.attributes
      && children.size() == rhs.children.size()) {
    auto it1 = children.begin();
    auto it2 = rhs.children.begin();
    for (; it1 != children.end() && it2 != rhs.children.end(); ++it1, ++it2)
      if (!equalsNode(*it1, *it2))
        return false;
    return true;
  }
  return false;
}


const bool Emptyhtmlnode::operator== (const Emptyhtmlnode& rhs) const {
  return tag_name == rhs.tag_name && attributes == rhs.attributes;
}


const bool Textnode::operator== (const Textnode& rhs) const {
  return content == rhs.content;
}


void Htmlnode::accept(Visitor &v) const {
  v.visit(this);
}

void Textnode::accept(Visitor &v) const {
  v.visit(this);
}

void Emptyhtmlnode::accept(Visitor &v) const {
  v.visit(this);
}


ostream& operator<<(ostream& stream, const Textnode& s) {
  stream << s.content << endl;
  return stream;
}


ostream& operator<<(ostream& stream, const Emptyhtmlnode& s) {
  stream << s.tag_name << " ";
  for (auto p : s.attributes) {
    if (p.second == "")
      stream << p.first << " ";
    else
      stream << p.first << "=" << p.second << " ";
  }
  stream << endl;
  return stream;
}

ostream& operator<<(ostream& stream, const Htmlnode& s) {
  stream << s.tag_name << " ";
  for (auto p : s.attributes) {
    if (p.second == "")
      stream << p.first << " ";
    else
      stream << p.first << "=" << p.second << " ";
  }
  stream << endl;
  for (auto n : s.children)
    stream << n;
  return stream;
}


ostream& operator<<(ostream& stream, const Node* s) {
  PrintVisitor v(stream);
  s->accept(v);
  return stream;
}


bool equalsNode(const Node* a, const Node* b) {
  const Htmlnode *h1, *h2;
  const Emptyhtmlnode *e1, *e2;
  const Textnode *t1, *t2;
  if ((h1 = dynamic_cast<const Htmlnode*>(a)) && (h2 = dynamic_cast<const Htmlnode*>(b)))
    return *h1 == *h2;
  else if ((e1 = dynamic_cast<const Emptyhtmlnode*>(a)) && (e2 = dynamic_cast<const Emptyhtmlnode*>(b)))
    return *e1 == *e2;
  else if ((t1 = dynamic_cast<const Textnode*>(a)) && (t2 = dynamic_cast<const Textnode*>(b)))
    return *t1 == *t2;
  else
    return false;
}

ostream& operator<<(ostream& stream, const Htmlstart& n) {
  for (auto i : n.nodes)
    stream << i;
  return stream;
}


PrintVisitor::PrintVisitor(ostream& stream)
  : Visitor(), stream(stream) {
}

const void PrintVisitor::visit(const Htmlnode *n) const {
  stream << *n;
}

const void PrintVisitor::visit(const Emptyhtmlnode *n) const {
  stream << *n;
}

const void PrintVisitor::visit(const Textnode *n) const {
  stream << *n;
}
