#include "bitlevel_io.h"
struct bitlevel_file_pointer * get_bitlevel_file_pointer(FILE * file_pointer){
    struct bitlevel_file_pointer * bitlevel_file_pointer;
    bitlevel_file_pointer = (struct bitlevel_file_pointer *)malloc(sizeof(struct bitlevel_file_pointer));
    bitlevel_file_pointer->file_pointer = file_pointer;
    bitlevel_file_pointer->buffer = 0;
    bitlevel_file_pointer->current_length = 0;
    return bitlevel_file_pointer;
}
size_t bitlevel_write(struct bitlevel_file_pointer * bitlevel_file_pointer, struct bitlevel_object write_object){
    int bits_from_lbyte;
    int bits_from_rbyte;
    size_t no_bytes_written;
    unsigned char byte;
    int lbyte, rbyte, bat;
    if ((bitlevel_file_pointer->current_length + write_object.length) >= 8){
        bits_from_lbyte = bitlevel_file_pointer->current_length;
        bits_from_rbyte = 8 - bits_from_lbyte;
        lbyte = ((bitlevel_file_pointer->buffer) << bits_from_rbyte);
        rbyte = (unsigned char)(write_object.value >> (write_object.length - bits_from_rbyte));
        bat = (lbyte + rbyte);
        byte = (unsigned char)bat;
        write_object.value = write_object.value - (rbyte << (write_object.length - bits_from_rbyte));
        write_object.length = write_object.length - bits_from_rbyte;
        fwrite(&byte, sizeof(unsigned char), 1, bitlevel_file_pointer->file_pointer);
        no_bytes_written = 1;
        while(write_object.length >= 8){
            no_bytes_written = no_bytes_written + 1;
            byte = write_object.value >> (write_object.length - 8);
            fwrite(&byte, sizeof(unsigned char), 1, bitlevel_file_pointer->file_pointer);
            write_object.value = write_object.value - (byte << (write_object.length - 8));
            write_object.length = write_object.length - 8;
        }
        bitlevel_file_pointer->buffer = write_object.value;
        bitlevel_file_pointer->current_length = write_object.length;
    } else{
        no_bytes_written = 0;
        bitlevel_file_pointer->buffer = (bitlevel_file_pointer->buffer << write_object.length) + write_object.value;
        bitlevel_file_pointer->current_length = bitlevel_file_pointer->current_length + write_object.length;
    }
    return no_bytes_written;
}
struct bitlevel_object bitlevel_read(struct bitlevel_file_pointer * bitlevel_file_pointer, int length){
    struct bitlevel_object bitlevel_object;
    unsigned char byte;
    bitlevel_object.length = length;
    while(length > bitlevel_file_pointer->current_length){
        fread(&byte, sizeof(unsigned char), 1, bitlevel_file_pointer->file_pointer);
        bitlevel_file_pointer->buffer = (bitlevel_file_pointer->buffer << 8) + (int)byte;
        bitlevel_file_pointer->current_length = bitlevel_file_pointer->current_length + 8;
    }
    bitlevel_object.value = bitlevel_file_pointer->buffer >> (bitlevel_file_pointer->current_length - length);
    bitlevel_file_pointer->buffer = bitlevel_file_pointer->buffer - (bitlevel_object.value << (bitlevel_file_pointer->current_length - length));
    bitlevel_file_pointer->current_length = bitlevel_file_pointer->current_length - length;
    return bitlevel_object;
}
size_t bitlevel_flush(struct bitlevel_file_pointer * bitlevel_file_pointer){
        unsigned char byte;
        size_t no_bytes_written;
        no_bytes_written = 0;
        while(bitlevel_file_pointer->current_length >= 8){
            no_bytes_written = no_bytes_written + 1;
            byte = bitlevel_file_pointer->buffer >> (bitlevel_file_pointer->current_length - 8);
            fwrite(&byte, sizeof(unsigned char), 1, bitlevel_file_pointer->file_pointer);
            bitlevel_file_pointer->current_length = bitlevel_file_pointer->current_length - 8;
            bitlevel_file_pointer->buffer = bitlevel_file_pointer->buffer - (byte << (bitlevel_file_pointer->current_length - 8));
        }
        if (bitlevel_file_pointer->current_length > 0){
            byte = (unsigned char)(bitlevel_file_pointer->buffer << (8 - bitlevel_file_pointer->current_length));
            fwrite(&byte, sizeof(unsigned char), 1, bitlevel_file_pointer->file_pointer);
            no_bytes_written = no_bytes_written + 1;
            bitlevel_file_pointer->current_length = 0;
            bitlevel_file_pointer->buffer = 0;
        }
        return no_bytes_written;
}