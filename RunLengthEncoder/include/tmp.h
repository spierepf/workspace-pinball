#include <H.xbm>
uint8_t buf[sizeof(H_bits)];
size_t length = rle_compress(H_bits, sizeof(H_bits), buf);
printf("#define H_rle_length %li\n", length);
printf("const unsigned char H_rle_data[] PROGMEM = {");
