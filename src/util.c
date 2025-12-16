#include "panik.h"
#include "tomlc17/toml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void show_help() {
    printf("Panik Help\n");
}

void error(char* e) {
    fprintf(stderr, "%s\n", e);
}

struct config getconfig(char path[256]) {
    struct config out = {0};

    FILE* fp = fopen(path, "r");
    if (!fp) {
        error("Config file not found");
        exit(404);
    }

    toml_result_t r =  toml_parse_file(fp);
    fclose(fp);

    if (!r.ok) {
        error(r.errmsg);
    }

    toml_datum_t root = toml_seek(r.toptab, "system.root");
    toml_datum_t arch = toml_seek(r.toptab, "system.arch");
    toml_datum_t cache = toml_seek(r.toptab, "system.cache");
    toml_datum_t local = toml_seek(r.toptab, "system.local");

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

    out.color = color.u.boolean;
    out.confirm = confirm.u.boolean;
    out.verbose = verbose.u.boolean;

    out.retries = retries.u.int64;
    out.timeout = timeout.u.int64;

    return out;
}

struct repos getrepos(char path[256]) {
    struct repos repos = {};
    FILE* fp = fopen(path, "r");
    if (!fp) {
        error("Config file not found");
        exit(404);
    }

    char line[512];
    repos.count = 0;

    int ln = 0;
    while (fgets(line, sizeof(line), fp)) {
        ln++;
        if (line[0] == '#' || line[0] == '\n') continue;

        int c = strcspn(line, "#");
        int e = strcspn(line, "\r\n");
        for (int i = c; c <= e; c++) {
            line[i] = 0;
        }

        char name[MAX_NAME];
        char url[MAX_URL];

        if (sscanf(line, "%31s %255s", name, url) != 2) {
            fprintf(stderr, "Malformed line #%d in %s", ln, path);
            continue;
        }

        if (repos.count < 32) {
            snprintf(repos.repositories[repos.count].name, sizeof(name), "%s", name);
            snprintf(repos.repositories[repos.count].path, sizeof(url), "%s", url);
            repos.count++;
        } else {
            error("Too many repositories defined!");
            break;
        }
    }

    fclose(fp);
    return repos;
}
