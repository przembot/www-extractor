#include "htmlvisitor.h"

wostream& operator<<(wostream& stream, const Textnode& s) {
  stream << s.content << endl;
  return stream;
}


wostream& operator<<(wostream& stream, const Emptyhtmlnode& s) {
  stream << s.tag_name << L" ";
  for (auto p : s.attributes) {
    if (p.second == L"")
      stream << p.first << L" ";
    else
      stream << p.first << L"=" << p.second << L" ";
  }
  stream << endl;
  return stream;
}

wostream& operator<<(wostream& stream, const Htmlnode& s) {
  stream << s.tag_name << L" ";
  for (auto p : s.attributes) {
    if (p.second == L"")
      stream << p.first << L" ";
    else
      stream << p.first << L"=" << p.second << L" ";
  }
  stream << endl;
  for (auto &n : s.children)
    stream << n.get();
  return stream;
}


wostream& operator<<(wostream& stream, const Node* s) {
  PrintVisitor v(stream);
  s->accept(v);
  return stream;
}

ToStringVisitor::ToStringVisitor(bool content, bool children)
  : content(content), children(children)
{}

const void ToStringVisitor::visit(const Htmlnode *n) {
  if (!content) {
    result += L"<" + n->tag_name + L" ";
    for (const auto& attr : n->attributes)
      result += attr.first + L"=" + attr.second + L" ";
    result += L">";
  }

  for (const auto& child : n->children)
    child->accept(*this);

  if (!content) {
    result += L"</" + n->tag_name + L">";
  }
}

const void ToStringVisitor::visit(const Emptyhtmlnode *n) {
  if (children) {
    result += L"<" + n->tag_name + L" ";
    for (const auto& attr : n->attributes)
      result += attr.first + L"=" + attr.second + L" ";
    result += L"/>";
  }
}

const void ToStringVisitor::visit(const Textnode *n) {
  if (content)
    result += n->content;
}

const wstring& ToStringVisitor::getResult() {
  return result;
}


wostream& operator<<(wostream& stream, const Htmlstart& n) {
  for (auto &i : n.nodes)
    stream << i.get();
  return stream;
}


PrintVisitor::PrintVisitor(wostream& stream)
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
