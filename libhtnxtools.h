#ifndef LIBHTNXTOOLS_LIBHTNXTOOLS_H_
#define LIBHTNXTOOLS_LIBHTNXTOOLS_H_

#include <vector>
#include <memory>
#include <unordered_map>


// 界门纲目科属种前缀
static const std::unordered_map<std::string, std::string> kRanks = {
    {"kingdom", "k__"},
    {"phylum", "p__"},
    {"class", "c__"},
    {"order", "o__"},
    {"family", "f__"},
    {"genus", "g__"},
    {"species", "s__"}
};


// taxdmp相关
static const std::string kRootId = "1";
static const std::string kDelimiter = "\t|\t";
static const std::string kScientificName = "scientific name";


void Split(const std::string&, const std::string&, std::vector<std::string>&);


class Names {
public:
    Names() = default;
    explicit Names(const std::string&);
    const std::string& Get(const std::string&) const;
private:
    static bool IsScientificName(const std::vector<std::string>&);
    std::unordered_map<std::string, std::string> data_;
};


class Node {
public:
    Node();
    Node(const std::string&, const std::string&, const std::string&);
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    const std::string& id() const;
    const std::string& name() const;
    const std::string& rank() const;
    const std::vector<Node*>& children() const;
    void parent(Node*);
    void AddChild(Node*);
    bool IsRoot() const;
    bool IsLeaf() const;
    bool IsStandard() const;
    void Lineage(std::vector<const Node*>&) const;
    ~Node();
private:
    std::string id_;
    std::string name_;
    std::string rank_;
    Node* parent_;
    std::vector<Node*> children_;
};


class Nodes {
public:
    Nodes() : root_(nullptr) {}
    Nodes(const std::string&, const Names&);
    Nodes(const Nodes&) = delete;
    Nodes& operator=(const Nodes&) = delete;
    Node* Get(const std::string&);
    ~Nodes();
private:
    Node* root_;
    std::unordered_map<std::string, Node*> data_;
};


void WriteNode(const Node*, std::ostream&);

void WriteTree(const Node*, std::ostream&);

#endif //LIBHTNXTOOLS_LIBHTNXTOOLS_H_
