#pragma once
#include <memory>
#include "ast.hpp"  // 你上面这份 AST 头
#include "ir.hpp"   // 你上面这份 IR 头

class IRGenerator {
public:
  std::unique_ptr<Program> Generate(const CompUnitAST& ast);
//接收ast指针,返回program指针
private:
  // 非拥有的“当前写入位置”
  Function*   current_func  = nullptr;
  BasicBlock* current_block = nullptr;

  // 拥有 Program
  std::unique_ptr<Program> program;

  // 逐层访问（只写到你现在需要的）
  void visit(const CompUnitAST& n);
  void visit(const FuncDefAST& n);
  void visit(const BlockAST& n);
  void visit(const StmtAST& n);
  void visit(const ReturnStmtAST& n);

  // 表达式→Value（现在只支持 Number）
  std::unique_ptr<Value> emit(const BaseAST& n);
};