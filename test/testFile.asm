
strlen:
	push rbp
	mov rbp, rsp

	mov rsi, [rsp+16]
	mov rcx, -1
.strlen0:
	inc rcx
	cmp byte [rsi + rcx], 0x00
	jne .strlen0
.strlen1:
	mov rdx, rcx

	mov rsp, rbp
	pop rbp
	ret



print:
    push rbp
    mov rbp, rsp
    
    mov rax, [rsp+16]
    push rax
    call strlen
    add rsp, 8

    mov rax, 1
    mov rdi, 1
    mov rsi, [rsp+16]
    syscall

    mov rsp, rbp
    pop rbp
    ret



exit:
    mov rax, 60
    mov rdi, QWORD [rsp+8]
    add rsp, 8
    syscall


global main
main:
sub rsp, 8
mov QWORD [rsp+8], STR0

push QWORD [rsp+8]
call print
add rsp, 8
mov QWORD [rsp+8], 10
push QWORD [rsp+8]
call exit
add rsp, 8

mov rax, 60
mov rdi, 0
syscall


section .data
STR0: db "Hello, World!", 0xa, 0
