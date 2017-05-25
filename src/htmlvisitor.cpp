#include "htmlvisitor.h"

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

ToStringVisitor::ToStringVisitor(bool content, bool children)
  : content(content), children(children)
{}

const void ToStringVisitor::visit(const Htmlnode *n) {
  if (!content) {
    result += "<" + n->tag_name + " ";
    for (const auto& attr : n->attributes)
      result += attr.first + "=" + attr.second + " ";
    result += ">";
  }

  for (const auto& child : n->children)
    child->accept(*this);

  if (!content) {
    result += "</" + n->tag_name + ">";
  }
}

const void ToStringVisitor::visit(const Emptyhtmlnode *n) {
  if (children) {
    result += "<" + n->tag_name + " ";
    for (const auto& attr : n->attributes)
      result += attr.first + "=" + attr.second + " ";
    result += "/>";
  }
}

const void ToStringVisitor::visit(const Textnode *n) {
  if (content)
    result += n->content;
}

const string& ToStringVisitor::getResult() {
  return result;
}


ostream& operator<<(ostream& stream, const Htmlstart& n) {
  for (auto i : n.nodes)
    stream << i;
  return stream;
}


PrintVisitor::PrintVisitor(ostream& stream)
  : stream(stream) {
}

const void PrintVisitor::visit(const Htmlnode *n) {
  stream << *n;
}

const void PrintVisitor::visit(const Emptyhtmlnode *n) {
  stream << *n;
}

const void PrintVisitor::visit(const Textnode *n) {
  stream << *n;
}
