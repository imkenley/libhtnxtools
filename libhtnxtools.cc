#include <vector>
#include <fstream>
#include <algorithm>

#include <fmt/core.h>

#include "libhtnxtools.h"



void Split(const std::string& str, const std::string& delimiter, std::vector<std::string>& result) {
    std::string::size_type start = 0, end = str.find(delimiter), len = delimiter.size();
    while (end != std::string::npos) {
        result.emplace_back(str.substr(start, end-start));
        start = end + len;
        end = str.find(delimiter, start);
        if (end == std::string::npos) {
            result.push_back(str.substr(start, str.size()));
        }
    }
}


bool Names::IsScientificName(const std::vector<std::string>& row) {
    return row[3] == kScientificName;
}

Names::Names(const std::string& names_dmp) {
    std::ifstream in(names_dmp);
    if (!in) {
        throw std::runtime_error(fmt::format("can't open file {}", names_dmp));
    }

    std::string line;
    std::vector<std::string> row;
    while (std::getline(in, line)) {
        line = line.substr(0, line.size()-2);
        Split(line, kDelimiter, row);
        std::string name = row[1];
        std::replace(name.begin(), name.end(), ' ', '_');
        if (IsScientificName(row)) {
            data_.emplace(row[0], name);
        }
        row.clear();
    }
}

const std::string& Names::Get(const std::string& id) const {
    if (data_.find(id) == data_.end()) {
        throw std::runtime_error(fmt::format("can't find the name of {} in names.dmp file", id));
    }
    return data_.at(id);
}


Node::Node() : parent_(nullptr) {}

Node::Node(const std::string& id, const std::string& name, const std::string& rank)
    : id_(id), name_(name), rank_(rank), parent_(nullptr) {}
void Node::parent(Node* parent) {
    parent_ = parent;
}

const std::string& Node::id() const {
    return id_;
}

const std::string& Node::name() const {
    return name_;
}

const std::string& Node::rank() const {
    return rank_;
}

const std::vector<Node*>& Node::children() const {
    return children_;
}

void Node::AddChild(Node* child) {
    children_.push_back(child);
}

bool Node::IsRoot() const {
    return parent_ == nullptr;
}

bool Node::IsLeaf() const {
    return children_.empty();
}

bool Node::IsStandard() const {
    return kRanks.find(rank_) != kRanks.end();
}

void Node::Lineage(std::vector<const Node*>& lineage) const {
    if (IsStandard()) {
        lineage.push_back(this);
    }
    if (! IsRoot()) {
        parent_->Lineage(lineage);
    }
}

Node::~Node() {
    for (auto child: children_) {
        delete child;
    }
}


Nodes::Nodes(const std::string& nodes_dmp, const Names& names) {
    std::ifstream in(nodes_dmp);
    if (! in) {
        throw std::runtime_error(fmt::format("can't open file {}", nodes_dmp));
    }
    std::string line;
    std::vector<std::string> row;
    std::unordered_map<std::string, std::string> id2parent;
    while (std::getline(in, line)) {
        Split(line, kDelimiter, row);
        id2parent.emplace(row[0], row[1]);
        Node* node = new Node(row[0], names.Get(row[0]), row[2]);
        data_.emplace(row[0], node);
        row.clear();
    }
    for (auto& pair : data_) {
        if (pair.first != kRootId) {
            Node* parent_node = Get(id2parent.at(pair.first));
            parent_node->AddChild(pair.second);
            pair.second->parent(parent_node);
        }
    }
    root_ = Get(kRootId);
}

Node* Nodes::Get(const std::string& id) {
    return data_.at(id);
}

Nodes::~Nodes() {
    delete root_;
}


void WriteNode(const Node* node, std::ostream& os) {
    std::vector<const Node*> lineage;
    node->Lineage(lineage);
    std::reverse(lineage.begin(), lineage.end());
    os << node->id() << "\t";
    for (int i=0; i!=lineage.size(); ++i) {
        const Node* parent = lineage[i];
        if (! parent->IsStandard()) {
            continue;
        }
        os << kRanks.at(parent->rank()) << parent->name();
        if (i != lineage.size() - 1) {
            os  << "; ";
        }
    }
}

void WriteTree(const Node* node, std::ostream& os) {
    WriteNode(node, os);
    os << "\n";
    for (Node* child : node->children()) {
        WriteTree(child, os);
    }
}
