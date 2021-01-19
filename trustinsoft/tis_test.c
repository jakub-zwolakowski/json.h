#include "json.h"
#include <string.h>
#include <stdio.h>

int main(void) {
    char* json = NULL;
    size_t len;
    FILE *f = fopen("test.json", "r");
    if (!f) {
        printf("fopen FAILED!");
        exit(1);
    } else {
        if (getdelim(&json, &len, '\0', f) != 0) {
            printf("json_parse(\"%s\", %lu);", json, len);
            struct json_value_s* root = json_parse(json, len);
            free(root);
            free(json);
        } else {
            printf("getdelim FAILED!");
            exit (1);
        }
        fclose(f);
    }
}
