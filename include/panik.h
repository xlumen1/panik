#pragma once
#include <getopt.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#define MAX_NAME 32
#define MAX_URL 256

static const struct option long_opts[] = {
    {"help",    no_argument,       0, 'h'},
    {"root",    required_argument, 0, 'R'},
    {"config",  required_argument, 0, 'C'},
    {"sync",    no_argument,       0, 's'},
    {"install", no_argument,       0, 'i'},
    {"remove",  no_argument,       0, 'r'},
    {"upgrade", no_argument,       0, 'u'},
    {0, 0, 0, 0}
};

typedef enum {
    MODE_NONE = 0,
    MODE_SYNC,
    MODE_INSTALL,
    MODE_REMOVE,
    MODE_UPGRADE
} pk_mode_t;

struct config {
    char root[MAX_URL];
    char arch[32];
    char cache[MAX_URL];
    char local[MAX_URL];
    char temp[MAX_URL];

    int color;
    int confirm;
    int verbose;

    int retries;
    int timeout;
};

struct repo {
        char name[MAX_NAME];
        char path[MAX_URL];
};

struct repos {
    int count;
    struct repo repositories[32];
};

extern struct config config;

void show_help();
int error(char* e);

struct config getconfig(char path[256]);
struct repos getrepos(char path[256]);

size_t write_file_cb(void* ptr, size_t size, size_t nmemb, FILE* stream);
int ensure_dir(const char* path);


int pk_sync(char** argv, int argc, struct repos repos);

