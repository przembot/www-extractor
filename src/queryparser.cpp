#include "queryparser.h"

const bool qstart::operator== (const qstart& rhs) const {
  if (questionType == rhs.questionType
    && children.size() == rhs.children.size()) {
    auto it1 = children.begin();
    auto it2 = rhs.children.begin();
    for (; it1 != children.end() && it2 != rhs.children.end();
           ++it1, ++it2)
      if (!(*it1 == *it2))
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


wostream& operator<<(wostream& stream, const qstart& s) {
  stream << s.questionType << endl;
  for (const auto e : s.children) {
    stream << e << endl;
  }
  return stream;
}


wostream& operator<<(wostream& stream, const qnode& n) {
  stream << n.tagname << L" " << n.tagNameKnown << endl;
  for (const auto &a : n.read_attributes)
    stream << a.first << L" " << a.second << L" ";
  stream << endl;
  for (const auto &a : n.unknown_attributes)
    stream << a << L" ";
  return stream;
}

wostream& operator<<(wostream& stream, const QuestionType& q) {
  static const map<QuestionType, wstring> dict = {
    {QuestionType::CONTENT_ONLY, L"content only"},
    {QuestionType::CHILDREN_ONLY, L"children only"},
    {QuestionType::EVERYTHING, L"everything"}
  };
  auto it = dict.find(q); // type-safety, it will be always found as dict has all elements
  stream << it->second;
  return stream;
}

void QueryParser::accept(const SymType& stype) {
  if (symbol.first != stype)
    throw QueryParseException("nieoczekiwany atom");
}


void QueryParser::accept(const SymSet& sset) {
  if (sset.find(symbol.first) == sset.end())
    throw QueryParseException("nieoczekiwany atom");
}


void QueryParser::acceptNext(const SymType& stype) {
  nextSymbol();
  if (symbol.first != stype)
    throw QueryParseException("nieoczekiwany atom");
}


void QueryParser::acceptNext(const SymSet& sset) {
  nextSymbol();
  if (sset.find(symbol.first) == sset.end())
    throw QueryParseException("nieoczekiwany atom");
}


void QueryParser::nextSymbol() {
  symbol = lexer.nextSymbol();
}


QueryParser::QueryParser(QueryLexer &inlexer)
  : lexer(inlexer) {
}


void QueryParser::parse(qstart* tree) {
  // inicjacja drzewa
  if (tree == nullptr)
    throw QueryParseException("tree uninitialized");
  result = tree;
  buffor.clear();

  parseStart();
}

void QueryParser::parseStart() {
  acceptNext(slashtk);

  nextSymbol();
  parseExpressions();
  parseQuery();
}

void QueryParser::parseQuery() {
  int counter = 1;
  while (symbol.first == slashtk) {
    ++counter;
    nextSymbol();
  }
  switch (counter) {
    case 1:
      result->questionType = QuestionType::CONTENT_ONLY;
      break;
    case 2:
      result->questionType = QuestionType::CHILDREN_ONLY;
      break;
    case 3:
      result->questionType = QuestionType::EVERYTHING;
      break;
    default:
      throw QueryParseException("unknown question type");
      break;
  }
}

void QueryParser::parseExpressions() {
  while (tryParseExpression());
}

bool QueryParser::tryParseExpression() {
  if (symbol.first == stringtk || symbol.first == anytagtk) {
    //nodebuilder = new qnode();
    nodebuilder.read_attributes.clear();
    nodebuilder.unknown_attributes.clear();
    nodebuilder.tagNameKnown = symbol.first==stringtk;
    if (symbol.first == stringtk) {
      nodebuilder.tagNameKnown = 1;
      nodebuilder.tagname = symbol.second;
    } else
      nodebuilder.tagNameKnown = 0;


    nextSymbol();
    parseAttributes();

    accept(slashtk);
    nextSymbol();

    result->children.push_back(nodebuilder);

    return true;
  }
  return false;
}

void QueryParser::parseAttributes() {
  while (tryParseAttribute());
}

bool QueryParser::tryParseAttribute() {
  if (symbol.first == stringtk) {
    wstring attrname = symbol.second;
    acceptNext(equalstk);
    acceptNext({mustexisttk, attrquerytk, singlequotevaltk, doublequotevaltk, stringtk});

    switch (symbol.first) {
      case mustexisttk:
        nodebuilder.read_attributes[attrname] = L"!";
        break;
      case attrquerytk:
        nodebuilder.unknown_attributes.push_back(attrname);
        break;
      default: // string val
        nodebuilder.read_attributes[attrname] = symbol.second;
        break;
    }

    nextSymbol();
    return true;
  }
  return false;
}
