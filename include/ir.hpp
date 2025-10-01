#pragma once
#include <memory>
#include <vector>
#include <string>
#include <ostream>

// —— 基类：Value（所有指令/值）——
class Value {
public:
  virtual ~Value() = default;
  virtual void dump(std::ostream& os) const = 0;
};

// —— 指令/值：整数字面量 ——
// 表示 Koopa 文本里的 “42”
class ConstInt : public Value {
public:
  int v;
  explicit ConstInt(int x) : v(x) {}
  void dump(std::ostream& os) const override { os << v; }
};

// —— 指令：return ——
// 表示 Koopa 文本里的 “ret <value>”
class Return : public Value {
public:
  std::unique_ptr<Value> val;   // 要返回的那个值
  explicit Return(std::unique_ptr<Value> v) : val(std::move(v)) {}
  void dump(std::ostream& os) const override {
    os << "ret";
    if (val) { os << " "; val->dump(os); }
  }
};

// —— 基本块 ——
// 打印成：%entry:
class BasicBlock {
public:
  std::string name;                              // 约定：不带 %
  std::vector<std::unique_ptr<Value>> insts;     // 指令列表（线性序）

  explicit BasicBlock(std::string n) : name(std::move(n)) {}
  //add instruction to basic block
  void add(std::unique_ptr<Value> v) { insts.push_back(std::move(v)); }
  void dump(std::ostream& os) const {
    os << "%" << name << ":\n";
    for (const auto& ins : insts) { os << "  "; ins->dump(os); os << "\n"; }
  }
};

// —— 函数 ——
// 打印成：fun @main() { ... }
class Function {
public:
  std::string name;
  std::vector<std::unique_ptr<BasicBlock>> blocks;

  explicit Function(std::string n) : name(std::move(n)) {}
  //add basic block to function
  void add(std::unique_ptr<BasicBlock> bb) { blocks.push_back(std::move(bb)); }
  void dump(std::ostream& os) const {
    os << "fun @" << name << "(): i32 {\n";
    for (const auto& bb : blocks) bb->dump(os);
    os << "}\n";
  }
};

// —— 程序 ——
// 由若干函数构成
class Program {
public:
  std::vector<std::unique_ptr<Function>> funcs;
//add function to program
  void add(std::unique_ptr<Function> f) { funcs.push_back(std::move(f)); }
  void dump(std::ostream& os) const {
    for (const auto& f : funcs) f->dump(os);
  }
};