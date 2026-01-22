#include "panik.h"
#include "tomlc17/toml.h"
#include <stdio.h>

int hash(char *str) {
    unsigned long hash = 5381;
    unsigned char *s = (unsigned char*)str;
    int c;

    while ((c = *s++))
        hash = ((hash << 5) + hash) + c;

    return (int)(long)(void*)hash;
}

void show_help() {
    printf("Panik Help\n");
}

int error(char* e) {
    fprintf(stderr, "%s\n", e);
    return hash(e);
}

struct config getconfig(char path[256]) {
    struct config out = {0};

    FILE* fp = fopen(path, "r");
    if (!fp) {
        exit(error("Config file not found"));
    }

    toml_result_t r =  toml_parse_file(fp);
    fclose(fp);

    if (!r.ok) {
        exit(error(r.errmsg));
    }

    toml_datum_t root = toml_seek(r.toptab, "system.root");
    toml_datum_t arch = toml_seek(r.toptab, "system.arch");
    toml_datum_t cache = toml_seek(r.toptab, "system.cache");
    toml_datum_t local = toml_seek(r.toptab, "system.local");
    toml_datum_t temp = toml_seek(r.toptab, "system.temp");

    toml_datum_t color = toml_seek(r.toptab, "behavior.color");
    toml_datum_t confirm = toml_seek(r.toptab, "behavior.confirm");
    toml_datum_t verbose = toml_seek(r.toptab, "behavior.verbose");
    toml_datum_t retries = toml_seek(r.toptab, "behavior.retries");
    toml_datum_t timeout = toml_seek(r.toptab, "behavior.timeout");

    toml_free(r);

    snprintf(out.root, sizeof(root.u.s), "%s", root.u.s);
    snprintf(out.arch, sizeof(arch.u.s), "%s", arch.u.s);
    snprintf(out.cache, sizeof(cache.u.s), "%s", cache.u.s);
    snprintf(out.local, sizeof(local.u.s), "%s", local.u.s);
    snprintf(out.temp, sizeof(temp.u.s), "%s", temp.u.s);

    out.color = color.u.boolean;
    out.confirm = confirm.u.boolean;
    out.verbose = verbose.u.boolean;

    out.retries = retries.u.int64;
    out.timeout = timeout.u.int64;

    return out;
}

struct repos getrepos(char path[256]) {
    struct repos repos = {0};
    FILE* fp = fopen(path, "r");
    if (!fp) {
        exit(error("Config file not found"));
    }

    char line[512];
    repos.count = 0;

    int ln = 0;
    while (fgets(line, sizeof(line), fp)) {
        ln++;
        if (line[0] == '#' || line[0] == '\n') continue;

        char *hash = strchr(line, '#');
        if (hash)
            *hash = '\0';
        
        line[strcspn(line, "\r\n")] = '\0';

        char name[MAX_NAME];
        char url[MAX_URL];

        if (sscanf(line, "%31s %255s", name, url) != 2) {
            fprintf(stderr, "Malformed line #%d in %s", ln, path);
            continue;
        }

        if (repos.count < 32) {
            snprintf(repos.repositories[repos.count].name, sizeof(repos.repositories[repos.count].name), "%s", name);
            snprintf(repos.repositories[repos.count].path, sizeof(repos.repositories[repos.count].path), "%s", url);
            repos.count++;
        } else {
            exit(error("Too many repositories defined!"));
            break;
        }
    }

    fclose(fp);
    return repos;
}

size_t write_file_cb(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    FILE* f = stream;
    return fwrite(ptr, size, nmemb, f);
}


int ensure_dir(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode))
            return 0;
        return -1;
    }
    return mkdir(path, 0755);
}
