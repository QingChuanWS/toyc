/*
 * This project is exclusively owned by QingChuanWS and shall not be used for
 * commercial and profitting purpose without QingChuanWS's permission.
 *
 * @Author: bingshan45@163.com
 * Github: https://github.com/QingChuanWS
 * @Description:
 *
 * Copyright (c) 2022 by QingChuanWS, All Rights Reserved.
 */

#include "codegen.h"
#include "function.h"
#include "node.h"
#include "token.h"
#include "tools.h"

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "%s: invalid number of arguments\n", argv[0]);
    return 1;
  }

  Token     head = Token();
  Token*    cur  = Token::TokenCreate(head, argv[1]);
  Function* prog = Function::Parse(cur);

  CodeGenerator gene;
  gene.CodeGen(prog);

  Token::TokenFree(head);
  Function::FunctionFree(prog);

  return 0;
}
