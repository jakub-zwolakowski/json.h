{
struct json_value_s *json_parse(
    const void *src,
    size_t src_size);
}

{
struct json_value_s {
  void *payload;
  size_t type;
};
}

{
struct json_value_s *json_parse_ex(
    const void *src,
    size_t src_size,
    size_t flags_bitset,
    void*(*alloc_func_ptr)(void *, size_t),
    void *user_data,
    struct json_parse_result_s *result);
}

{
enum json_parse_flags_e {
  json_parse_flags_default = 0,
  json_parse_flags_allow_trailing_comma = 0x1,
  json_parse_flags_allow_unquoted_keys = 0x2,
  json_parse_flags_allow_global_object = 0x4,
  json_parse_flags_allow_equals_in_object = 0x8,
  json_parse_flags_allow_no_commas = 0x10,
  json_parse_flags_allow_c_style_comments = 0x20,
  json_parse_flags_deprecated = 0x40,
  json_parse_flags_allow_location_information = 0x80,
  json_parse_flags_allow_single_quoted_strings = 0x100,
  json_parse_flags_allow_hexadecimal_numbers = 0x200,
  json_parse_flags_allow_leading_plus_sign = 0x400,
  json_parse_flags_allow_leading_or_trailing_decimal_point = 0x800,
  json_parse_flags_allow_inf_and_nan = 0x1000,
  json_parse_flags_allow_multi_line_strings = 0x2000,
  json_parse_flags_allow_simplified_json =
      (json_parse_flags_allow_trailing_comma |
       json_parse_flags_allow_unquoted_keys |
       json_parse_flags_allow_global_object |
       json_parse_flags_allow_equals_in_object |
       json_parse_flags_allow_no_commas),
  json_parse_flags_allow_json5 =
      (json_parse_flags_allow_trailing_comma |
       json_parse_flags_allow_unquoted_keys |
       json_parse_flags_allow_c_style_comments |
       json_parse_flags_allow_single_quoted_strings |
       json_parse_flags_allow_hexadecimal_numbers |
       json_parse_flags_allow_leading_plus_sign |
       json_parse_flags_allow_leading_or_trailing_decimal_point |
       json_parse_flags_allow_inf_and_nan |
       json_parse_flags_allow_multi_line_strings)
};
}

{
const char json[] = "{\"a\" : true, \"b\" : [false, null, \"foo\"]}";
struct json_value_s* root = json_parse(json, strlen(json));
assert(root->type == json_type_object);

struct json_object_s* object = (struct json_object_s*)root->payload;
assert(object->length == 2);

struct json_object_element_s* a = object->start;

struct json_string_s* a_name = a->name;
assert(0 == strcmp(a_name->string, "a"));
assert(a_name->string_size == strlen("a"));

struct json_value_s* a_value = a->value;
assert(a_value->type == json_type_true);
assert(a_value->payload == NULL);

struct json_object_element_s* b = a->next;
assert(b->next == NULL);

struct json_string_s* b_name = b->name;
assert(0 == strcmp(b_name->string, "b"));
assert(b_name->string_size == strlen("b"));

struct json_value_s* b_value = b->value;
assert(b_value->type == json_type_array);

struct json_array_s* array = (struct json_array_s*)b_value->payload;
assert(array->length == 3);

struct json_array_element_s* b_1st = array->start;

struct json_value_s* b_1st_value = b_1st->value;
assert(b_1st_value->type == json_type_false);
assert(b_1st_value->payload == NULL);

struct json_array_element_s* b_2nd = b_1st->next;

struct json_value_s* b_2nd_value = b_2nd->value;
assert(b_2nd_value->type == json_type_null);
assert(b_2nd_value->payload == NULL);

struct json_array_element_s* b_3rd = b_2nd->next;
assert(b_3rd->next == NULL);

struct json_value_s* b_3rd_value = b_3rd->value;
assert(b_3rd_value->type == json_type_string);

struct json_string_s* string = (struct json_string_s*)b_3rd_value->payload;
assert(0 == strcmp(string->string, "foo"));
assert(string->string_size == strlen("foo"));

/* Don't forget to free the one allocation! */
free(root);
}

{
const char json[] = "{\"a\" : true, \"b\" : [false, null, \"foo\"]}";
struct json_value_s* root = json_parse(json, strlen(json));

struct json_object_s* object = json_value_as_object(root);
assert(object != NULL);
assert(object->length == 2);

struct json_object_element_s* a = object->start;

struct json_string_s* a_name = a->name;
assert(0 == strcmp(a_name->string, "a"));
assert(a_name->string_size == strlen("a"));

struct json_value_s* a_value = a->value;
assert(json_value_is_true(a_value));

struct json_object_element_s* b = a->next;
assert(b->next == NULL);

struct json_string_s* b_name = b->name;
assert(0 == strcmp(b_name->string, "b"));
assert(b_name->string_size == strlen("b"));

struct json_array_s* array = json_value_as_array(b->value);
assert(array->length == 3);

struct json_array_element_s* b_1st = array->start;

struct json_value_s* b_1st_value = b_1st->value;
assert(json_value_is_false(b_1st_value));

struct json_array_element_s* b_2nd = b_1st->next;

struct json_value_s* b_2nd_value = b_2nd->value;
assert(json_value_is_null(b_2nd_value));

struct json_array_element_s* b_3rd = b_2nd->next;
assert(b_3rd->next == NULL);

struct json_string_s* string = json_value_as_string(b_3rd->value);
assert(string != NULL);
assert(0 == strcmp(string->string, "foo"));
assert(string->string_size == strlen("foo"));

/* Don't forget to free the one allocation! */
free(root);
}

