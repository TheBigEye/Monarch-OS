#ifndef CSTDDDEF_H
#define CSTDDDEF_H

// Define a new type, ptrdiff_t, which represents the difference between two pointers.
typedef long ptrdiff_t;

typedef unsigned long long  size_t;             // 8 byte, 64 bits object size

// The largest alignment requirement of any type.
union max_align_t {
    char c;
    short s;
    int i;
    long l;
    long long ll;
    float f;
    double d;
    long double ld;
    void *p;
    void (*fp)(void);
};

// Represents a null pointer constant.
#define NULL 0

// Returns the offset of a field within a structure.
#define offsetof(type, member) (reinterpret_cast<size_t>(&(static_cast<type*>(0)->member)))

#endif /* CSTDDDEF_H_ */
