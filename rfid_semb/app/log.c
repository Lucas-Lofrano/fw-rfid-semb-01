#include "lfs.h"
#include "AT45DB021E.h"
#include "log.h"

// Use packed struct to save space (4+1 bytes vs potential 8 bytes with padding)
struct __attribute__((packed)) log_entry_t {
    uint32_t tag_id;
    char op;
};
// Store strings in flash memory using static const
static const char LOG_FILE_PATH[] = "/tag_log.csv";
static const char LOG_HEADER[] = "tag_id,op\n";

lfs_t lfs;  // File system instance

// One-time hardware initialization (call this before first mount)
void flash_storage_init(void) {
    AT45DB_conf_bin();  // Essential for flash chip configuration
}

int log_init(void) {
    // Mount file system with proper initialization
    int err = lfs_mount(&lfs, &lfs_AT45DB_cfg);
    if (err < 0) return err;

    // Single file handle for all operations
    lfs_file_t file;

    // Try to open existing file
    err = lfs_file_open(&lfs, &file, LOG_FILE_PATH, LFS_O_RDWR | LFS_O_APPEND);

    if (err == LFS_ERR_NOENT) {
        // Create new file with header
        err = lfs_file_open(&lfs, &file, LOG_FILE_PATH,
                           LFS_O_WRONLY | LFS_O_CREAT);
        if (err) return err;

        // Write header and verify
        const int wrote = lfs_file_write(&lfs, &file, LOG_HEADER, sizeof(LOG_HEADER)-1);
        if (wrote != sizeof(LOG_HEADER)-1) {
            lfs_file_close(&lfs, &file);  // Clean up
            return LFS_ERR_IO;
        }
    }

    // Always close the file handle
    return lfs_file_close(&lfs, &file);
}

int log_append(uint32_t tag_id, char op) {
    lfs_file_t file;
    log_entry_t entry = {.tag_id = tag_id, .op = op};

    // Single write operation with atomic append
    int err = lfs_file_open(&lfs, &file, LOG_FILE_PATH,
                           LFS_O_WRONLY | LFS_O_APPEND | LFS_O_CREAT);
    if (err) return err;

    // Direct write without intermediate checks
    const int wrote = lfs_file_write(&lfs, &file, &entry, sizeof(entry));
    lfs_file_close(&lfs, &file);  // Always attempt close

    return (wrote == sizeof(entry)) ? 0 : LFS_ERR_IO;
}
