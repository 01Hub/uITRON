/** 
 *  Hyper Operating System V4 Advance
 *
 * Copyright (C) 1998-2011 by Project HOS
 * http://sourceforge.jp/projects/hos/
 */


				.syntax unified


				.text
				.align	2

				.global	main

/************************************************
  リセットハンドラ
 ************************************************/
				.global	_kernel_nmi_handler
				.thumb
				.thumb_func
				.type	_kernel_nmi_handler, %function
_kernel_nmi_handler:
				b		_kernel_nmi_handler
				
				.size	_kernel_nmi_handler, .-_kernel_nmi_handler


				.end


; end of file

