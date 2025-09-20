#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <sstream> 
#include "../include/ast.hpp"
#include "koopa.h"

using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
// 看起来会很烦人, 于是干脆采用这种看起来 dirty 但实际很有效的手段
extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

// 全局保存 IR 字符串，保证 c_str() 在整个程序需要期间有效
static std::string g_ir_string;
static const char *g_ir_cstr = nullptr; //这个在

int main(int argc, const char *argv[]) {
  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);


  // parse input file
unique_ptr<BaseAST> ast;
auto ret = yyparse(ast);
assert(!ret);
 assert(ast); // 确保解析得到 AST
// dump AST
// ===== 用 ostringstream 捕获 IR 输出 =====
  std::ostringstream oss;
  ast->IR(oss);               // 注意：IR 函数现在签名为 IR(std::ostream& out)
  g_ir_string = oss.str();    // 保存到全局 string（保证长期有效）
  g_ir_cstr = g_ir_string.c_str(); // 安全的 const char*

  // 示例：打印捕获到的 IR
  std::cout << "Generated IR:\n" << g_ir_string << std::endl;

  // 传给 libkoopa


// 解析字符串 str, 得到 Koopa IR 程序
koopa_program_t program;
koopa_error_code_t ret1 = koopa_parse_from_string(g_ir_cstr, &program);
assert(ret1 == KOOPA_EC_SUCCESS);  // 确保解析时没有出错
// 创建一个 raw program builder, 用来构建 raw program
koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
// 将 Koopa IR 程序转换为 raw program
koopa_raw_program_t raw = koopa_build_raw_program(builder, program);

//遍历函数列表

// 使用 for 循环遍历函数列表
for (size_t i = 0; i < raw.funcs.len; ++i) {
  // 正常情况下, 列表中的元素就是函数, 我们只不过是在确认这个事实
  // 当然, 你也可以基于 raw slice 的 kind, 实现一个通用的处理函数
  assert(raw.funcs.kind == KOOPA_RSIK_FUNCTION);
  // 获取当前函数
  koopa_raw_function_t func = (koopa_raw_function_t) raw.funcs.buffer[i];
  // 进一步处理当前函数
  // ...
      for (size_t j = 0; j < func->bbs.len; ++j) {
      assert(func->bbs.kind == KOOPA_RSIK_BASIC_BLOCK);
      koopa_raw_basic_block_t bb = (koopa_raw_basic_block_t) func->bbs.buffer[j];
      // 进一步处理当前基本块
                  koopa_raw_value_t value = ...;
            // 示例程序中, 你得到的 value 一定是一条 return 指令
            assert(value->kind.tag == KOOPA_RVT_RETURN);
            // 于是我们可以按照处理 return 指令的方式处理这个 value
            // return 指令中, value 代表返回值
            koopa_raw_value_t ret_value = value->kind.data.ret.value;
            // 示例程序中, ret_value 一定是一个 integer
            assert(ret_value->kind.tag == KOOPA_RVT_INTEGER);
            // 于是我们可以按照处理 integer 的方式处理 ret_value
            // integer 中, value 代表整数的数值
            int32_t int_val = ret_value->kind.data.integer.value;
            // 示例程序中, 这个数值一定是 0
            assert(int_val == 0);
      // ...
    }

}




// 释放 Koopa IR 程序占用的内存
koopa_delete_program(program);

// 处理 raw program
// ...

// 处理完成, 释放 raw program builder 占用的内存
// 注意, raw program 中所有的指针指向的内存均为 raw program builder 的内存
// 所以不要在 raw program 处理完毕之前释放 builder
koopa_delete_raw_program_builder(builder);


  // 关闭输入文件
  fclose(yyin);
  return 0;
}
