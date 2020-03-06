#include <stdio.h>
#include <assert.h>
#include "bitwise.h"

union Myfloat {
    float_bits a;
    float b;
};

int main(){

    int i;
    unsigned max = (unsigned) -1;
    printf("MAX: %u\n", max);

    //test _261a
    /*
    for (i = 0; i < max; i++) {
      if (i == 0) { assert(_261a(0) == 0);}
      else { assert(_261a(i) == 1);}
    }
    printf("All tests passed for _261a.\n");
    */

    //test float_twice
    union Myfloat bits;
    for (i =8388609; i < 100000000; i++) {
        bits.a = i;
        printf("%d\n", i);
        printf("%f\n", i);
        //if exp bits == 0 and sign = 0
        if (i >= 2139095040 && i <= 2147483647) {
            assert( float_twice(bits.a) == i);
        }
            //if exp bits == 0 and sign = 1
        else if (i >= 4286578688 && i <= 4294967295) {
            assert( float_twice(bits.a) == i);
        }
        else {
            assert( float_twice(bits.a) == 2.0*i);
        }
    }

    printf("Test passed for float_twice.\n");

    assert(_261b(0) == 1);
    assert(_261b(-1) == 0);
    assert(_261b(-2) == 1);

    assert(_261c(0) == 0);
    assert(_261c(128) == 1);
    assert(_261c(256) == 0);
    assert(_261c(-1) == 1);
    assert(_261c(1) == 1);

    assert(_261d(4286578688) == 0);
    assert(_261d(2147483648) == 1);
    assert(_261d(-1) == 0);
    assert(_261d(-127) == 0);
    assert(_261d(4261412864) == 1);
    assert(_261d(1) == 1);
    assert(_261d(2852126720) == 1);

    assert(rotat_left(0x12345678, 4) == 0x23456781);
    assert(rotat_left(0x12345678, 0) == 0x12345678);
    assert(rotat_left(0x12345678, 20) == 0x67812345);
    assert(rotat_left(0x12345678, 32) == 0x12345678);
    assert(rotat_left(0x00000001, 1) == 0x00000002);

    printf("All tests passed.\n");
    return 0;
}

int _261a(int x){
    int result = !!x;
    return result;
}

int _261b(int x){
    int result = !!(~x);
    return result;
}

int _261c(int x){
    x = x << 24;
    int result = !!x;
    return result;
}

int _261d(int x){
    unsigned int shifted = x;
    shifted = shifted >> 24;
    shifted = shifted ^ 255;
    shifted = !!shifted;
    return shifted;
}

unsigned rotat_left(unsigned x, int n) {
    unsigned w = sizeof(unsigned) << 3;
    unsigned y = x << n;
    unsigned z = x >> (w-n-1);
    z = z >> 1;
    unsigned result = y | z;
    printf("%x\n", result);
    return result;
}

float_bits float_negate(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exponent = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFF;

    if (exponent == 255) { //For NAN and Infinity
        return f;
    }
    else {
        sign = sign ^ 1;
        return sign << 31 | exponent << 23 | frac;
    }
}

float_bits float_twice(float_bits f) {
    unsigned sign = f >> 31;
    unsigned exponent = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;
    unsigned firstfrac = f & 0x400000; //first frac > 0 or not.

    //treat NAN and Infinity the same (infinity * 2 == infinity)
    if (exponent == 255) {
        return f;
    }
        //CASE 1: if f = 0
    else if (exponent == 0 && frac == 0) {
        printf("CASE 1\n");
        return f;
    }
        //CASE 2: denormalized part 1 -> switches to normalized mode
    else if (exponent == 0 && firstfrac > 0) {
        printf("CASE 2\n");
        frac = frac << 1;
        frac = frac & 0x7FFFF;
        exponent = exponent + 1; //exponent += 2;
        return sign << 31 | exponent << 23 | frac;
    }
        //CASE 3: denormalized part 2
    else if (exponent == 0 && firstfrac == 0) {
        printf("CASE 3\n");
        frac = frac << 1;
        return sign << 31 | exponent << 23 | frac;
    }
        //CASE 4: normalized
    else {
        printf("CASE 4\n");
        printf("Exponent Before: %u\n", exponent);
        exponent = exponent + 1;
        printf("Exponent After: %u\n", exponent);
        printf("%f\n", sign << 31 | exponent << 23 | frac);
        return sign << 31 | exponent << 23 | frac;
    }
}
