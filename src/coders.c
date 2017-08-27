#include "coders.h"
void
binary_encode(uint32_t value, uint32_t length, t_bwriter * writer)
{
    uint32_t b, v = value;
    while(length > 0)
    {
        b = v % 2;
        write_bit(b, writer);
        v = v >> 1;
        length--;
    }
}
uint32_t
binary_decode(uint32_t * V,uint32_t length, t_breader * reader)
{
    uint32_t i = 0, b;
    *V = 0;
    while(i<length)
    {
        if (get_bit(reader, &b) == 0) return 0;
        *V += b << i;
        i++;
    }
    return 1;
}
void
unary_encode(uint32_t value, t_bwriter * writer)
{
    uint32_t v = value;
    while(v > 1)
    {
        write_bit(1, writer);
        v--;
    }
    write_bit(0, writer);
}
uint32_t
unary_decode(uint32_t * V, t_breader * reader)
{
    uint32_t b;
    *V = 1;
    if (get_bit(reader, &b) == 0) return 0;
    while(b == 1)
    {
        *V= *V + 1;
        if (get_bit(reader, &b) == 0) return 0;
    }
    return 1;
}
void
elias_gamma_encode(uint32_t value, t_bwriter * writer)
{
    uint32_t l = (uint32_t)log2(value);
    unary_encode(l + 1, writer);
    binary_encode(value, l, writer);
}
uint32_t
elias_gamma_decode(uint32_t * V,t_breader * reader)
{
    uint32_t l;
    if(unary_decode(&l, reader) == 0) return 0;
    l--;
    if(l == 0){
        *V = 1;
        return 1;
    }
    if(binary_decode(V, l, reader) == 0) return 0;
    *V = *V + (1 << l);
    return 1;
}
void
elias_delta_encode(uint32_t value, t_bwriter * writer)
{
    uint32_t l = (uint32_t)log2(value);
    elias_gamma_encode(l + 1, writer);
    binary_encode(value, l, writer);
}
uint32_t
elias_delta_decode(uint32_t * V,t_breader * reader)
{
    uint32_t l;
    if(elias_gamma_decode(&l, reader) == 0) return 0;
    l--;
    if(l == 0){
        *V = 1;
        return 1;
    }
    if(binary_decode(V, l, reader) == 0) return 0;
    *V = *V + (1 << l);
    return 1;
}


uint32_t log2(uint32_t value){
    return log(value)/log(2);
}