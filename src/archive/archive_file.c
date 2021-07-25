#include <pulsar/archive/archive_file.h>

#ifdef __SWITCH__
// Path concatenation borrowed from newlib
int _concatenate_path(int* errno, char *path, const char *extra, int maxLength);
#endif

static bool _concatPath(char* base, const char* dest, size_t size) {
	if(base == NULL || size == 0 || dest == NULL) {
		return false;
	}

#ifdef __SWITCH__
	// If the dest path contains a device name
	// copy it to the output first, and concat it with the rest of the path
	const char* destDevice = strchr(dest, ':');
	if(destDevice != NULL) {
		destDevice++;
		if(*destDevice != '/' || destDevice - dest >= size) {
			return false;
		}

		size_t deviceRootLen = destDevice - dest + 1;
		if(deviceRootLen >= size) {
			return false;
		}

		strncpy(base, dest, deviceRootLen);
		base[deviceRootLen] = '\0';
		dest = destDevice;
	} else if (*dest == '/' && size >= 2) {
		// Root path on the switch can be on a different device than the CWD
		// avoid it appending the wrong device by removing it in the base
		strcpy(base, "/");
	}

	int localErrno;
	return _concatenate_path(&localErrno, base, dest, size) == 0;
#else
	const char* resolved;
	if(*dest == '/') {
		resolved = realpath(dest, NULL);
	} else {
		size_t baseLen = strlen(base);
		if(baseLen >= size) {
			return false;
		}

		snprintf(base+baseLen, size-baseLen, "/%s", dest);
		resolved = realpath(base, NULL);
	}

	if(resolved == NULL) {
		return false;
	}

	base[size-1] = '\0';
	strncpy(base, resolved, size);
	free(resolved);
	return base[size-1] == '\0';
#endif
}

static char* _getContextPath(const char* path) {
	char* context = (char*)malloc(FS_MAX_PATH);

	if(context == NULL) {
		return NULL;
	}

	if(
		getcwd(context, FS_MAX_PATH) == NULL
		|| !_concatPath(context, path, FS_MAX_PATH)
		|| !_concatPath(context, "..", FS_MAX_PATH)
	) {
		free(context);
		return NULL;
	}

	return context;
}

PLSR_ArchiveFileHandle plsrArchiveFileOpen(const char* path, bool storePath) {
	FILE* f = fopen(path, "r");
	if(f == NULL) {
		return PLSR_INVALID_ARCHIVE_FILE_HANDLE;
	}

	PLSR_ArchiveSharedReader* reader = (PLSR_ArchiveSharedReader*)malloc(sizeof(PLSR_ArchiveSharedReader));

	if(reader == NULL) {
		fclose(f);
		return PLSR_INVALID_ARCHIVE_FILE_HANDLE;
	}

	reader->context = storePath ? _getContextPath(path) : NULL;
	reader->f = f;
	reader->refs = 1;

	return reader;
}

void plsrArchiveFileClose(PLSR_ArchiveFileHandle handle) {
	if(!handle) {
		return;
	}

	PLSR_ArchiveSharedReader* reader = (PLSR_ArchiveSharedReader*)handle;
	reader->refs--;
	if(reader->refs == 0) {
		fclose(reader->f);

		if(reader->context != NULL) {
			free(reader->context);
		}

		free(reader);
	}
}

PLSR_ArchiveFileHandle plsrArchiveFileCloneHandle(PLSR_ArchiveFileHandle handle) {
	if(handle) {
		PLSR_ArchiveSharedReader* reader = (PLSR_ArchiveSharedReader*)handle;
		reader->refs++;
	}

	return handle;
}

bool plsrArchiveFileRelativePath(PLSR_ArchiveFileHandle handle, const char* path, char* out, size_t size) {
	if(!handle || out == NULL || size == 0) {
		return false;
	}

	PLSR_ArchiveSharedReader* reader = (PLSR_ArchiveSharedReader*)handle;

	strncpy(out, reader->context, size);
	out[size-1] = '\0';

	return _concatPath(out, path, size);
}
