
#include "parser.h"
#include "token.h"
#include "utils.h"
#include "node.h"

NodePtr Parser::Program(TokenPtr* rest, TokenPtr tok) {
  tok = tok->SkipToken("{");
  return CompoundStmt(rest, tok);
}

// compound-stmt  = (declaration | stmt)* "}"
NodePtr Parser::CompoundStmt(TokenPtr* rest, TokenPtr tok) {
  NodePtr sub_expr = std::make_shared<Node>(ND_END, tok);
  NodePtr cur = sub_expr;

  while (!tok->Equal("}")) {
    if (tok->IsTypename()) {
      cur->next = Declaration(&tok, tok);
    } else {
      cur->next = Stmt(&tok, tok);
    }
    cur = cur->next;
    cur->TypeInfer();
  }

  *rest = tok->next;
  return Node::CreateBlockNode(ND_BLOCK, tok, sub_expr->next);
  ;
}

// declaration = declspec (
//                 declarator ( "=" expr)?
//                 ("," declarator ("=" expr)? ) * )? ";"
NodePtr Parser::Declaration(TokenPtr* rest, TokenPtr tok) {
  TypePtr ty_base = Declspec(&tok, tok);

  NodePtr decl_expr = std::make_shared<Node>(ND_END, tok);
  NodePtr cur = decl_expr;
  int i = 0;

  while (!tok->Equal(";")) {
    if (i++ > 0) {
      tok = tok->SkipToken(",");
    }
    TypePtr ty = Declarator(&tok, tok, ty_base);
    ObjectPtr var = Object::CreateLocalVar(ty->name->GetIdent(), ty, &locals);
    if (!tok->Equal("=")) {
      continue;
    }
    NodePtr lhs = Node::CreateVarNode(var, tok);
    NodePtr rhs = Assign(&tok, tok->next);
    NodePtr assgin_node = Node::CreateBinaryNode(ND_ASSIGN, tok, lhs, rhs);
    cur = cur->next = Node::CreateUnaryNode(ND_EXPR_STMT, tok, assgin_node);
  }

  *rest = tok->next;
  return Node::CreateBlockNode(ND_BLOCK, tok, decl_expr->next);
  ;
}

// declspec = "char" | "int"
TypePtr Parser::Declspec(TokenPtr* rest, TokenPtr tok) {
  if (tok->Equal("char")) {
    *rest = tok->SkipToken("char");
    return ty_char;
  }

  *rest = tok->SkipToken("int");
  return ty_int;
}

// declarator = "*"* ident type-suffix
TypePtr Parser::Declarator(TokenPtr* rest, TokenPtr tok, TypePtr ty) {
  while (tok->Equal("*")) {
    ty = Type::CreatePointerType(ty);
    tok = tok->next;
  }
  *rest = tok;

  if (tok->kind != TK_IDENT) {
    tok->ErrorTok("expected a variable name.");
  }
  ty = TypeSuffix(rest, tok->next, ty);
  ty->name = tok;
  return ty;
}

// type-suffix = "(" func-params ")" | "[" num "]" type-suffix | ɛ
TypePtr Parser::TypeSuffix(TokenPtr* rest, TokenPtr tok, TypePtr ty) {
  if (tok->Equal("(")) {
    return FunctionParam(rest, tok->next, ty);
  }
  if (tok->Equal("[")) {
    int len = tok->next->GetNumber();
    tok = tok->next->next->SkipToken("]");
    ty = TypeSuffix(rest, tok, ty);
    return Type::CreateArrayType(ty, len);
  }

  *rest = tok;
  return ty;
}

// func-param = param ("," param) *
// param = declspec declarator
TypePtr Parser::FunctionParam(TokenPtr* rest, TokenPtr tok, TypePtr ty) {
  TypePtr params = std::make_shared<Type>(TY_END, 0);
  TypePtr cur = params;

  while (!tok->Equal(")")) {
    if (cur != params) {
      tok = tok->SkipToken(",");
    }
    TypePtr base_ty = Declspec(&tok, tok);
    TypePtr var_type = Declarator(&tok, tok, base_ty);
    cur->next = std::make_shared<Type>(*var_type);
    cur = cur->next;
  }
  ty = Type::CreateFunctionType(ty, params->next);

  *rest = tok->next;
  return ty;
}

// stmt = "return" expr ";" |
// "if" "(" expr ")" stmt ("else" stmt)? |
// "for" "(" expr-stmt expr? ";" expr? ")" stmt |
// "{" compuound-stmt |
// expr-stmt
NodePtr Parser::Stmt(TokenPtr* rest, TokenPtr tok) {
  if (tok->Equal("return")) {
    NodePtr node = Node::CreateUnaryNode(ND_RETURN, tok, Expr(&tok, tok->next));
    *rest = tok->SkipToken(";");
    return node;
  }

  if (tok->Equal("if")) {
    TokenPtr node_name = tok;
    tok = tok->next;
    NodePtr cond = Expr(&tok, tok->SkipToken("("));
    NodePtr then = Stmt(&tok, tok->SkipToken(")"));
    NodePtr els = nullptr;
    if (tok->Equal("else")) {
      els = Stmt(&tok, tok->next);
    }
    *rest = tok;
    return Node::CreateIfNode(node_name, cond, then, els);
  }

  if (tok->Equal("for")) {
    TokenPtr node_name = tok;
    tok = tok->next->SkipToken("(");

    NodePtr init = ExprStmt(&tok, tok);
    NodePtr cond = nullptr;
    NodePtr inc = nullptr;

    if (!tok->Equal(";")) {
      cond = Expr(&tok, tok);
    }
    tok = tok->SkipToken(";");
    if (!tok->Equal(")")) {
      inc = Expr(&tok, tok);
    }
    tok = tok->SkipToken(")");
    return Node::CreateForNode(node_name, init, cond, inc, Stmt(rest, tok));
  }

  if (tok->Equal("while")) {
    TokenPtr node_name = tok;
    tok = tok->next->SkipToken("(");

    NodePtr cond = Expr(&tok, tok);
    tok = tok->SkipToken(")");
    NodePtr then = Stmt(rest, tok);
    return Node::CreateForNode(node_name, nullptr, cond, nullptr, then);
  }

  if (tok->Equal("{")) {
    return CompoundStmt(rest, tok->next);
  }

  return ExprStmt(rest, tok);
}

// expr-stmt = expr ";"
NodePtr Parser::ExprStmt(TokenPtr* rest, TokenPtr tok) {
  if (tok->Equal(";")) {
    *rest = tok->next;
    return Node::CreateBlockNode(ND_BLOCK, tok, nullptr);
  }
  NodePtr node = Node::CreateUnaryNode(ND_EXPR_STMT, tok, Expr(&tok, tok));
  *rest = tok->SkipToken(";");
  return node;
}

// expr = assign
NodePtr Parser::Expr(TokenPtr* rest, TokenPtr tok) { return Assign(rest, tok); }

// assign = equality ("=" assign)?
NodePtr Parser::Assign(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Equality(&tok, tok);
  if (tok->Equal("=")) {
    return Node::CreateBinaryNode(ND_ASSIGN, tok, node, Assign(rest, tok->next));
  }
  *rest = tok;
  return node;
}

// equality = relational ("==" relational | "!=" relational)
NodePtr Parser::Equality(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Relational(&tok, tok);

  for (;;) {
    TokenPtr node_name = tok;
    if (tok->Equal("==")) {
      node = Node::CreateBinaryNode(ND_EQ, node_name, node, Relational(&tok, tok->next));
      continue;
    }
    if (tok->Equal("!=")) {
      node = Node::CreateBinaryNode(ND_NE, node_name, node, Relational(&tok, tok->next));
      continue;
    }

    *rest = tok;
    return node;
  }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)
NodePtr Parser::Relational(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Add(&tok, tok);

  for (;;) {
    TokenPtr node_name = tok;
    if (tok->Equal("<")) {
      node = Node::CreateBinaryNode(ND_LT, node_name, node, Add(&tok, tok->next));
      continue;
    }
    if (tok->Equal("<=")) {
      node = Node::CreateBinaryNode(ND_LE, node_name, node, Add(&tok, tok->next));
      continue;
    }
    if (tok->Equal(">")) {
      node = Node::CreateBinaryNode(ND_LT, node_name, Add(&tok, tok->next), node);
      continue;
    }
    if (tok->Equal(">=")) {
      node = Node::CreateBinaryNode(ND_LE, node_name, Add(&tok, tok->next), node);
      continue;
    }
    *rest = tok;
    return node;
  }
}

// add = mul ("+"mul | "-" mul)
NodePtr Parser::Add(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Mul(&tok, tok);

  for (;;) {
    TokenPtr node_name = tok;
    if (tok->Equal("+")) {
      node = Node::CreateAddNode(node_name, node, Mul(&tok, tok->next));
      continue;
    }
    if (tok->Equal("-")) {
      node = Node::CreateSubNode(node_name, node, Mul(&tok, tok->next));
      continue;
    }
    *rest = tok;
    return node;
  }
}

NodePtr Parser::Mul(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Unary(&tok, tok);

  for (;;) {
    TokenPtr node_name = tok;
    if (tok->Equal("*")) {
      node = Node::CreateBinaryNode(ND_MUL, node_name, node, Primary(&tok, tok->next));
      continue;
    }
    if (tok->Equal("/")) {
      node = Node::CreateBinaryNode(ND_DIV, node_name, node, Primary(&tok, tok->next));
      continue;
    }
    *rest = tok;
    return node;
  }
}

// unary = ("+" | "-" | "*" | "&") ? unary | primary
NodePtr Parser::Unary(TokenPtr* rest, TokenPtr tok) {
  if (tok->Equal("+")) {
    return Unary(rest, tok->next);
  }
  if (tok->Equal("-")) {
    return Node::CreateUnaryNode(ND_NEG, tok, Unary(rest, tok->next));
  }
  if (tok->Equal("&")) {
    return Node::CreateUnaryNode(ND_ADDR, tok, Unary(rest, tok->next));
  }
  if (tok->Equal("*")) {
    return Node::CreateUnaryNode(ND_DEREF, tok, Unary(rest, tok->next));
  }
  return Postfix(rest, tok);
}

NodePtr Parser::Postfix(TokenPtr* rest, TokenPtr tok) {
  NodePtr node = Primary(&tok, tok);

  while (tok->Equal("[")) {
    // x[y] is short for *(x + y)
    TokenPtr node_name = tok;
    NodePtr idx = Expr(&tok, tok->next);
    tok = tok->SkipToken("]");
    NodePtr op = Node::CreateAddNode(node_name, node, idx);
    node = Node::CreateUnaryNode(ND_DEREF, node_name, op);
  }
  *rest = tok;
  return node;
}

// primary = "(" "{" stmt+ "}" ")"
//          |"(" expr ")" | "sizeof" unary | ident func-args? | str | num
NodePtr Parser::Primary(TokenPtr* rest, TokenPtr tok) {
  // This is a GNU statement expression.
  if (tok->Equal("(") && tok->next->Equal("{")) {
    TokenPtr start = tok;
    NodePtr stmt = CompoundStmt(&tok, tok->next->next);
    *rest = tok->SkipToken(")");
    return Node::CreateBlockNode(ND_STMT_EXPR, start, stmt->body);
  }
  if (tok->Equal("(")) {
    NodePtr node = Expr(&tok, tok->next);
    *rest = tok->SkipToken(")");
    return node;
  }

  if (tok->Equal("sizeof")) {
    NodePtr node = Unary(rest, tok->next);
    node->TypeInfer();
    long size = node->ty->size;
    return Node::CreateConstNode(size, tok);
  }

  if (tok->kind == TK_IDENT) {
    if (tok->next->Equal("(")) {
      return Call(rest, tok);
    }
    *rest = tok->next;
    return Node::CreateIdentNode(tok);
  }

  if (tok->kind == TK_STR) {
    ObjectPtr var = Object::CreateStringVar(String(tok->str_literal));
    *rest = tok->next;
    return Node::CreateVarNode(var, tok);
  }

  if (tok->kind == TK_NUM) {
    NodePtr node = Node::CreateConstNode(tok->val, tok);
    *rest = tok->next;
    return node;
  }

  tok->ErrorTok("expected an expression");
  return nullptr;
}

// function = ident "(" (assign ("," assign)*)? ")"
NodePtr Parser::Call(TokenPtr* rest, TokenPtr tok) {
  TokenPtr start = tok;
  tok = tok->next->next;

  NodePtr args = std::make_shared<Node>(ND_END, tok);
  NodePtr cur = args;

  while (!tok->Equal(")")) {
    if (cur != args) {
      tok = tok->SkipToken(",");
    }
    cur->next = Assign(&tok, tok);
    cur = cur->next;
  }

  *rest = tok->SkipToken(")");
  return Node::CreateCallNode(start, args->next);
}
