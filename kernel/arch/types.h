#ifndef ARCH_TYPES_H
#define ARCH_TYPES_H

/*
 * architecture-independent type definitions
 * each architecture provides its implementation in arch/<arch>/types.h
 */

#if defined(ARCH_AMD64)
    #include <arch/amd64/types.h>
#elif defined(ARCH_X86)
    #error "x86 not implemented"
#elif defined(ARCH_ARM64)
    #error "ARM64 not implemented"
#else
    #error "Unsupported architecture"
#endif

/*
 * each arch must define:
 *
 * ARCH_BITS - 32 or 64
 * ARCH_PTR_SIZE - sizeof(void*) in bytes
 *
 * exact width types (all archs):
 *
 * int8, uint8, int16, uint16, int32, uint32, int64, uint64
 *
 * pointer-sized types (arch-specific):
 *
 * intptr, uintptr - pointer-sized signed/unsigned
 * size, ssize - size types
 *
 * native word types (arch-specific):
 *
 * word, uword - native register width
 */

#endif
