#ifndef __CC_H__
#define __CC_H__

#include "cpu.h"

//#define LWIP_PROVIDE_ERRNO 1
#include "lwip_errno.h"

// Typedefs for the types used by lwip

#include "ttc_basic.h"
#include <stdio.h>

/* These ones are already defined in stm32_io.h
   typedef unsigned   char    u8_t;
   typedef signed     char    s8_t;
   typedef unsigned   short   u16_t;
   typedef signed     short   s16_t;
   typedef unsigned   long    u32_t;io.h
   typedef signed     long    s32_t;
*/

typedef u32_t mem_ptr_t;

typedef int sys_prot_t;
///
//
///

// Compiler hints for packing lwip's structures

#define PACK_STRUCT_BEGIN
//#define PACK_STRUCT_BEGIN _Pragma("pack(2)")
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
//#define PACK_STRUCT_END _Pragma("pack()")
#define PACK_STRUCT_FIELD(x) x

// Platform specific diagnostic output


// non-fatal, print a message.x
#ifndef LWIP_PLATFORM_DIAG
#define LWIP_PLATFORM_DIAG(x) printf x;
#endif
// fatal, print message and abandon execution.
#define LWIP_PLATFORM_ASSERT(x) {LWIP_PLATFORM_DIAG((x));Assert(0,ec_UNKNOWN);}


// "lightweight" synchronization mechanisms

// declare a protection state variable.
#define SYS_ARCH_DECL_PROTECT(x)
// enter protection mode.
#define SYS_ARCH_PROTECT(x)
// leave protection mode.
#define SYS_ARCH_UNPROTECT(x)

/* Define (sn)printf formatters for these lwIP types */
#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "lu"
#define S32_F "ld"
#define X32_F "lx"

#endif /* __CC_H__ */
