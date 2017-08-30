#include "new.h"
uint32_t * global_hack;
int run_new_compression(){
    C_block_t * block = malloc(sizeof(C_block_t));
    t_bwriter * writer = malloc(sizeof(t_bwriter));
    start_bwriter(writer);
    while((block->len = fread(block->content, sizeof(uint32_t), BLOCK_SIZE, stdin)) > 0){
        binary_encode(MAGIC, MAGIC_LENGTH, writer);
        mr_encode_block(block, writer);
    }
    flush_bits(writer);
    return 0;
}
int run_new_decompression(){
    t_breader * reader = malloc(sizeof(t_breader));
    t_iwriter * writer = malloc(sizeof(t_iwriter));
    uint32_t V;
    io_back backfeed;
    start_breader(reader);
    start_iwriter(writer);
    backfeed.val = 0;
    backfeed.len = 0;
    while(atend(reader)){
        if(binary_decode(&V,MAGIC_LENGTH - backfeed.len, reader) == 0) break;
        V = V + (backfeed.val << (MAGIC_LENGTH - backfeed.len));
        if(V != MAGIC) break;
        if(mr_decode_block(reader, writer, &backfeed) == 0) break;
    }
    flush_ints(writer);
    return 0;
}


int syms_used_cmpfunc (const void * a, const void * b)
{
   return ( global_hack[*(uint32_t *)b] - global_hack[*(uint32_t *)a] );
}
int syms_used_cmpfunc2 (const void * a, const void * b)
{
   return ( *(uint32_t*)a - *(uint32_t*)b );
}
void mr_encode_block(C_block_t * block, t_bwriter * writer)
{
    uint32_t n = 0;
    uint32_t i, x, L, nmax, m, xmax = 0, j = 0;
    uint32_t * M = block->content - 1;
    uint32_t * table0 = calloc(SYMBOL_MAP_SIZE, sizeof(uint32_t));
    uint32_t * table = table0 - 1;
    uint32_t * syms_used0 = malloc(BLOCK_SIZE * sizeof(uint32_t));
    uint32_t * syms_used1 = malloc(BLOCK_SIZE * sizeof(uint32_t));
    uint32_t * syms_used = syms_used0 - 1;
    uint32_t * spare;
    uint32_t * w;
    uint32_t * base;
    uint32_t * offset;
    uint32_t add, sym, code_len;
    m = block->len;
    for(i = 1; i <= m; i++)
    {
        x = M[i];
        if(x > xmax) xmax = x;
        if(table[x] == 0){
            n++;
            syms_used[n] = x;
        }
        table[x] += 1;
    }
    for(i=1;i<=xmax;i++)
        if(table[i])
            syms_used1[j++] = i;
    global_hack = table;
    qsort(syms_used+1, n, sizeof(uint32_t), syms_used_cmpfunc);
    spare = malloc(n * sizeof(uint32_t));
    for(i=1;i<=n;i++)
        spare[i-1] = table[syms_used[i]];
    calculate_huffman_code(spare, n);
    for(i=1;i<=n;i++)
        table[syms_used[i]] = spare[i-1];
    L = table[syms_used[n]];
    syms_used = syms_used1 - 1;
    /* qsort(syms_used+1, n, sizeof(uint32_t), syms_used_cmpfunc2); */
    nmax = syms_used[n];
    w = calloc(L+2, sizeof(uint32_t));
    base = malloc((L+2) * sizeof(uint32_t));
    offset = malloc((L+2) * sizeof(uint32_t));
    for(i=1;i<=n;i++){
        w[table[syms_used[i]]]++;
    }
    base[1] = 0;
    offset[1] = 1;
    offset[L+1] = n + 1;
    for(i=2; i <= L; i++)
    {
        base[i] = (base[i-1] + w[i-1]) << 1;
        offset[i] = offset[i-1] + w[i-1];
    }
    elias_delta_encode(m, writer);
    elias_delta_encode(nmax, writer);
    elias_delta_encode(n, writer);
    elias_delta_encode(L, writer);
    /* interp_encode(syms_used, n); */

    /* -- snip -- */
    add = 0;
    for (i=1;i<=n;i++){
        elias_delta_encode(syms_used[i] - add,writer);
        add = syms_used[i];
    }
    /* -- snip -- */
    for(i=1;i<=n;i++){
        unary_encode((L+1-table[syms_used[i]]),writer);
    }
    for(i=2;i<=L;i++)
        w[i]=offset[i];
    for(i=1;i<=n;i++){
        sym = syms_used[i];
        code_len = table[sym];
        table[sym] = w[code_len];
        w[code_len] = w[code_len] + 1;
    }
    for(i=1;i<=m;i++){
        canonical_encode(table[M[i]], base, offset, writer);
    }
}
void calculate_huffman_code(uint32_t * P0, uint32_t n)
{
    uint32_t * P = P0 - 1;
    uint32_t r,s,a,u,d,x;
    r = n;
    s = n;
    for(x=n;x>=2;x--)
    {
        if((s < 1) || (r > x && P[r] < P[s]))
        {
            P[x] = P[r];
            P[r] = x;
            r--;
        }
        else
        {
            P[x] = P[s];
            s--;
        }
        if((s < 1) || (r > x && P[r] < P[s]))
        {
            P[x] = P[x] + P[r];
            P[r] = x;
            r--;
        }
        else
        {
            P[x] = P[x] + P[s];
            s--;
        }
    }
    if(n > 1) P[2] = 0;
    for(x=3; x<=n; x++)
    {
        P[x] = P[P[x]] + 1;
    }
    a = 1;
    u = 0;
    d = 0;
    r = 2;
    x = 1;
    while(a > 0)
    {
        while(r<=n && P[r] == d)
        {
            u++;
            r++;
        }
        while(a > u)
        {
            P[x] = d;
            x++;
            a--;
        }
        a = u << 1;
        d++;
        u = 0;
    }
}
void canonical_encode(uint32_t symbol, uint32_t * base, uint32_t * offset, t_bwriter * writer){
    uint32_t value, length = 1;
    while(symbol >= offset[length+1])
    {
        length++;
    }
    value = symbol - offset[length] + base[length];
    binary_encode(value, length, writer);
}
uint32_t canonical_decode(uint32_t * x, uint32_t * V, uint32_t L, uint32_t * base, uint32_t * offset, uint32_t * lj_limit, t_breader * reader)
{
    uint32_t l = 1, value, c;
    while(*V >= lj_limit[l])
        l++;
    c = *V >> (L - l);
    *V = *V - (c << (L-l));
    *x = c - base[l] + offset[l];
    if(binary_decode(&value, l, reader) == 0){
        *V = *V << l;
        return 0;
    }
    *V = (*V << l) + value;
    return 1;
}
uint32_t mr_decode_block(t_breader * reader, t_iwriter * writer, io_back * backfeed)
{
    uint32_t m, nmax, n, L, i, sym, code_len, c, from, temp, V, start, add;
    uint32_t * syms_used;
    uint32_t * syms_used0;
    uint32_t * table;
    uint32_t * table0;
    uint32_t * w;
    uint32_t * base;
    uint32_t * offset;
    uint32_t * lj_limit;
    if(elias_delta_decode(&m, reader) == 0) return 0;
    if(elias_delta_decode(&nmax, reader) == 0) return 0;
    if(elias_delta_decode(&n, reader) == 0) return 0;
    if(elias_delta_decode(&L, reader) == 0) return 0;
    syms_used0 = malloc(n * sizeof(uint32_t));
    syms_used = syms_used0 - 1;
    table0 = malloc(n * sizeof(uint32_t));
    table = table0 - 1;
    w = calloc((L+2), sizeof(uint32_t));
    base = calloc((L+2), sizeof(uint32_t));
    offset = calloc((L+2), sizeof(uint32_t));
    lj_limit = calloc((L+2), sizeof(uint32_t));
    /* interp_decode */
    /* -- snip -- */
    add = 0;
    for(i=1;i<=n;i++){
        if(elias_delta_decode(&syms_used[i], reader) == 0) return 0;
        syms_used[i] += add;
        add = syms_used[i];
    }
    /* -- snip -- */ 
    for(i=1;i<=n;i++){
        if(unary_decode(&table[i], reader) == 0) return 0;
        table[i] = L + 1 - table[i];
        w[table[i]]++;
    }
    /* build tables */
    for(i=1;i<=L;i++)
    {
        if(i == 1)
            base[i] = 0;
        else
            base[i] = (base[i - 1] + w[i - 1]) << 1;
        if(i == 1)
            offset[i] = 1;
        offset[i+1] = offset[i] + w[i];
    }
    for(i=1;i<=L;i++)
    {
        if(i == L)
            lj_limit[i] = 1 << L;
        else
            lj_limit[i] = base[i+1] << (L-i-1);
        /*fprintf(stderr, "[%d] => %u { %u } %u\n", i, lj_limit[i], base[i], offset[i]);*/
    }
    /* done */

    for(i=2;i<=L;i++)
    {
        w[i] = offset[i];
    }
    for(i=1;i<=n;i++)
    {
        sym = syms_used[i];
        code_len = table[i];
        table[i] = w[code_len];
        w[code_len]++;
    }
    start = 1;
    while(start <= n)
    {
        from = start;
        sym = syms_used[start];
        while(table[from] != DONE)
        {
            i = table[from];
            table[from] = DONE;
            temp = syms_used[i];
            syms_used[i] = sym;
            sym = temp;
            from = i;
        }
        while(start <= n && table[start] == DONE)
            start++;
    }
    if(binary_decode(&V, L, reader) == 0) return 0;
    for(i=0;i<m;i++){
        canonical_decode(&c, &V, L, base, offset, lj_limit, reader);
        write_int(syms_used[c], writer);
    }
    backfeed->len = L;
    backfeed->val = V;
    return 1;
}

