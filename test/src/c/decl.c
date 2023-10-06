/*
 * This project is exclusively owned by QingChuanWS and shall not be used for
 * commercial and profitting purpose without QingChuanWS's permission.
 * 
 * @Author: bingshan45@163.com
 * Github: https://github.com/QingChuanWS
 * @Description: 
 * 
 * Copyright (c) 2023 by QingChuanWS, All Rights Reserved. 
 */
#include "test.h"

int main() {
  ASSERT(1, ({ char x; sizeof(x); }));
  ASSERT(2, ({ short int x; sizeof(x); }));
  ASSERT(2, ({ int short x; sizeof(x); }));
  ASSERT(4, ({ int x; sizeof(x); }));
  ASSERT(8, ({ long int x; sizeof(x); }));
  ASSERT(8, ({ int long x; sizeof(x); }));

  ASSERT(8, ({ long long x; sizeof(x); }));
  ASSERT(8, ({ long long int x; sizeof(x); }));

  ASSERT(0, ({ _Bool x=0; x; }));
  ASSERT(1, ({ _Bool x=1; x; }));
  ASSERT(1, ({ _Bool x=2; x; }));
  ASSERT(1, (_Bool)1);
  ASSERT(1, (_Bool)2);
  ASSERT(0, (_Bool)(char)256);


  printf("OK\n");
  return 0;
}