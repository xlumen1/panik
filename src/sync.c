#include "panik.h"
#include <string.h>

int pk_sync(char** argv, int argc, struct config config, struct repos repos) {
    int repo_index = 0;
    int repo_queue[32] = {-1};

    if (argc == 0) {
       for (int j = 0; j < repos.count; j++) {
            repo_queue[repo_index] = j;
        } 
    } else {
        for (int i = 0; i < argc; i++) {
            for (int j = 0; j < repos.count; j++) {
                if (strcmp(repos.repositories[j].name, argv[i]) == 0) {
                    repo_queue[repo_index] = j;
                }
            }
        }
    }

    (void)repo_queue;
    (void)config;
    return 0;
}
