#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include <string>

#include "htmllexer.h"
#include "htmlparser.h"
#include "querylexer.h"
#include "queryparser.h"

using namespace std;

class Extractor {
  public:
    Extractor(const string& query, const string& html);
    list<string> getResult();

  private:
    void run();
    void match(const Node*, int, list<string>);
    qstart querytree;
    Htmlstart htmltree;
    bool wasCalculated;
    list<string> result;
};

enum class node_t {
  HTMLNODE,
  EMPTYHTMLNODE,
  TEXTNODE
};

/**
 * Visitor class validating if given html node
 * matched provided subquery
 */
class MatchVisitor : public Visitor {
  public:
    MatchVisitor(qnode node); // node to match
    const void visit(const Htmlnode *n);
    const void visit(const Emptyhtmlnode *n);
    const void visit(const Textnode *n);
    map<string, string> getMatchedUnknownAttributes();
    bool matchSucceded();
    void clear();
    const list<Node*>& getChildren();

  private:
    qnode pattern;
    bool success;
    map<string, string> matched_unknown_attributes;
    node_t nodeType;
    list<Node*> children;
};

#endif // _EXTRACTOR_H_
