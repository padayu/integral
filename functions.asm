section .data
one dq 1.0
two dq 2.0
three dq 3.0
five dq 5.0
buff1 dq 0.0
buff2 dq 0.0
 
section .text
 
global f
f:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [ebp + 8]
    fisttp qword [buff1]
    fld qword [ebp + 8]
    fild qword [buff1]
    fsubp
    f2xm1
    fld qword [one]
    faddp
    fstp qword [buff2]
 
    fild qword [buff1]
    fld qword [one]
    fscale
    fstp
 
    fld qword [buff2]
 
    fmulp
 
    fld qword [one]
    faddp
 
    leave
    xor eax, eax
    ret
 
global g
g:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [ebp + 8]
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
 
    leave
    xor eax, eax
    ret
 
global h
h:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [one]
    fld qword [ebp + 8]
    fsubp
    fld qword [three]
    fdivp
 
    leave
    xor eax, eax
    ret
 
global f_
f_:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [ebp + 8]
    fisttp qword [buff1]
    fld qword [ebp + 8]
    fild qword [buff1]
    fsubp
    f2xm1
    fld qword [one]
    faddp
    fstp qword [buff2]
 
    fild qword [buff1]
    fld qword [one]
    fscale
    fstp
 
    fld qword [buff2]
 
    fmulp
 
    fldln2
    fmulp
 
    leave
    xor eax, eax
    ret
 
global g_
g_:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [five]
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
    fld qword [ebp + 8]
    fmulp
 
    leave
    xor eax, eax
    ret
 
global h_
h_:
    push ebp
    mov ebp, esp
 
    finit
    fld qword [one]
    fld qword [three]
    fdivp
    fchs
 
    leave
    xor eax, eax
    ret