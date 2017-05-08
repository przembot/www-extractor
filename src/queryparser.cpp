#include "queryparser.h"

const bool qstart::operator== (const qstart& rhs) const {
  if (questionType == rhs.questionType
    && children.size() == rhs.children.size()) {
    auto it1 = children.begin();
    auto it2 = rhs.children.begin();
    for (; it1 != children.end() && it2 != rhs.children.end();
           ++it1, ++it2)
      if (!(**it1 == **it2))
        return false;
    return true;
  }
  return false;
}

const bool qnode::operator== (const qnode& rhs) const {
  if (tagname == rhs.tagname && tagNameKnown == rhs.tagNameKnown
      && unknown_attributes.size() == rhs.unknown_attributes.size()
      && read_attributes.size() == rhs.read_attributes.size()) {
    auto i1 = unknown_attributes.begin();
    auto i2 = rhs.unknown_attributes.begin();
    for (; i1 != unknown_attributes.end() && i2 != rhs.unknown_attributes.end();
          ++i1, ++i2)
      if (*i1 != *i2)
        return false;

    auto it1 = read_attributes.begin();
    auto it2 = rhs.read_attributes.begin();
    for (; it1 != read_attributes.end() && it2 != rhs.read_attributes.end();
          ++it1, ++it2)
      if (*it1 != *it2)
        return false;

    return true;
  }
  return false;
}


ostream& operator<<(ostream& stream, const qstart& s) {
  stream << s.questionType << endl;
  for (const auto e : s.children) {
    stream << *e << endl;
  }
  return stream;
}


ostream& operator<<(ostream& stream, const qnode& n) {
  stream << n.tagname << " " << n.tagNameKnown << endl;
  for (const auto &a : n.read_attributes)
    stream << a.first << " " << a.second << " ";
  stream << endl;
  for (const auto &a : n.unknown_attributes)
    stream << a << " ";
  return stream;
}
