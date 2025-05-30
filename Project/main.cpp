#define FUSE_USE_VERSION 31
#include <fuse3/fuse.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <ctime>
#include <string>
#include <iostream>
#include <algorithm>
#include <openssl/sha.h>
#include <openssl/evp.h>

struct Superblock {
    int block_size = 512;
    int total_blocks = 2048;
    int free_blocks = 2048;
    int inode_count = 0;
};

struct Inode {
    mode_t mode;
    size_t size;
    time_t created, modified;
    std::vector<int> blocks;
};

const int BLOCK_SIZE = 512;
const int TOTAL_BLOCKS = 2048;

std::vector<std::vector<char>> disk(TOTAL_BLOCKS, std::vector<char>(BLOCK_SIZE));
std::vector<bool> block_used(TOTAL_BLOCKS, false);
Superblock sb;
std::unordered_map<std::string, Inode> inode_table;

std::string sha256(const std::string &str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char *)str.c_str(), str.size(), hash);
    char output[65];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        sprintf(output + i * 2, "%02x", hash[i]);
    output[64] = 0;
    return std::string(output);
}

bool check_password() {
    std::string input;
    std::cout << "Enter password: ";
    std::cin >> input;

    std::string hash = sha256(input);
    const std::string stored_hash = "4f49a49ad55d07205ebd49098c3f4bc8c53494ff8614c1568a004fe78a31b350"; // "myfspass"

    return hash == stored_hash;
}

int allocate_block() {
    for (int i = 0; i < TOTAL_BLOCKS; ++i) {
        if (!block_used[i]) {
            block_used[i] = true;
            sb.free_blocks--;
            return i;
        }
    }
    return -1;
}

static int myfs_utimens(const char *path, const struct timespec tv[2],
                        struct fuse_file_info *fi) {
    auto it = inode_table.find(path);
    if (it == inode_table.end()) return -ENOENT;

    it->second.modified = tv[1].tv_sec;
    return 0;
}

const std::string encryption_key = "nyuta";

std::string encrypt(const std::string &originalData) {
    std::string encryptedData= originalData;
    for (size_t i = 0; i < originalData.size(); ++i) {
        encryptedData[i] ^= encryption_key[i % encryption_key.size()];
    }
    return encryptedData;
}

std::string decrypt(const std::string &encryptedData) {
    return encrypt(encryptedData);
}


static int myfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
    memset(stbuf, 0, sizeof(struct stat));

    if (std::string(path) == "/") {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }

    auto it = inode_table.find(path);
    if (it == inode_table.end()) return -ENOENT;

    const Inode &inode = it->second;
    stbuf->st_mode = inode.mode;
    stbuf->st_nlink = 1;
    stbuf->st_size = inode.size;
    stbuf->st_ctime = inode.created;
    stbuf->st_mtime = inode.modified;
    return 0;
}

static int myfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                        off_t offset, struct fuse_file_info *fi,
                        enum fuse_readdir_flags flags) {
    if (std::string(path) != "/") return -ENOENT;

    filler(buf, ".", NULL, 0, FUSE_FILL_DIR_PLUS);
    filler(buf, "..", NULL, 0, FUSE_FILL_DIR_PLUS);

    for (const auto &entry : inode_table) {
        std::string name = entry.first.substr(1);
        filler(buf, name.c_str(), NULL, 0, FUSE_FILL_DIR_PLUS);
    }
    return 0;
}
static int myfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
    if (inode_table.count(path)) return -EEXIST;

    Inode inode;
    inode.mode = S_IFREG | 0644;
    inode.size = 0;
    inode.created = inode.modified = time(nullptr);

    inode_table[path] = inode;
    sb.inode_count++;
    return 0;
}

static int myfs_open(const char *path, struct fuse_file_info *fi) {
    if (!inode_table.count(path)) return -ENOENT;
    return 0;
}


static int myfs_read(const char *path, char *buf, size_t size, off_t offset,
                     struct fuse_file_info *fi) {
    auto it = inode_table.find(path);
    if (it == inode_table.end()) return -ENOENT;

    Inode &inode = it->second;
    size_t bytes_read = 0;

    while (bytes_read < size && offset < inode.size) {
        int block_index = offset / BLOCK_SIZE;
        int block_offset = offset % BLOCK_SIZE;

        if (block_index >= inode.blocks.size()) break;

        int block_no = inode.blocks[block_index];
        size_t to_read = std::min(BLOCK_SIZE - block_offset, static_cast<int>(size - bytes_read));

        memcpy(buf + bytes_read, &disk[block_no][block_offset], to_read);

        bytes_read += to_read;
        offset += to_read;
    }
    std::string encrypted(buf, bytes_read);
    std::string decrypted = decrypt(encrypted);
    memcpy(buf, decrypted.c_str(), decrypted.size());
    return decrypted.size();
}

static int myfs_unlink(const char *path) {
    auto it = inode_table.find(path);
    if (it == inode_table.end()) return -ENOENT;

    for (int block : it->second.blocks) {
        block_used[block] = false;
        sb.free_blocks++;
    }

    inode_table.erase(it);
    sb.inode_count--;
    return 0;
}

static int myfs_write(const char *path, const char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi) {
    auto it = inode_table.find(path);
    if (it == inode_table.end()) return -ENOENT;

    Inode &inode = it->second;
    size_t bytes_written = 0;
    std::string originalData(buf, size);
    std::string encrypted = encrypt(originalData.c_str());
    while (bytes_written < encrypted.size()) {
        int block_index = (offset + bytes_written) / BLOCK_SIZE;
        int block_offset = (offset + bytes_written) % BLOCK_SIZE;

        if (block_index >= inode.blocks.size()) {
            int new_block = allocate_block();
            if (new_block == -1) return -ENOSPC;
            inode.blocks.push_back(new_block);
        }

        int block_no = inode.blocks[block_index];
        size_t to_write = std::min(BLOCK_SIZE - block_offset, static_cast<int>(encrypted.size() - bytes_written));
        memcpy(&disk[block_no][block_offset], encrypted.c_str() + bytes_written, to_write);
        bytes_written += to_write;
    }

    inode.size = std::max(inode.size, (size_t)(offset + bytes_written));
    inode.modified = time(nullptr);
    return bytes_written;
}

static struct fuse_operations myfs_oper = {0};

int main(int argc, char *argv[]) {

    if (!check_password()) {
        std::cerr << "Access denied: wrong password." << std::endl;
        return 1;
    }
    myfs_oper.utimens = myfs_utimens;
    myfs_oper.getattr = myfs_getattr;
    myfs_oper.readdir = myfs_readdir;
    myfs_oper.open = myfs_open;
    myfs_oper.read = myfs_read;
    myfs_oper.write = myfs_write;
    myfs_oper.create = myfs_create;
    myfs_oper.unlink = myfs_unlink;

    return fuse_main(argc, argv, &myfs_oper, NULL);
}
