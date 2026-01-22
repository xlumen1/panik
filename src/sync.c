#include "panik.h"
#include <sys/types.h>
#include <unistd.h>

int download_repo_index(CURL* ch, struct repo repo);

int pk_sync(char** argv, int argc, struct repos repos) {
    CURL* ch = curl_easy_init();
    if (!ch) {
        return 1;
    }
    int repo_index = 0;
    int* repo_queue = calloc(repos.count, sizeof(int));
    for (int i = 0; i < repos.count; i++) {
        repo_queue[i] = -1;
    }

    if (argc == 0) {
       for (int j = 0; j < repos.count; j++) {
            repo_queue[repo_index] = j;
            repo_index++;
        } 
    } else {
        for (int i = 0; i < argc; i++) {
            for (int j = 0; j < repos.count; j++) {
                if (strcmp(repos.repositories[j].name, argv[i]) == 0) {
                    repo_queue[repo_index] = j;
                    repo_index++;
                }
            }
        }
    }

    curl_easy_setopt(ch, CURLOPT_CONNECTTIMEOUT, config.timeout);
    curl_easy_setopt(ch, CURLOPT_TIMEOUT, config.timeout);
    curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(ch, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(ch, CURLOPT_USERAGENT, "panik/0.1");

    for (int i = 0; i < repo_index; i++) {
        int idx = repo_queue[i];
        if (idx < 0)
            continue;
        
        printf("Downloading %s index from %s.\n", repos.repositories[idx].name, repos.repositories[idx].path);
        download_repo_index(ch, repos.repositories[idx]);
    }
    
    curl_easy_cleanup(ch);
	free(repo_queue);
    return 0;
}

int download_repo_index(CURL* ch, struct repo repo) {
    char url[512 - 32];
    char dir[512 - 32];
    char tmp_path[512];
    char final_path[512];

    snprintf(url, sizeof(url), "%s/index", repo.path);
    snprintf(dir, sizeof(dir), "/var/lib/panik/repos/%s", repo.name);
    snprintf(tmp_path, sizeof(tmp_path), "%s/index.tmp", dir);
    snprintf(final_path, sizeof(final_path), "%s/index", dir);

    if (ensure_dir("/var/lib/panik") != 0)
        return error("Failed to create /var/lib/panik");

    if (ensure_dir("/var/lib/panik/repos") != 0)
        return error("Failed to create /var/lib/panik/repos");

    if (ensure_dir(dir) != 0)
        return error("Failed to create repo dir");

    FILE *f = fopen(tmp_path, "wb");
    if (!f)
        return error("Failed to open temp file");

    curl_easy_setopt(ch, CURLOPT_URL, url);
    curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, write_file_cb);
    curl_easy_setopt(ch, CURLOPT_WRITEDATA, f);

    CURLcode res = curl_easy_perform(ch);
    fclose(f);

    if (res != CURLE_OK) {
        unlink(tmp_path);
        return error("Download failed");
    }

    if (rename(tmp_path, final_path) != 0) {
        unlink(tmp_path);
        return error("Failed to replace index");
    }

    return 0;
}
