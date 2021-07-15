.686p
.model flat

.code



; Assembly function to trigger a software breakpoint in the attached debugger
; INFO: If debugger is not attached, the process will simply crash
;
_breakpoint_func@0 PROC
	xor		eax, eax
	int		3
	ret
_breakpoint_func@0 ENDP


END


