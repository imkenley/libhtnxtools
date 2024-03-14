# -*- coding: utf-8 -*-


# 界门纲目科属种前缀
RANKS = {
    'kingdom': 'k__',
    'phylum': 'p__',
    'class': 'c__',
    'order': 'o__',
    'family': 'f__',
    'genus': 'g__',
    'species': 's__'
}


# taxdmp相关
ROOT_ID = '1'
DELIMITER = '\t|\t'
SCIENTIFIC_NAME = 'scientific name'


class Names:
    def __init__(self):
        self.data = {}

    @staticmethod
    def is_scientific_name(row):
        return row[3] == SCIENTIFIC_NAME

    @classmethod
    def from_file(cls, file):
        names = cls()
        with open(file) as fp:
            for line in fp:
                row = line[: -3].split(DELIMITER)
                if cls.is_scientific_name(row):
                    names.data[row[0]] = row[1].replace(' ', '_')
        return names

    def get(self, id):  # noqa
        return self.data.get(id)


class Node:
    def __init__(self, id, name, rank):  # noqa
        self.id = id
        self.name = name
        self.rank = rank
        self.parent = None
        self.children = []

    def add_child(self, child):
        self.children.append(child)

    def is_root(self):
        return self.parent is None

    def is_leaf(self):
        return len(self.children) == 0

    def is_standard(self):
        return self.rank in RANKS

    def lineage(self, lineage):
        if self.is_standard():
            lineage.append(self)
        if not self.is_root():
            self.parent.lineage(lineage)


class Nodes:
    def __init__(self):
        self.root = None
        self.data = {}

    def get(self, id):  # noqa
        return self.data.get(id)

    def add(self, id, node):  # noqa
        self.data[id] = node

    @classmethod
    def from_file(cls, file, names):
        nodes = cls()
        id2parent = {}
        with open(file) as fp:
            for line in fp:
                row = line.split(DELIMITER)
                id2parent[row[0]] = row[1]
                nodes.add(row[0], Node(row[0], names.get(row[0]), row[2]))
        for id, node in nodes.data.items():  # noqa
            if id == ROOT_ID:
                continue
            parent_id = id2parent[id]
            parent_node = nodes.get(parent_id)
            parent_node.add_child(node)
            node.parent = parent_node
        return nodes


def write_node(node, out):
    lineage = []
    node.lineage(lineage)
    lineage.reverse()
    out.write(node.id)
    out.write('\t')
    for i, parent in enumerate(lineage):
        if not parent.is_standard():
            continue
        out.write(RANKS[parent.rank])
        out.write(parent.name)
        if i != len(lineage) - 1:
            out.write('; ')


def write_tree(node, out):
    write_node(node, out)
    out.write('\n')
    for child in node.children:
        write_tree(child, out)


def main():
    print('开始读取所有阶元的名称...')
    names = Names.from_file('data/taxdump/names.dmp')

    print('开始读取阶元信息以及各阶元间的从属信息...')
    nodes = Nodes.from_file('data/taxdump/nodes.dmp', names)

    print('获取哺乳动物的所有子阶元并输出到文件...')
    with open('output/mammalia.txt', 'w') as fp:
        write_tree(nodes.get('40674'), fp)


if __name__ == '__main__':
    main()
