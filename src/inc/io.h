#ifndef OCEAN_IO_H
#define OCEAN_IO_H

char* read_file(const char* filename);

void write_file(const char* filename, char* outbuffer);

char* sh(const char* cmd);

#endif
