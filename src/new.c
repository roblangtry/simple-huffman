#include "new.h"

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

