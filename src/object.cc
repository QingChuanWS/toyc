/*
 * This project is exclusively owned by QingChuanWS and shall not be used for
 * commercial and profitting purpose without QingChuanWS's permission.
 *
 * @ Author: bingshan45@163.com
 * @ Github: https://github.com/QingChuanWS
 * @ Description:
 *
 * Copyright (c) 2023 by QingChuanWS, All Rights Reserved.
 */

#include "object.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "node.h"
#include "parser.h"
#include "token.h"
#include "tools.h"
#include "type.h"
#include "utils.h"

// All local variable instance created during parsing are accumulated to this list.
// each function has self local variable.
ObjectPtr locals = nullptr;
// Likewise, global variable are accumulated to this list.
ObjectPtr globals = nullptr;
// All variable or tag scope instance are accumulated to this list.
ScopePtr scope = nullptr;

ObjectPtr cur_fn = nullptr;

void Scope::EnterScope(ScopePtr& next) {
  ScopePtr sc = std::make_shared<Scope>();
  sc->next = next;
  next = sc;
}

void Scope::LevarScope(ScopePtr& next) { next = next->next; }

VarScopePtr& Scope::PushVarScope(const String& name) {
  scope->vars[name] = std::make_shared<VarScope>();
  return scope->vars[name];
}

VarScopePtr Scope::FindVarScope(const String& name) {
  for (ScopePtr sc = scope; sc != nullptr; sc = sc->next) {
    auto v = sc->vars.find(name);
    if (v != sc->vars.end()) {
      return v->second;
    }
  }
  return nullptr;
}

TypePtr Scope::FindTag(const String& name) {
  for (ScopePtr sc = scope; sc != nullptr; sc = sc->next) {
    auto t = sc->tags.find(name);
    if (t != sc->tags.end()) {
      return t->second;
    }
  }
  return nullptr;
}

const TypePtr Scope::FindTypedef(const TokenPtr& tok) {
  if (tok->GetKind() == TK_IDENT) {
    VarScopePtr v = FindVarScope(tok->GetIdent());
    if (v != nullptr) {
      return v->GetType();
    }
  }
  return nullptr;
}

ObjectPtr Object::CreateVar(Objectkind kind, const String& name, const TypePtr& ty) {
  ObjectPtr obj = std::make_shared<Object>(kind, name, ty);
  Scope::PushVarScope(name)->SetVar(obj);
  return obj;
}

ObjectPtr Object::CreateLocalVar(const String& name, const TypePtr& ty, ObjectPtr* next) {
  ObjectPtr obj = CreateVar(OB_LOCAL, name, ty);
  // if (Find(name.c_str()) != nullptr) {
  //   ty->name->ErrorTok("redefined variable.");
  // }
  obj->next = *next;
  *next = obj;
  return obj;
}

ObjectPtr Object::CreateGlobalVar(const String& name, const TypePtr& ty, ObjectPtr* next) {
  ObjectPtr obj = CreateVar(OB_GLOBAL, name, ty);
  // if (ty->HasName() && ty->name->FindVar() != nullptr) {
  //   ty->name->ErrorTok("redefined variable.");
  // }
  obj->next = *next;
  *next = obj;
  return obj;
}

ObjectPtr Object::CreateStringVar(const String& name) {
  TypePtr ty = Type::CreateArrayType(ty_char, name.size());
  ObjectPtr obj = CreateGlobalVar(CreateUniqueName(), ty, &globals);
  obj->init_data = name;
  obj->is_string = true;
  return obj;
}

TokenPtr Object::CreateFunction(TokenPtr tok, TypePtr basety, ObjectPtr* next) {
  TypePtr ty = Parser::Declarator(&tok, tok, basety);
  ObjectPtr fn = CreateVar(OB_FUNCTION, ty->name->GetIdent(), ty);

  // function declaration
  if (tok->Equal(";")) {
    fn->is_defination = true;
    tok = tok->SkipToken(";");
    return tok;
  }
  cur_fn = fn;
  locals = nullptr;
  // create scope.
  Scope::EnterScope(scope);

  // funtion defination.
  CreateParamVar(ty->params);
  fn->params = locals;
  fn->body = Parser::Program(&tok, tok);
  fn->loc_list = locals;
  fn->next = *next;
  *next = fn;
  // leave scope.
  Scope::LevarScope(scope);
  return tok;
}

void Object::CreateParamVar(TypePtr param) {
  if (param != nullptr) {
    CreateParamVar(param->next);
    ObjectPtr v = CreateLocalVar(param->name->GetIdent(), param, &locals);
  }
}

bool Object::IsFuncToks(TokenPtr tok) {
  if (tok->Equal(";")) {
    return false;
  }
  while (tok->Equal("*")) {
    tok = tok->GetNext();
  }
  if (tok->GetKind() != TK_IDENT) {
    tok->ErrorTok("expected a variable name.");
  }
  tok = tok->GetNext();
  if (tok->Equal("(")) {
    return true;
  }
  return false;
}

ObjectPtr Object::Parse(TokenPtr tok) {
  globals = nullptr;
  // enter scope
  Scope::EnterScope(scope);
  while (!tok->IsEof()) {
    VarAttrPtr attr = std::make_shared<VarAttr>();
    TypePtr basety = Parser::Declspec(&tok, tok, attr);

    if (attr->is_typedef) {
      Parser::ParseTypedef(&tok, tok, basety);
      continue;
    }

    if (IsFuncToks(tok)) {
      tok = CreateFunction(tok, basety, &globals);
      continue;
    }
    tok = ParseGlobalVar(tok, basety);
  }
  // leave scope.
  Scope::LevarScope(scope);
  return globals;
}

TokenPtr Object::ParseGlobalVar(TokenPtr tok, TypePtr basety) {
  bool first = true;

  while (!tok->Equal(";")) {
    if (!first) {
      tok = tok->SkipToken(",");
    }
    first = false;
    TypePtr ty = Parser::Declarator(&tok, tok, basety);
    ObjectPtr gv = CreateGlobalVar(ty->name->GetIdent(), ty, &globals);
  }
  return tok->SkipToken(";");
}

void Object::OffsetCal() {
  for (Object* fn = this; fn != nullptr; fn = fn->next.get()) {
    if (!fn->IsFunction()) {
      continue;
    }

    int offset = 0;
    for (ObjectPtr v = fn->loc_list; v != nullptr; v = v->next) {
      offset += v->ty->size;
      offset = AlignTo(offset, v->ty->align);
      v->offset = offset;
    }
    fn->stack_size = AlignTo(offset, 16);
  }
}
