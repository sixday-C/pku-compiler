# 做实验的心态:
1.mark不懂的问题.没有log=没有发生.
2.不要卡在一个地方,先进行最小实现.

# To do:

- [x] 改IR结构
- [x] 查看wdlin,YOOkoishi的pku-minic

# 文件结构:
## 头文件include
    ast.hpp                 //ast数据结构
    ir.hpp                  //ir数据结构
    ir_gen.hpp        //ast->ir
    rv_gen.hpp     //ir->rv

## 源文件src
    ast.cpp                 //ast   成员函数实现
    ir_gen.cpp        //ast->ir
    riscv_gen.cpp     //ir->rv

## 输出测试文件
    test_ast.cpp
    test_ir.cpp
    test_riscv.cpp


## 编译main
    main.cpp
    

    