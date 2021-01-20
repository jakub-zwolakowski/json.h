#include "json.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
    /* Get the file name */
    if (argc != 2) {
        printf("WRONG NUMBER OF ARGUMENTS!");
        exit(1);
    }
    char *path = argv[1];
    printf("TEST %s\n", path);
    /* Handle the path */
    int expected_result = 0; /* 0 = whatever, 1 = success, -1 = failure */
    if (strlen(path) >= 2) {
        if((path[0] == 'y') && (path[1] == '_'))
            expected_result = 1; /* Success is expected */
        if((path[0] == 'n') && (path[1] == '_'))
            expected_result = -1; /* Failure is expected */
    }
    switch(expected_result) {
        case 1: printf("EXPECTED PARSING RESULT: SUCCESS\n"); break;
        case -1: printf("EXPECTED PARSING RESULT: FAILURE\n"); break;
        default: printf("EXPECTED PARSING RESULT: WHATEVER\n"); break;
    }
    /* Open the file */
    FILE *f = fopen(path, "r");
    if (!f) {
        printf("fopen FAILED!");
        exit(2);
    }
    /* Read the JSON */
    char *src = NULL;
    size_t len;
    if (getline(&src, &len, f) <= 0) {
        printf("getline FAILED!");
        fclose(f);
        exit(3);
    }
    /* Prepare the parsing parameters */
    size_t src_size = strlen(src);
    size_t flags_bitset = json_parse_flags_default;
    struct json_value_s *value;
    struct json_parse_result_s result;
    /* Print the parsing parameters */
    printf("CALL json_parse_ex\n");
    printf("> src_size = %zu\n", src_size);
    printf("> flags_bitset = %zu\n", flags_bitset);
    printf("--- JSON BEGINNING ---\n%s\n--- JSON END ---\n", src);
    /* Call json_parse_ex */
    value = json_parse_ex(src, src_size, flags_bitset, NULL, 0, &result);
    /* Handle the result */
    int actual_result = 0; /* 0 = unknown, 1 = success, -1 = failure */
    if (result.error != json_parse_error_none) {
        actual_result = -1;
        printf("PARSING RESULT: ERROR\n");
        switch (result.error) {
            case json_parse_error_none:
                printf("> error: %s\n", "json_parse_error_none");
                break;
            case json_parse_error_expected_comma_or_closing_bracket:
                printf("> error: %s\n", "json_parse_error_expected_comma_or_closing_bracket");
                break;
            case json_parse_error_expected_colon:
                printf("> error: %s\n", "json_parse_error_expected_colon");
                break;
            case json_parse_error_expected_opening_quote:
                printf("> error: %s\n", "json_parse_error_expected_opening_quote");
                break;
            case json_parse_error_invalid_string_escape_sequence:
                printf("> error: %s\n", "json_parse_error_invalid_string_escape_sequence");
                break;
            case json_parse_error_invalid_number_format:
                printf("> error: %s\n", "json_parse_error_invalid_number_format");
                break;
            case json_parse_error_invalid_value:
                printf("> error: %s\n", "json_parse_error_invalid_value");
                break;
            case json_parse_error_premature_end_of_buffer:
                printf("> error: %s\n", "json_parse_error_premature_end_of_buffer");
                break;
            case json_parse_error_invalid_string:
                printf("> error: %s\n", "json_parse_error_invalid_string");
                break;
            case json_parse_error_allocator_failed:
                printf("> error: %s\n", "json_parse_error_allocator_failed");
                break;
            case json_parse_error_unexpected_trailing_characters:
                printf("> error: %s\n", "json_parse_error_unexpected_trailing_characters");
                break;
            case json_parse_error_unknown:
                printf("> error: %s\n", "json_parse_error_unknown");
                break;
        }
        printf("> error_offset: %zu\n", result.error_offset);
        printf("> error_line_no: %zu\n", result.error_line_no);
        printf("> error_row_no: %zu\n", result.error_row_no);
    } else {
        actual_result = 1;
        printf("PARSING RESULT: SUCCESS\n");
    }
    /* Was this the expected result? */
    int return_value = 0;
    /*@ assert result_as_expected: expected_result != -actual_result; */
    if (expected_result == -actual_result) {
        return_value = 1;
        printf("RESULT IS *NOT* AS EXPECTED\n");
    } else {
        printf("RESULT IS AS EXPECTED\n");
    }
    /* Clean-up */
    free(value);
    free(src);
    fclose(f);
    /* Finish */
    return return_value;
}
