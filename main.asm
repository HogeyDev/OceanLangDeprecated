section .data
.S0 db "Hello, World!", 0
section .text

	print:
	push ebp
	mov ebp, esp

	mov eax, [esp+8]
	push eax
	call strlen
	add esp, 4

	mov eax, 4
	mov ebx, 1
	mov ecx, [esp+8]
	int 0x80

	mov esp, ebp
	pop ebp
	ret

	strlen:
	push ebp
	mov ebp, esp

	mov esi, [esp+8]
	mov ecx, -1
	.strlen0:
	inc ecx
	cmp byte [esi + ecx], 0x00
	jne .strlen0
	.strlen1:
	mov edx, ecx

	mov esp, ebp
	pop ebp
	ret
global _start
_start:
push ebp
mov ebp, esp
push .S0
call print
add esp, 4
mov eax, 1
mov ebx, 0
int 0x80
