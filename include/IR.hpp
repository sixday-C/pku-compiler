#pragma once
extern "C" {
#include "koopa.h"
}


// 声明重载的 Visit（不同参数类型）
void Visit(const koopa_raw_program_t &program);
void Visit(const koopa_raw_slice_t &slice);
void Visit(const koopa_raw_function_t func);
void Visit(const koopa_raw_basic_block_t bb);
void Visit(const koopa_raw_value_t value);
