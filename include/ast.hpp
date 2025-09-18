#pragma once
#include <iostream>
#include <memory>
#include <string>

/*

由EBNF实现AST：
CompUnit  ::= FuncDef;
FuncDef   ::= FuncType IDENT "(" ")" Block;
FuncType  ::= "int";
Block     ::= "{" Stmt "}";
Stmt      ::= "return" Number ";";
Number    ::= INT_CONST;

*/

/*

# fun @main(): i32 {
# %entry:
#   ret 0
# }

*/

// 所有 AST 的基类
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual void Dump() const = 0;
  virtual void IR() const = 0;
};

// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
 public:
  // 用智能指针管理对象
  std::unique_ptr<BaseAST> func_def;

   void Dump() const override {
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
  }
    virtual void IR() const override{
        func_def->IR();
    }
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

    void Dump() const override {
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
    }
    void IR()const override{
        std::cout<<"fun @"<<ident<<"(): ";
        func_type->IR();
        block->IR();
    }
};

class FuncTypeAST : public BaseAST{
    public:
    std::string type;
    FuncTypeAST(std::string t) : type(std::move(t)) {}

    void Dump() const override{
        std::cout<<"FuncTypeAST { ";
        std::cout<<type<<" }";
    }
    void IR() const override{
        std::cout<<"i32";
    }
};

class NumberAST:public BaseAST{
    public:
    int value;
    NumberAST(int v) : value(v) {}

    void Dump() const override{
        std::cout<<"NumberAST : ";
        std::cout<<value;
    }
    void IR() const override{
        std::cout<<value;
    }
};

class StmtAST :public BaseAST{
    public:
    ~StmtAST() override = default;

    void Dump() const override{
        std::cout<<" StmtAST "; 
    }
};

class ReturnStmtAST :public StmtAST{
    public:
    std::unique_ptr<BaseAST> value;

    void Dump() const override{
        std::cout<<" Return ";
        value->Dump();
    }
    void IR() const override{
        std::cout<<"    ret ";
        value->IR();
        std::cout<<std::endl;
    }
};

class BlockAST: public BaseAST{
    public:
    std::unique_ptr<BaseAST> stmt;

    void Dump() const override{
        std::cout<<"BlockAST:{";
        stmt->Dump();
        std::cout<<"}";
    }
    void IR() const override{
        std::cout<<" {"<<std::endl;
        std::cout<<"%entry:"<<std::endl;
        stmt->IR();
        std::cout<<"}";
    }
};
// ...
