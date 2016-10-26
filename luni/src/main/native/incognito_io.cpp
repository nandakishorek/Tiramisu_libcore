#include "cutils/log.h"

#include <stdlib.h>
#include <errno.h>

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
