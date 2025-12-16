#include "panik.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    pk_mode_t mode = MODE_NONE;
    char config_path[MAX_URL] = "/etc/panik/panik.toml";
    char repository[MAX_NAME] = "";

    int opt;
    while ((opt = getopt_long(argc, argv, "hR:C:suir", long_opts, NULL)) != -1) {
        switch (opt) {
        case 'h':
            show_help();
            return 0;
        
        case 'R':
            strncpy(repository, optarg, sizeof(repository) - 1);
            repository[sizeof(repository) - 1] = '\0';
            break;
        
        case 'C':
            strncpy(config_path, optarg, sizeof(config_path) - 1);
            config_path[sizeof(config_path) - 1] = '\0';
            break;

        case 's':
            if (mode) error("multiple modes specified");
            mode = MODE_SYNC;
            break;
        
        case 'u':
            if (mode) error("multiple modes specified");
            mode = MODE_UPGRADE;
            break;
        
        case 'i':
            if (mode) error("multiple modes specified");
            mode = MODE_INSTALL;
            break;

        case 'r':
            if (mode) error("multiple modes specified");
            mode = MODE_REMOVE;
            break;

        default:
            break;
        }
    }

    char** args = &argv[optind];
    int arg_count = argc - optind;

    char cpath[256];
    int n = snprintf(cpath, sizeof(cpath), "%s/panik.toml", config_path);
    if (n < 0 || n >= (int)sizeof(cpath)) {
        fprintf(stderr, "config path too long!\n");
        return 1;
    }

    char rpath[256];
    n = snprintf(rpath, sizeof(rpath), "%s/repos.conf", config_path);
    if (n < 0 || n >= (int)sizeof(rpath)) {
        fprintf(stderr, "config path too long!\n");
        return 1;
    }
    struct config config = getconfig(cpath);
    struct repos repos = getrepos(rpath);
    printf("Arch=%s\n", config.arch);
    printf("Verbose? %s\n", config.verbose ? "Yes" : "No");
    for (int i = 0; i < repos.count; i++) {
        printf("%s: %s\n", repos.repositories[i].name, repos.repositories[i].path);
    }

    switch (mode)
    {
    case MODE_SYNC:
        return pk_sync(args, arg_count, config, repos);

    case MODE_UPGRADE:
        // pk_upgrade(args, arg_count)
        break;

    case MODE_INSTALL:
        if (arg_count < 1) {
            error("No packages specified");
            return -1;
        }
        // pk_install(args, arg_count)
        break;

    case MODE_REMOVE:
        // pk_remove(args, arg_count)
        break;

    case MODE_NONE:
        error("no mode specified");
        return -1;
    }
    
    return 0;
}
