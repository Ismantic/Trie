# Trie

C++ 实现的高效 Trie 数据结构集合，包含三种不同的字典树实现，附详细原理文档。

## 实现

### CritbitTrie

基于二进制关键位（Critbit）压缩的二叉 Trie。通过跳过相同前缀的二进制位，只在关键分叉位建立节点，大幅减少存储开销。支持动态插入、前缀搜索和序列化。

### DoubleArrayTrie

基于 base/check 双数组的经典 Trie 压缩实现。将树结构平铺到一维数组中，适合静态字典的快速查找。

### DoubleArray（XOR）

基于 XOR 运算的双数组 Trie。利用 XOR 的自反性实现状态转移（`next = pos ^ state ^ char`），支持精确匹配、前缀搜索和扩展搜索。Header-only 模板实现。

## 构建

```bash
make          # 构建所有目标
make test     # 运行测试
make clean    # 清理
```

## 使用示例

### CritbitTrie

```cpp
#include "trie.h"

trie::CritbitTrie ct;
ct.Insert("apple");
ct.Insert("app");
ct.Insert("application");

auto results = ct.GetValues("app");       // 前缀搜索：app, apple, application
auto common = ct.GetCommonValues("apple"); // 公共前缀：apple, app
```

### DoubleArray（XOR）

```cpp
#include "double_array.h"

std::vector<std::string> words = {"app", "apple", "apply"};
std::sort(words.begin(), words.end()); // 需要排序

trie::DoubleArray<int> dat;
dat.Build(words);

auto r = dat.GetUnit("apple");                    // 精确匹配
auto prefixes = dat.PrefixSearch("application");   // 前缀搜索
auto expand = dat.FindWordsWithPrefix("app");      // 扩展搜索
```

## 文档

- [CritbitTrie 原理详解](docs/CritbitTrie.md) - Critbit 关键位压缩、节点裂变机制
- [DoubleArrayTrie 原理详解](docs/DoubleArrayTrie.md) - XOR 状态转移、双数组构建算法

## 文件结构

```
├── trie.h                 # CritbitTrie + DoubleArrayTrie 头文件
├── trie.cc                # CritbitTrie + DoubleArrayTrie 实现
├── double_array.h         # DoubleArray (XOR) 头文件（header-only）
├── example.cc             # 使用示例
├── double_array_test.cc   # DoubleArray 测试
└── docs/                  # 原理文档
```

## License

[MIT](LICENSE)
