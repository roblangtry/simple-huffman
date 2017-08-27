#include "new.h"
const uint32_t BIT_VALUES[] = {
    0,
    1,
    2,
    4,
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
    524288,
    1048576,
    2097152,
    4194304,
    8388608,
    16777216,
    33554432,
    67108864,
    134217728,
    268435456,
    536870912,
    1073741824,
    2147483648
};
int run_new_compression(){
    t_C_stats * stats = malloc(sizeof(t_C_stats));
    t_C_model * model = malloc(sizeof(t_C_model));
    t_C_block * block = malloc(sizeof(t_C_block));
    t_bwriter * writer = malloc(sizeof(t_bwriter));
    start_bwriter(writer);
    while((block->len = fread(block->content, sizeof(uint32_t), BLOCK_SIZE, stdin)) > 0){
        get_compression_statistics(block, stats, writer);
        build_compression_model(block, stats, model, writer);
        run_compression_model(block, model, writer);
    }
    flush_bits(writer);
    return 0;
}
int run_new_decompression(){
    t_D_stats * stats = malloc(sizeof(t_D_stats));
    t_D_model * model = malloc(sizeof(t_D_model));
    t_breader * reader = malloc(sizeof(t_breader));
    t_iwriter * writer = malloc(sizeof(t_iwriter));
    start_breader(reader);
    start_iwriter(writer);
    while(atend(reader)){
        if(get_decompression_statistics(stats, reader) == 0) break;
        if(build_decompression_model(stats, model, reader) == 0) break;
        if(run_decompression_model(model, reader, writer) == 0) break;
    }
    flush_ints(writer);
    return 0;
}
int
get_compression_statistics(t_C_block * block, t_C_stats * stats, t_bwriter * writer)
{
    uint32_t * hashmap = calloc(SYMBOL_MAP_SIZE, sizeof(uint32_t));
    uint32_t max = 0;
    uint32_t i;
    uint32_t j = 0;
    t_vector vector;
    stats->no_distinct = 0;
    stats->total = 0;
    for(i = 0; i < block->len; i++)
    {
        stats->total++;
        if(hashmap[block->content[i]] == 0) stats->no_distinct++;
        if(block->content[i] > max) max = block->content[i];
        hashmap[block->content[i]]++;
    }
    stats->freqs = malloc(sizeof(t_vector) * stats->no_distinct);
    for(i=0; i <= max; i++)
    {
        if(hashmap[i] > 0)
        {
            vector.value = i;
            vector.freq = hashmap[i];
            stats->freqs[j++] = vector;
        }
    }
    free(hashmap);
    return 1;
}
int
build_compression_model(t_C_block * block, t_C_stats * stats, t_C_model * model, t_bwriter * writer)
{

    free(stats->freqs);
    return 1;
}
int
run_compression_model(t_C_block * block, t_C_model * model, t_bwriter * writer)
{
    int i;
    elias_delta_encode(block->len, writer);
    for(i=0;i<block->len;i++)
    {
        elias_delta_encode(block->content[i], writer);
    }
    return 1;
}
int
get_decompression_statistics(t_D_stats * stats, t_breader * reader)
{
    return 1;
}
int
build_decompression_model(t_D_stats * stats, t_D_model * model, t_breader * reader)
{
    return 1;
}
int
run_decompression_model(t_D_model * model, t_breader * reader, t_iwriter * writer)
{
    uint32_t i, V, len;
    if(elias_delta_decode(&len, reader) == 0) return 0;
    for(i=0;i<len;i++)
    {
        if(elias_delta_decode(&V, reader) == 0) break;
        write_int(V, writer);
    }
    return 1;
}
void start_breader(t_breader * reader){
    reader->length = 0;
    reader->current = 0;
    reader->head = 0;
    reader->buffer_head = 0;
    reader->end = 0;
}
uint32_t get_bit(t_breader * reader, uint32_t * value){
    if(reader->head == 0){
        /* pull from buffer */
        if(reader->buffer_head == reader->length)
        {
            if(reader->end) return 0;
            reader->length = fread(reader->buffer, sizeof(uint32_t), BUFFER_SZ, stdin);
            if(reader->length != BUFFER_SZ) reader->end = 1;
            reader->buffer_head = 0;
        }
        if(reader->buffer_head == reader->length)
        {
            return 0;
        }
        else
        {
            reader->current = reader->buffer[reader->buffer_head++];
        }
        reader->head = 32;
    }
    if(reader->current & BIT_VALUES[reader->head--]) {
        *value = 1;
        return 1;
    }
    *value = 0;
    return 1;
}
void start_bwriter(t_bwriter * writer){
    writer->current = 0;
    writer->head = 32;
    writer->buffer_head = 0;
}
void write_bit(uint32_t b, t_bwriter * writer){
    if(b)
        writer->current += BIT_VALUES[writer->head];
    writer->head--;
    if(writer->head == 0)
    {
        writer->buffer[writer->buffer_head++] = writer->current;
        if(writer->buffer_head == BUFFER_SZ)
        {
            fwrite(writer->buffer, sizeof(uint32_t), writer->buffer_head, stdout);
            writer->buffer_head = 0;
        }
        writer->current = 0;
        writer->head = 32;
    }
}
void flush_bits(t_bwriter * writer)
{
    if(writer->head != 32)
        writer->buffer[writer->buffer_head++] = writer->current;
    fwrite(writer->buffer, sizeof(uint32_t), writer->buffer_head, stdout);
    writer->buffer_head = 0;
    writer->current = 0;
    writer->head = 32;
}
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
size_t get_file_size(FILE * stream){
    size_t off, sz;
    off = ftell(stream);
    fseek(stream, 0, SEEK_END);
    sz = ftell(stream);
    fseek(stream, off, SEEK_SET);
    if(off == -1 || sz == -1){
        fprintf(stderr, "File Size cannot be determined\n");
        exit(EXIT_FAILURE);
    }
    return sz;
}
int
atend(t_breader * reader)
{
    if(reader->end == 1 && reader->buffer_head == reader->length) return 0;
    return 1;
}
void
start_iwriter(t_iwriter * writer)
{
    writer->head = 0;
}
void
write_int(uint32_t i, t_iwriter * writer)
{
    writer->buffer[writer->head++] = i;
    if(writer->head == BUFFER_SZ) {
        fwrite(writer->buffer, sizeof(uint32_t), writer->head, stdout);
        writer->head = 0;
    }
}
void
flush_ints(t_iwriter * writer)
{
    if(writer->head) {
        fwrite(writer->buffer, sizeof(uint32_t), writer->head, stdout);
        writer->head = 0;
    }
}
uint32_t log2(uint32_t value){
    return log(value)/log(2);
}