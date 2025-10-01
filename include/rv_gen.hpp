#pragma once
#include <ostream>
#include "ir.hpp"  // 你的 IR 头文件（Program/Function/BasicBlock/Value 等）

// 极简 RISC-V 代码生成器：把 IR 遍历成 RISC-V 汇编
class RvGen {
public:
  explicit RvGen(std::ostream& os) : os_(os) {}
  void generate(const Program& p);

private:
  std::ostream& os_;

  void gen_function(const Function& f);
  void gen_block(const BasicBlock& bb);

  // 把一个 Value 计算/放入 a0（当前只支持 ConstInt）
  void eval_to_a0(const Value& v);
};