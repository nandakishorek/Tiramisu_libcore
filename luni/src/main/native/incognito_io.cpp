#include "cutils/log.h"

#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


#define MAX_NUM_FDS_PER_FILE 32
#define MAX_FILE_PATH_SIZE 4096
#define MAX_FILES_PER_PROCESS 128

enum File_Status {
	VALID = 0,
	DELETED
};

struct OpenedFile {
	char original_filename[MAX_FILE_PATH_SIZE];
	char incog_filename[MAX_FILE_PATH_SIZE];
	File_Status status; 
	int fd_arr[MAX_NUM_FDS_PER_FILE];
	int fd_cnt;
};

struct IncognitoState {
	struct OpenedFile *opened_files;	
	int opened_files_cnt;
	int total_files_cnt;
};

static struct IncognitoState global_incognito_state;
static bool init_done = false;

int Incognito_io_init() {
	// Check if the global incognito state is already inited for the process.
    // If the state is inited, return.
	if (init_done) {
		ALOGE("Tiramisu: Incognito session for the app exists, restart the app to start a new incognito session");
		return 0;
	}

	// Allocate memory.
	global_incognito_state.opened_files = (struct OpenedFile *)
		calloc(MAX_FILES_PER_PROCESS, sizeof(struct OpenedFile));
	if (global_incognito_state.opened_files == NULL) {
		return ENOMEM;
	}
	global_incognito_state.total_files_cnt = MAX_FILES_PER_PROCESS;
	global_incognito_state.opened_files_cnt = 0;
	init_done = true;
	ALOGE("Tiramisu: Incognito state init successful");
	return 0;
}

void Incognito_io_stop() {
	free(global_incognito_state.opened_files);
	global_incognito_state.total_files_cnt = 0;
	global_incognito_state.opened_files_cnt = 0;
	init_done = false;
	ALOGE("Tiramisu: Incognito state deinit successful");
	return;
}

inline int get_new_filename(char *old_filename, char *new_filename,
                      size_t new_filename_size) {
    strcpy(new_filename, "INCOGNITO_TIRAMISU_");
    int len = strlen(new_filename);
    int old_file_len = strlen(old_filename);

    // These checks can be disabled if performance is an issue.
    if ((old_file_len + len + 1) > (int) new_filename_size) {
        ALOGE("Tiramisu: Error: Not able to generate new filename, buffer too small");
        return ENOMEM;
    }

    strcpy(new_filename+len, old_filename);

    return 0;
}

inline int get_new_file_path(char *directory_name, char *filename,
    char *path_buf, size_t path_buf_size) {

    int dirlen = strlen(directory_name);
    int filelen = strlen(filename);

    // Check if buffer is big enough.
    if ((dirlen + filelen + 1) > (int) path_buf_size) {
        //ALOGE("Tiramisu: Error: Not able to generate new path, buffer too small");
        return ENOMEM;
    }

    // Copy directory path.
    strcpy(path_buf, directory_name);

    strcpy(path_buf+dirlen, "/");

    // Copy filename to path.
    strcpy(path_buf+dirlen+1, filename);

    return 0;
}

template <typename T>
void ignore_var(T &&) {
}

int make_file_copy(char *original_filename, char *copy_filename) {
	ignore_var(original_filename);
	ignore_var(copy_filename);
	return 0;
}

int incognito_file_open(char *filename, int flags, int mode) {
	// Make a copy of the original file only the file is being opened in
	// append mode
	ignore_var(filename);
	ignore_var(flags);
	ignore_var(mode);
	return 0;
}

void incognito_file_close(int fd) {
	ALOGE("Close system call for fd %d", fd);
}

