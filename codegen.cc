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

#include "node.h"
#include "tools.h"

static int Count() {
  static int count = 0;
  return count++;
}

int CodeGenerator::depth = 0;

#define ASM_GEN(...) AsmPrint(__VA_ARGS__)

// recursive over state.
template<typename T>
static void AsmPrint(const T arg) {
  std::cout << arg << std::endl;
}

template<typename T, typename... Types>
static void AsmPrint(const T first_arg, const Types... args) {
  std::cout << first_arg;
  AsmPrint(args...);
}

void CodeGenerator::GetVarAddr(Node* node) {
  if (node->kind_ == ND_VAR) {
    ASM_GEN("  lea rax, [rbp - ", node->var_->offset_, "]");
    return;
  }

  node->tok_->ErrorTok("not an lvalue");
}

void CodeGenerator::Push() {
  ASM_GEN("  push rax");
  depth++;
}

void CodeGenerator::Pop(const char* arg) {
  ASM_GEN("  pop ", arg);
  depth--;
}

void CodeGenerator::CodeGen(Function* prog) {
  prog->OffsetCal();

  // using intel syntax
  // e.g. op dst, src
  ASM_GEN("  .intel_syntax noprefix");
  ASM_GEN("  .global main");
  ASM_GEN("main:");

  // prologue; equally instruction "enter 0xD0,0".
  ASM_GEN("  push rbp");
  ASM_GEN("  mov rbp, rsp");
  ASM_GEN("  sub rsp, ", prog->stack_size_);

  StmtGen(prog->node_);
  DEBUG(depth == 0);

  // Epilogue; equally instruction leave.
  ASM_GEN(".L.return:");
  ASM_GEN("  mov rsp, rbp");
  ASM_GEN("  pop rbp");
  ASM_GEN("  ret");
}

void CodeGenerator::StmtGen(Node* node) {
  switch (node->kind_) {
  case ND_EXPR_STMT: ExprGen(node->lhs_); return;
  case ND_BLOCK:
    for (Node* n = node->body_; n != nullptr; n = n->next_) {
      StmtGen(n);
    }
    return;
  case ND_RETURN:
    ExprGen(node->lhs_);
    ASM_GEN("  jmp .L.return");
    return;
  case ND_IF: {
    int seq = Count();
    ExprGen(node->cond_);
    ASM_GEN("  cmp rax, 0");
    ASM_GEN("  je .L.else.", seq);
    StmtGen(node->then_);
    ASM_GEN("  jmp .L.end.", seq);
    ASM_GEN(".L.else.", seq, ":");
    if (node->els_ != nullptr) {
      StmtGen(node->els_);
    }
    ASM_GEN(".L.end.", seq, ":");
    return;
  }
  case ND_FOR: {
    int seq = Count();
    if(node->init_ != nullptr)
      StmtGen(node->init_);
    ASM_GEN(".L.begin.", seq, ":");
    if (node->cond_ != nullptr) {
      ExprGen(node->cond_);
      ASM_GEN("  cmp rax, 0");
      ASM_GEN("  je .L.end.", seq);
    }
    StmtGen(node->then_);
    if (node->inc_ != nullptr) {
      ExprGen(node->inc_);
    }
    ASM_GEN("  jmp .L.begin.", seq);
    ASM_GEN(".L.end.", seq, ":");
    return;
  }
  default: 
    node->tok_->ErrorTok("invalid statement");
  }
}

// post-order for code-gen
void CodeGenerator::ExprGen(Node* node) {
  switch (node->kind_) {
  case ND_NUM: ASM_GEN("  mov rax, ", node->val_); return;
  case ND_NEG:
    ExprGen(node->lhs_);
    ASM_GEN("  neg rax");
    return;
  case ND_VAR:
    GetVarAddr(node);
    ASM_GEN("  mov rax, [rax]");
    return;
  case ND_ASSIGN:
    GetVarAddr(node->lhs_);
    Push();
    ExprGen(node->rhs_);
    Pop("rdi");
    ASM_GEN("  mov [rdi], rax");
    return;
  default: break;
  }

  ExprGen(node->rhs_);
  Push();
  ExprGen(node->lhs_);
  Pop("rdi");

  switch (node->kind_) {
  case ND_ADD: ASM_GEN("  add rax, rdi"); return;
  case ND_SUB: ASM_GEN("  sub rax, rdi"); return;
  case ND_MUL: ASM_GEN("  imul rax, rdi"); return;
  case ND_DIV:
    ASM_GEN("  cqo");
    ASM_GEN("  idiv rdi");
    return;
  case ND_EQ:
    ASM_GEN("  cmp rax, rdi");
    ASM_GEN("  sete al");
    ASM_GEN("  movzb rax, al");
    return;
  case ND_NE:
    ASM_GEN("  cmp rax, rdi");
    ASM_GEN("  setne al");
    ASM_GEN("  movzb rax, al");
    return;
  case ND_LT:
    ASM_GEN("  cmp rax, rdi");
    ASM_GEN("  setl al");
    ASM_GEN("  movzb rax, al");
    return;
  case ND_LE:
    ASM_GEN("  cmp rax, rdi");
    ASM_GEN("  setle al");
    ASM_GEN("  movzb rax, al");
    return;
  default: node->tok_->ErrorTok("invalid expression.");
  }
}
