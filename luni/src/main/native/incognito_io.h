/*
 * Copyright (c) 2016 Tiramisu
 */
#ifndef INCOGNITO_IO_H_included
#define INCOGNITO_IO_H_included

enum File_Status {
	VALID = 0,
	DELETED
};

int Incognito_io_init();
void Incognito_io_stop();
int incognito_file_open(const char *pathname, int flags, int *path_set,
						char *incognito_file_path, int incog_pathname_sz, int *add_entry);
int add_file_entry(const char *original_filename, const char *new_filename,
				   File_Status status, int fd);

bool lookup_filename(const char *pathname, char *incognito_pathname,
					 size_t incog_pathname_sz);
#endif // INCOGNITO_IO_H_included
