#include "extractor.h"

Extractor::Extractor(const wstring& query, const wstring& html)
  : wasCalculated(0) {
  // make sure we have parsed query and html
  result.clear();
  try {
    QueryLexer qlexer(make_unique<StringSource>(query));
    QueryParser qparser(qlexer);
    HtmlLexer hlexer(make_unique<StringSource>(html));
    HtmlParser hparser(hlexer);
    qparser.parse(&querytree);
    hparser.parse(&htmltree);
  } catch (QueryParseException e) {
    throw runtime_error(e.what());
  } catch (HtmlParseException e) {
    throw runtime_error(e.what());
  }
}

Extractor::Extractor(const string& queryfile, const string& htmlfile)
  : wasCalculated(0) {
  result.clear();
  try {
    QueryLexer qlexer(make_unique<FileSource>(queryfile));
    QueryParser qparser(qlexer);
    HtmlLexer hlexer(make_unique<FileSource>(htmlfile));
    HtmlParser hparser(hlexer);
    qparser.parse(&querytree);
    hparser.parse(&htmltree);
  } catch (QueryParseException e) {
    throw runtime_error(e.what());
  } catch (HtmlParseException e) {
    throw runtime_error(e.what());
  }
}

list<wstring> Extractor::getResult() {
  // if result is already calculated - return it
  if (wasCalculated)
    return result;
  else
    wasCalculated = 1;

  run();

  return result;
}

const list<Node*>& MatchVisitor::getChildren() {
  return children;
}

void Extractor::run() {
  for_each(htmltree.nodes.begin(), htmltree.nodes.end(),
      [this](const unique_ptr<Node>& n) { traverseAndMatch(n.get()); });
}


/**
 * Traverses HTML, find Htmlnodes
 * and applies match to each
 */
void Extractor::traverseAndMatch(const Node* node) {
  TraverseVisitor v([this](const Node* n) { this->match(n, 0, {}); });
  node->accept(v);
}

/**
 * Verifies if Node matched query, written in recursive style
 */
void Extractor::match(const Node* node, int depth, list<wstring> out) {
  if (depth < querytree.children.size()) {
    MatchVisitor visitor(querytree.children[depth]);
    node->accept(visitor);
    if (visitor.matchSucceded()) {
      // push unknown attributes to out
      auto attrs = visitor.getMatchedUnknownAttributes();
      for (const auto& attr : attrs)
        out.push_back(attr.second);

      wstring tmp;
      if (depth+1 == querytree.children.size()) { // final node match succeded
        // push content depending on querytype
        switch (querytree.questionType) {
          case QuestionType::CONTENT_ONLY:
            tmp = contentString(node);
            if (!tmp.empty())
              out.push_back(tmp);
            break;
          case QuestionType::CHILDREN_ONLY:
            tmp = childrenString(node);
            if (!tmp.empty())
              out.push_back(tmp);
            break;
          case QuestionType::EVERYTHING:
            tmp = allString(node);
            if (!tmp.empty())
              out.push_back(tmp);
            break;
          default:
            break;
        }
        // push results to Extractor::result
        result.insert(result.end(), out.begin(), out.end());
      } else
        // recursion call
        for (const auto& child : visitor.getChildren())
          match(child, depth+1, out);
    }
  }
}


MatchVisitor::MatchVisitor(qnode node)
  : pattern(node), success(0)
{}

void MatchVisitor::clear() {
  matched_unknown_attributes.clear();
  success = 0;
}

bool MatchVisitor::matchSucceded() {
  return success;
}

map<wstring, wstring> MatchVisitor::getMatchedUnknownAttributes() {
  return matched_unknown_attributes;
}

const void MatchVisitor::visit(const Htmlnode *n) {
  for_each(n->children.begin(), n->children.end(),
      [this](const unique_ptr<Node>& node) { children.push_back(node.get()); });
  success = 1;
  // check if tag name valid
  if (pattern.tagNameKnown && n->tag_name != pattern.tagname) {
    success = 0;
    return;
  }

  // check if unknown attributes can be found
  for (const wstring& attrname : pattern.unknown_attributes) {
    const auto it = n->attributes.find(attrname);
    if (it != n->attributes.end())
      // and remember what was the result of match
      matched_unknown_attributes[attrname] = it->second;
    else {
      success = 0;
      return;
    }
  }

  // check if all required attributes can be found
  for (const auto& patternit : pattern.read_attributes) {
    const auto it = n->attributes.find(patternit.first);
    // "!" represents that attribute must exist (ignoring its content)
    if (it != n->attributes.end()) {
      if (patternit.second != L"!" && patternit.second != it->second) {
        success = 0;
        return;
      }
    } else {
      success = 0;
      return;
    }
  }

}

const void MatchVisitor::visit(const Emptyhtmlnode *n) {
  // shameless copypaste
  success = 1;
  // check if tag name valid
  if (pattern.tagNameKnown && n->tag_name != pattern.tagname) {
    success = 0;
    return;
  }

  // check if unknown attributes can be found
  for (const wstring& attrname : pattern.unknown_attributes) {
    const auto it = n->attributes.find(attrname);
    if (it != n->attributes.end())
      // and remember what was the result of match
      matched_unknown_attributes[attrname] = it->second;
    else {
      success = 0;
      return;
    }
  }

  // check if all required attributes can be found
  for (const auto& patternit : pattern.read_attributes) {
    const auto it = n->attributes.find(patternit.first);
    // "!" represents that attribute must exist (ignoring its content)
    if (it != n->attributes.end()) {
      if (patternit.second != L"!" && patternit.second != it->second) {
        success = 0;
        return;
      }
    } else {
      success = 0;
      return;
    }
  }

}

const void MatchVisitor::visit(const Textnode *n) {
  // text node doesn't match anything in hierarchy
  success = 0;
}

TraverseVisitor::TraverseVisitor(function<void(const Node*)> func)
  : f(func)
{}

const void TraverseVisitor::visit(const Htmlnode *n) {
  f(n);

  for (const auto &child : n->children)
    child->accept(*this);
}

const void TraverseVisitor::visit(const Emptyhtmlnode *n) {
  f(n);
}

const void TraverseVisitor::visit(const Textnode *n) {
  f(n);
}

list<wstring> findInfo(wstring htmlCode, wstring schema) {
  Extractor e(schema, htmlCode);
  return e.getResult();
}
