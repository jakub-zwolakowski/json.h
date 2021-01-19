#include "json.h"
#include <string.h>
#include <stdio.h>

int main(void) {
    char* src = NULL;
    size_t len;
    FILE *f = fopen("test.json", "r");
    if (!f) {
        printf("fopen FAILED!");
        exit(1);
    } else {
        if (getline(&src, &len, f) > 0) {
            /* Parameters */
            size_t src_size = strlen(src);
            size_t flags_bitset = json_parse_flags_default;
            struct json_value_s *value;
            struct json_parse_result_s result;
            /* Print */
            printf("CALL json_parse_ex\n");
            printf("> src_size = %zu\n", src_size);
            printf("> flags_bitset = %zu\n", flags_bitset);
            printf("--- JSON BEGINNING ---\n%s\n--- JSON END ---\n", src);
            /* Call */
            value = json_parse_ex(src, src_size, flags_bitset, NULL, 0, &result);
            /* Result */
            int return_value = 0;
            if (result.error != json_parse_error_none) {
                return_value = 1;
                printf("PARSING ERROR!\n");
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
                printf("PARSING SUCCESS!\n");
            }
            /* Clean-up */
            free(value);
            free(src);
            fclose(f);
            return return_value;
        } else {
            printf("getline FAILED!");
            fclose(f);
            exit(2);
        }
    }
}
