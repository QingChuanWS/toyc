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

#ifndef NODE_GRUAD
#define NODE_GRUAD

#include "token.h"
#include "tools.h"

enum NodeKind {
  ND_ADD,         // +
  ND_SUB,         // -
  ND_MUL,         // *
  ND_DIV,         // /
  ND_EQ,          // ==
  ND_NE,          // !=
  ND_LT,          // <
  ND_LE,          // <=
  ND_NUM,         // number
  ND_RETURN,      // return
  ND_EXPR_STMT,   // expression statement
  ND_END,
};

class Node {
 public:
  Node(NodeKind kind = ND_END)
      : kind_(kind)
      , next(nullptr)
      , lhs_(nullptr)
      , rhs_(nullptr)
      , val_(0) {}
  Node(NodeKind kind, Node* lhs, Node* rhs = nullptr)
      : kind_(kind)
      , next(nullptr)
      , lhs_(lhs)
      , rhs_(rhs)
      , val_(0) {}
  Node(int val)
      : kind_(ND_NUM)
      , next(nullptr)
      , lhs_(nullptr)
      , rhs_(nullptr)
      , val_(val) {}

  // post-order for AST delete
  static void NodeFree(Node* node);
  // parsing token list and generate AST.
  // program = stmt*
  static Node* Program(Token** tok);

 private:
  // stmt = "return" expr ";" | expr ";"
  static Node* Stmt(Token** tok);
  // expr = equality
  static Node* Expr(Token** tok);
  // equality = relational ("==" relational | "!=" relational)
  static Node* Equality(Token** tok);
  // relational = add ("<" add | "<=" add | ">" add | ">=" add)
  static Node* Relational(Token** tok);
  // add = mul ("+"mul | "-" mul)
  static Node* Add(Token** tok);
  // mul = unary ("*" unary | "/" unary)
  static Node* Mul(Token** tok);
  // unary = ("+" | "-") ? unary | primary
  static Node* Unary(Token** tok);
  // primary = "(" expr ")" | num
  static Node* Primary(Token** tok);

  friend class CodeGenerator;

  NodeKind kind_;
  Node*    next;
  Node*    lhs_;
  Node*    rhs_;
  long     val_;
};

#endif   // !NODE_GRUAD
