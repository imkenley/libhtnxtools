# libhtnxtools

## 简介

前几天有个客户找到我，想要做一个`刺盘孢属`核酸序列库。我检索了一下，目前没有 太好的工具。
`沈伟（Wei Shen）`的`taxonkit`虽然可以做到，但是`taxonkit`的功能很多， 不是专门做这个的，
用来实现构建nt/nr子库功能过于繁琐。

本项目的目标是提供一个一键式解决方案，直接输出任何一个属（或者说任何一个阶元）的ID，即可提取
其所有的核酸或者蛋白序列。具体来说，可以分为3步：

（1）获取一个阶元下的所有子阶元的ID；

（2）获取多个ID下所有序列编号；

（3）根据编号提取所有序列。

## 准备

**1 下载数据**

（1）`taxdump`

NCBI Taxonomy是目前使用最广泛的物种信息数据库之一。
它由美国国家生物技术信息中心（National Center for Biotechnology Information，简称NCBI）维护和管理。
NCBI Taxonomy是一个层次化的分类系统，用于组织和命名生物学上已知的所有物种。
它使用一种称为分类阶元（taxon）的基本单位来表示各个物种。每个分类阶元都有一个唯一的标识符（TaxID），
以及与其相关的分类信息，如科、属、种等。该数据库收集了来自各种来源的物种分类信息，包括文献、研究论文、基因组项目等。
它提供了一个便于科学家和研究人员查找和访问物种信息的平台。研究人员可以使用NCBI Taxonomy来了解物种的命名、分类关系、进化关系等重要信息。

NCBI Taxonomy每天会把所有的数据库信息导出，并以纯文本文件的形式放到`https://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz`
供人下载使用。该目录下有几个文件可以用来解决上面的两个问题。

`names.dmp`提供各个阶元的名称，`nodes.dmp`提供各个阶元的分类级别和层次关系，通过这两个文件我们就可以构建一个系统发生树。

```shell
mkdir -p data/taxdump
curl -o data/taxdump.tar.gz https://ftp.ncbi.nlm.nih.gov/pub/taxonomy/taxdump.tar.gz
tar -zx -f data/taxdump.tar.gz -C data/taxdump
```

## 编译

```shell
git clone --recursive https://github.com/imkenley/libhtnxtools.git
cd libhtnxtools
mkdir build
cd build
cmake .. && make
```

## 使用

**1 提取一个阶元下的所有阶元，并输出完整谱系信息**

```shell
mkdir output
build/tree
```