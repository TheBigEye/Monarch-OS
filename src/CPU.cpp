#include "CPU.h"
#include "common/typedefs.h"
#include "IO.h"

using namespace std;

// This function returns the CPU name
string CPU_name() {
    string name;
    uint_32 eax, ebx, ecx, edx;

    asm volatile (
        "cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x80000002)
    );

    name += (char)eax;
    name += (char)ebx;
    name += (char)ecx;
    name += (char)edx;

    asm volatile (
        "cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x80000003)
    );

    name += (char)eax;
    name += (char)ebx;
    name += (char)ecx;
    name += (char)edx;

    asm volatile (
        "cpuid" : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (0x80000004)
    );

    name += (char)eax;
    name += (char)ebx;
    name += (char)ecx;
    name += (char)edx;

    return name;
}

// This function returns the CPU frequency
uint_32 CPU_freq() {
    uint_32 freq = 0;
    asm volatile (
        "cpuid" : "=A" (freq)
    );
    return freq;
}

// This function returns the CPU cycles per second
uint_64 CPU_cycles() {
    uint_64 start, end;
    uint_32 cycles_low, cycles_high;
    uint_32 multiplier;

    asm volatile (
        "CPUID\n\t"
        "RDTSC\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
        "%rax", "%rbx", "%rcx", "%rdx"
    );

    start = ( (uint_64)cycles_high << 32) | cycles_low;

    asm volatile (
        "CPUID\n\t"
        "RDTSC\n\t"
        "mov %%edx, %0\n\t"
        "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
        "%rax", "%rbx", "%rcx", "%rdx"
    );

    end = ( (uint_64)cycles_high << 32) | cycles_low;

    multiplier = end - start;

    return multiplier;
}
