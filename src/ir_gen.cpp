#include "../include/ir_gen.hpp"
#include "../include/ir.hpp"
#include "../include/ast.hpp"

#include <cassert>
#include <utility>

// Generate IR from AST
std::unique_ptr<Program> IRGenerator::Generate(const CompUnitAST& ast) {
  program = std::make_unique<Program>();
  visit(ast);
  return std::move(program);  // 或 return program;
}

// CompUnit ::= FuncDef;
void IRGenerator::visit(const CompUnitAST& n) {
  auto* f = dynamic_cast<FuncDefAST*>(n.func_def.get());
  assert(f && "CompUnit.func_def must be FuncDefAST");
  visit(*f);
}

// FuncDef ::= FuncType IDENT "(" ")" Block;
void IRGenerator::visit(const FuncDefAST& n) {
  // 1) 建函数
  auto func = std::make_unique<Function>(n.ident);

  // 2) 建 entry 基本块，并设为“当前写入点”
  auto bb = std::make_unique<BasicBlock>("entry");
  current_func  = func.get();
  current_block = bb.get();

  // 3) 函数体
  auto* b = dynamic_cast<BlockAST*>(n.block.get());
  assert(b && "FuncDef.block must be BlockAST");
  visit(*b);

  // 4) 装订
  func->add(std::move(bb));
  program->add(std::move(func));
}

// Block ::= "{" Stmt "}";
void IRGenerator::visit(const BlockAST& n) {
  auto* s = dynamic_cast<StmtAST*>(n.stmt.get());
  assert(s && "Block.stmt must be StmtAST");
  visit(*s);
}

// Stmt ::= "return" Number ";";
void IRGenerator::visit(const StmtAST& n) {
  // 只有一种语句：ReturnStmtAST
  if (auto* r = dynamic_cast<const ReturnStmtAST*>(&n)) {
    visit(*r);
    return;
  }
  assert(false && "Only ReturnStmtAST supported for now");
}

// ReturnStmtAST(value)
void IRGenerator::visit(const ReturnStmtAST& n) {
  assert(current_block && "current_block not set");

  // value 是 BaseAST*，但我们现在只支持 Number
  auto v = emit(*n.value);          // -> unique_ptr<Value> (ConstInt)
  auto ret = std::make_unique<Return>(std::move(v));
  current_block->add(std::move(ret));
}

// 目前只支持 NumberAST → ConstInt
std::unique_ptr<Value> IRGenerator::emit(const BaseAST& n) {
  if (auto* num = dynamic_cast<const NumberAST*>(&n)) {
    return std::make_unique<ConstInt>(num->value);
  }
  assert(false && "Only NumberAST is supported in expressions");
  return nullptr;
}