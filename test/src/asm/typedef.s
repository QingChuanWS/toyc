.file 1 "asm_file//typedef.c"
  .data
  .global .L..0
.L..0:
  .byte 40
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 105
  .byte 110
  .byte 116
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 116
  .byte 32
  .byte 120
  .byte 61
  .byte 49
  .byte 59
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..1
.L..1:
  .byte 40
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 115
  .byte 116
  .byte 114
  .byte 117
  .byte 99
  .byte 116
  .byte 32
  .byte 123
  .byte 105
  .byte 110
  .byte 116
  .byte 32
  .byte 97
  .byte 59
  .byte 125
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 116
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 120
  .byte 46
  .byte 97
  .byte 61
  .byte 49
  .byte 59
  .byte 32
  .byte 120
  .byte 46
  .byte 97
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..2
.L..2:
  .byte 40
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 105
  .byte 110
  .byte 116
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 116
  .byte 32
  .byte 116
  .byte 61
  .byte 49
  .byte 59
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..3
.L..3:
  .byte 40
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 115
  .byte 116
  .byte 114
  .byte 117
  .byte 99
  .byte 116
  .byte 32
  .byte 123
  .byte 105
  .byte 110
  .byte 116
  .byte 32
  .byte 97
  .byte 59
  .byte 125
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 105
  .byte 110
  .byte 116
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 125
  .byte 32
  .byte 116
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 120
  .byte 46
  .byte 97
  .byte 61
  .byte 50
  .byte 59
  .byte 32
  .byte 120
  .byte 46
  .byte 97
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..4
.L..4:
  .byte 40
  .byte 123
  .byte 32
  .byte 116
  .byte 121
  .byte 112
  .byte 101
  .byte 100
  .byte 101
  .byte 102
  .byte 32
  .byte 116
  .byte 59
  .byte 32
  .byte 116
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 115
  .byte 105
  .byte 122
  .byte 101
  .byte 111
  .byte 102
  .byte 40
  .byte 120
  .byte 41
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..5
.L..5:
  .byte 40
  .byte 123
  .byte 32
  .byte 77
  .byte 121
  .byte 73
  .byte 110
  .byte 116
  .byte 32
  .byte 120
  .byte 61
  .byte 51
  .byte 59
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..6
.L..6:
  .byte 40
  .byte 123
  .byte 32
  .byte 77
  .byte 121
  .byte 73
  .byte 110
  .byte 116
  .byte 50
  .byte 32
  .byte 120
  .byte 59
  .byte 32
  .byte 115
  .byte 105
  .byte 122
  .byte 101
  .byte 111
  .byte 102
  .byte 40
  .byte 120
  .byte 41
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .global .L..7
.L..7:
  .byte 79
  .byte 75
  .byte 10
  .byte 0
  .byte 0
  .intel_syntax noprefix
  .global main
 .text
main:
  push rbp
  mov rbp, rsp
  sub rsp, 48
  .loc 1 60
  .loc 1 51
  .loc 1 51
  .loc 1 51
  .loc 1 51
  mov rax, 1
  push rax
  .loc 1 51
  .loc 1 51
  .loc 1 51
  .loc 1 51
  .loc 1 51
  lea rax, [rbp - 40]
  push rax
  .loc 1 51
  .loc 1 51
  mov rax, 1
  pop rdi
  mov [rdi], eax
  .loc 1 51
  .loc 1 51
  lea rax, [rbp - 40]
  movsxd rax, DWORD PTR [rax]
  push rax
  .loc 1 51
  .loc 1 51
  lea rax, [rip + .L..0]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 52
  .loc 1 52
  .loc 1 52
  .loc 1 52
  mov rax, 1
  push rax
  .loc 1 52
  .loc 1 52
  .loc 1 52
  .loc 1 52
  .loc 1 52
  lea rax, [rbp - 36]
  add rax, 0
  push rax
  .loc 1 52
  .loc 1 52
  mov rax, 1
  pop rdi
  mov [rdi], eax
  .loc 1 52
  .loc 1 52
  lea rax, [rbp - 36]
  add rax, 0
  movsxd rax, DWORD PTR [rax]
  push rax
  .loc 1 52
  .loc 1 52
  lea rax, [rip + .L..1]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 53
  .loc 1 53
  .loc 1 53
  .loc 1 53
  mov rax, 1
  push rax
  .loc 1 53
  .loc 1 53
  .loc 1 53
  .loc 1 53
  .loc 1 53
  lea rax, [rbp - 32]
  push rax
  .loc 1 53
  .loc 1 53
  mov rax, 1
  pop rdi
  mov [rdi], eax
  .loc 1 53
  .loc 1 53
  lea rax, [rbp - 32]
  movsxd rax, DWORD PTR [rax]
  push rax
  .loc 1 53
  .loc 1 53
  lea rax, [rip + .L..2]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 54
  .loc 1 54
  .loc 1 54
  .loc 1 54
  mov rax, 2
  push rax
  .loc 1 54
  .loc 1 54
  .loc 1 54
  .loc 1 54
  .loc 1 54
  .loc 1 54
  lea rax, [rbp - 28]
  add rax, 0
  push rax
  .loc 1 54
  .loc 1 54
  mov rax, 2
  pop rdi
  mov [rdi], eax
  .loc 1 54
  .loc 1 54
  lea rax, [rbp - 28]
  add rax, 0
  movsxd rax, DWORD PTR [rax]
  push rax
  .loc 1 54
  .loc 1 54
  lea rax, [rip + .L..3]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 55
  .loc 1 55
  .loc 1 55
  .loc 1 55
  mov rax, 4
  push rax
  .loc 1 55
  .loc 1 55
  .loc 1 55
  .loc 1 55
  .loc 1 55
  mov rax, 4
  push rax
  .loc 1 55
  .loc 1 55
  lea rax, [rip + .L..4]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 56
  .loc 1 56
  .loc 1 56
  .loc 1 56
  mov rax, 3
  push rax
  .loc 1 56
  .loc 1 56
  .loc 1 56
  .loc 1 56
  .loc 1 56
  lea rax, [rbp - 20]
  push rax
  .loc 1 56
  .loc 1 56
  mov rax, 3
  pop rdi
  mov [rdi], eax
  .loc 1 56
  .loc 1 56
  lea rax, [rbp - 20]
  movsxd rax, DWORD PTR [rax]
  push rax
  .loc 1 56
  .loc 1 56
  lea rax, [rip + .L..5]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 57
  .loc 1 57
  .loc 1 57
  .loc 1 57
  mov rax, 16
  push rax
  .loc 1 57
  .loc 1 57
  .loc 1 57
  .loc 1 57
  .loc 1 57
  mov rax, 16
  push rax
  .loc 1 57
  .loc 1 57
  lea rax, [rip + .L..6]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 0
  call assert
  .loc 1 58
  .loc 1 58
  .loc 1 58
  lea rax, [rip + .L..7]
  push rax
  pop rdi
  mov rax, 0
  call printf
  .loc 1 59
  .loc 1 59
  .loc 1 59
  mov rax, 0
  jmp .L.return.main
.L.return.main:
  mov rsp, rbp
  pop rbp
  ret
