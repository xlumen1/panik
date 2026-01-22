#include "panik.h"
#include <stdio.h>

int main(int argc, char** argv) {
    curl_global_init(0);
    pk_mode_t mode = MODE_NONE;
    char config_path[MAX_URL] = "/etc/panik";
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
            if (mode) return error("multiple modes specified");
            mode = MODE_SYNC;
            break;
        
        case 'u':
            if (mode) return error("multiple modes specified");
            mode = MODE_UPGRADE;
            break;
        
        case 'i':
            if (mode) return error("multiple modes specified");
            mode = MODE_INSTALL;
            break;

        case 'r':
            if (mode) return error("multiple modes specified");
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
        return error("Config path too long!\n");
    }

    char rpath[256];
    n = snprintf(rpath, sizeof(rpath), "%s/repos.conf", config_path);
    if (n < 0 || n >= (int)sizeof(rpath)) {
        return error("Config path too long!\n");
    }
    struct config config = getconfig(cpath);
    struct repos repos = getrepos(rpath);

    switch (mode)
    {
    case MODE_SYNC:
        return pk_sync(args, arg_count, config, repos);

    case MODE_UPGRADE:
        // pk_upgrade(args, arg_count)
        break;

    case MODE_INSTALL:
        if (arg_count < 1) {
            return error("No packages specified");
        }
        // pk_install(args, arg_count)
        break;

    case MODE_REMOVE:
        // pk_remove(args, arg_count)
        break;

    case MODE_NONE:
        return error("No mode specified");
    }
    
    return 0;
}
