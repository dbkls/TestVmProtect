
.code



; Assembly function to trigger a software breakpoint in the attached debugger
; INFO: If debugger is not attached, the process will simply crash
;
breakpoint_func PROC
	xor		eax, eax
	int		3
	ret
breakpoint_func ENDP


END


