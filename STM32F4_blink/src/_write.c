//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// Do not include on semihosting and when freestanding
#if !defined(OS_USE_SEMIHOSTING) && !(__STDC_HOSTED__ == 0)

// ----------------------------------------------------------------------------

#include <errno.h>
#include "diag/Trace.h"
#include "globalvars.h"
#include "FreeRTOS.h"
#include "task.h"

//extern UART_HandleTypeDef UartHandle;

// ----------------------------------------------------------------------------

// When using retargetted configurations, the standard write() system call,
// after a long way inside newlib, finally calls this implementation function.

// Based on the file descriptor, it can send arrays of characters to
// different physical devices.

// Currently only the output and error file descriptors are tested,
// and the characters are forwarded to the trace device, mainly
// for demonstration purposes. Adjust it for your specific needs.

// For freestanding applications this file is not used and can be safely
// ignored.

ssize_t
_write(int fd, const char* buf, size_t nbyte);

ssize_t _write(int fd __attribute__((unused)),
		const char* buf __attribute__((unused)),
		size_t nbyte __attribute__((unused))) {
#if defined(TRACE)
	// STDOUT and STDERR are routed to the trace device
	if (fd == 1 || fd == 2)
	{
		return trace_write (buf, nbyte);
	}
#endif // TRACE

	for (unsigned int i=0; i<nbyte; i++)
	{
		if (buf[i] == '\n')
		{
			while (!(USART2->SR & UART_FLAG_TXE)) taskYIELD();
			USART2->DR = (unsigned int)0x0D;
			while (!(USART2->SR & UART_FLAG_TXE)) taskYIELD();
			USART2->DR = (unsigned int)0x0A;
		}
			else
		{
			while (!(USART2->SR & UART_FLAG_TXE)) taskYIELD();
			USART2->DR = (unsigned int)buf[i];
		}
	};
	//HAL_UART_Transmit(&UartHandle, (uint8_t*)buf, nbyte, 0xFFFF);
	errno = ENOSYS;
	return nbyte;
}

// ----------------------------------------------------------------------------

#endif // !defined(OS_USE_SEMIHOSTING) && !(__STDC_HOSTED__ == 0)
