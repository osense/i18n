// vim: set filetype=cpp shiftwidth=4 tabstop=4 expandtab tw=80:

/**
 *  =====================================================================
 *    Copyright 2011 Uvarov Michael 
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 *  $Id$
 * 
 *  @copyright 2010-2011 Michael Uvarov
 *  @author Michael Uvarov <freeakk@gmail.com>
 *  =====================================================================
 */

#define I18N_STRING     true
#define I18N_COLLATION  true
#define I18N_SEARCH    	true
#define I18N_MESSAGE   	true
#define I18N_REGEX   	true
#define I18N_LOCALE   	true

#define BUF_SIZE 65536 // 2^16, since we're using a 2-byte length header
#define STR_LEN 32768
#define LOCALE_LEN 255
#define ATOM_LEN 16


#include "unicode/uloc.h"
#include "unicode/ustring.h"
#include "unicode/uchar.h"
#include "unicode/ubrk.h"
#include "unicode/unorm2.h"

#include "unicode/ucol.h"

#include "unicode/umsg.h"
#include "unicode/msgfmt.h"

#include "unicode/utypes.h"

#include "unicode/uregex.h"


#include "erl_nif.h"

extern "C" {
#include "cloner.h"
}

#include <string.h>

#define CHECK(ENV, X) if (U_FAILURE(X)) return get_error_code(ENV, X); 





static ERL_NIF_TERM make_error(ErlNifEnv* env, const char* code) {
    return enif_make_tuple2(env,
        enif_make_atom(env, "error"),
        enif_make_atom(env, code));
}

/** 
 * http://icu-project.org/apiref/icu4c/utypes_8h.html#a3343c1c8a8377277046774691c98d78c
 */
static ERL_NIF_TERM get_error_code(ErlNifEnv* env, UErrorCode status) {
    switch (status) {
#ifdef U_ILLEGAL_ARGUMENT_ERROR
        case U_ILLEGAL_ARGUMENT_ERROR:
            return make_error(env, "i18n_illegal_argument_error");
#endif
#ifdef U_MISSING_RESOURCE_ERROR
        case U_MISSING_RESOURCE_ERROR:
            return make_error(env, "i18n_resourse_error");
#endif
#ifdef U_INVALID_FORMAT_ERROR
        case U_INVALID_FORMAT_ERROR:
            return make_error(env, "i18n_format_error");
#endif
#ifdef U_FILE_ACCESS_ERROR
        case U_FILE_ACCESS_ERROR:
            return make_error(env, "i18n_access_error");
#endif
#ifdef U_INTERNAL_PROGRAM_ERROR
        case U_INTERNAL_PROGRAM_ERROR:
            return make_error(env, "i18n_internal_program_error");
#endif
#ifdef U_MESSAGE_PARSE_ERROR
        case U_MESSAGE_PARSE_ERROR:
            return make_error(env, "i18n_parse_error");
#endif
#ifdef U_MEMORY_ALLOCATION_ERROR
        case U_MEMORY_ALLOCATION_ERROR:
            return make_error(env, "i18n_memory_allocation_error");
#endif
#ifdef U_INDEX_OUTOFBOUNDS_ERROR
        case U_INDEX_OUTOFBOUNDS_ERROR:
            return make_error(env, "i18n_index_out_of_bounds_error");
#endif
#ifdef U_INVALID_CHAR_FOUND
        case U_INVALID_CHAR_FOUND:
            return make_error(env, "i18n_invalid_char_found");
#endif
#ifdef U_TRUNCATED_CHAR_FOUND
        case U_TRUNCATED_CHAR_FOUND:
            return make_error(env, "i18n_truncated_char_found");
#endif
#ifdef U_ILLEGAL_CHAR_FOUND
        case U_ILLEGAL_CHAR_FOUND:
            return make_error(env, "i18n_illegal_char_found");
#endif
#ifdef U_INVALID_TABLE_FORMAT
        case U_INVALID_TABLE_FORMAT:
            return make_error(env, "i18n_invalid_table_format");
#endif
#ifdef U_INVALID_TABLE_FILE
        case U_INVALID_TABLE_FILE:
            return make_error(env, "i18n_invalid_table_file");
#endif
#ifdef U_BUFFER_OVERFLOW_ERROR
        case U_BUFFER_OVERFLOW_ERROR:
            return make_error(env, "i18n_buffer_overflow_error");
#endif
#ifdef U_UNSUPPORTED_ERROR
        case U_UNSUPPORTED_ERROR:
            return make_error(env, "i18n_unsupported_error");
#endif
#ifdef U_RESOURCE_TYPE_MISMATCH
        case U_RESOURCE_TYPE_MISMATCH:
            return make_error(env, "i18n_resource_type_mismatch");
#endif
#ifdef U_ILLEGAL_ESCAPE_SEQUENCE
        case U_ILLEGAL_ESCAPE_SEQUENCE:
            return make_error(env, "i18n_escape_sequence");
#endif
#ifdef U_UNSUPPORTED_ESCAPE_SEQUENCE
        case U_UNSUPPORTED_ESCAPE_SEQUENCE:
            return make_error(env, "i18n_unsupported_escape_sequence");
#endif
#ifdef U_NO_SPACE_AVAILABLE
        case U_NO_SPACE_AVAILABLE:
            return make_error(env, "i18n_no_space_available");
#endif
#ifdef U_CE_NOT_FOUND_ERROR
        case U_CE_NOT_FOUND_ERROR:
            return make_error(env, "i18n_ce_not_found_error");
#endif
#ifdef U_PRIMARY_TOO_LONG_ERROR
        case U_PRIMARY_TOO_LONG_ERROR:
            return make_error(env, "i18n_primary_too_long_error");
#endif
#ifdef U_STATE_TOO_OLD_ERROR
        case U_STATE_TOO_OLD_ERROR:
            return make_error(env, "i18n_state_too_old_error");
#endif
#ifdef U_TOO_MANY_ALIASES_ERROR
        case U_TOO_MANY_ALIASES_ERROR:
            return make_error(env, "i18n_too_many_aliases_error");
#endif
#ifdef U_ENUM_OUT_OF_SYNC_ERROR
        case U_ENUM_OUT_OF_SYNC_ERROR:
            return make_error(env, "i18n_enum_out_of_sync_error");
#endif
#ifdef U_INVARIANT_CONVERSION_ERROR
        case U_INVARIANT_CONVERSION_ERROR:
            return make_error(env, "i18n_invariant_conversion_error");
#endif
#ifdef U_INVALID_STATE_ERROR
        case U_INVALID_STATE_ERROR:
            return make_error(env, "i18n_invalid_state_error");
#endif
#ifdef U_COLLATOR_VERSION_MISMATCH
        case U_COLLATOR_VERSION_MISMATCH:
            return make_error(env, "i18n_collator_version_mismatch");
#endif
#ifdef U_USELESS_COLLATOR_ERROR
        case U_USELESS_COLLATOR_ERROR:
            return make_error(env, "i18n_useless_collator_error");
#endif
#ifdef U_NO_WRITE_PERMISSION
        case U_NO_WRITE_PERMISSION:
            return make_error(env, "i18n_no_write_permission");
#endif
#ifdef U_STANDARD_ERROR_LIMIT
        case U_STANDARD_ERROR_LIMIT:
            return make_error(env, "i18n_standard_error_limit");
#endif
#ifdef U_BAD_VARIABLE_DEFINITION
        case U_BAD_VARIABLE_DEFINITION:
            return make_error(env, "i18n_bad_variable_definition");
#endif
#ifdef U_PARSE_ERROR_START
        case U_PARSE_ERROR_START:
            return make_error(env, "i18n_parse_error_start");
#endif
#ifdef U_MALFORMED_RULE
        case U_MALFORMED_RULE:
            return make_error(env, "i18n_malformed_rule");
#endif
#ifdef U_MALFORMED_SET
        case U_MALFORMED_SET:
            return make_error(env, "i18n_malformed_set");
#endif
#ifdef U_MALFORMED_SYMBOL_REFERENCE
        case U_MALFORMED_SYMBOL_REFERENCE:
            return make_error(env, "i18n_malformed_symbol_reference");
#endif
#ifdef U_MALFORMED_UNICODE_ESCAPE
        case U_MALFORMED_UNICODE_ESCAPE:
            return make_error(env, "i18n_malformed_unicode_escape");
#endif
#ifdef U_MALFORMED_VARIABLE_DEFINITION
        case U_MALFORMED_VARIABLE_DEFINITION:
            return make_error(env, "i18n_malformed_variable_definition");
#endif
#ifdef U_MALFORMED_VARIABLE_REFERENCE
        case U_MALFORMED_VARIABLE_REFERENCE:
            return make_error(env, "i18n_malformed_variable_reference");
#endif
#ifdef U_MISMATCHED_SEGMENT_DELIMITERS
        case U_MISMATCHED_SEGMENT_DELIMITERS:
            return make_error(env, "i18n_mismatched_segment_delimiters");
#endif
#ifdef U_MISPLACED_ANCHOR_START
        case U_MISPLACED_ANCHOR_START:
            return make_error(env, "i18n_misplaced_anchor_start");
#endif
#ifdef U_MISPLACED_CURSOR_OFFSET
        case U_MISPLACED_CURSOR_OFFSET:
            return make_error(env, "i18n_misplaced_cursor_offset");
#endif
#ifdef U_MISPLACED_QUANTIFIER
        case U_MISPLACED_QUANTIFIER:
            return make_error(env, "i18n_misplaced_quantifier");
#endif
#ifdef U_MISSING_OPERATOR
        case U_MISSING_OPERATOR:
            return make_error(env, "i18n_missing_operator");
#endif
#ifdef U_MISSING_SEGMENT_CLOSE
        case U_MISSING_SEGMENT_CLOSE:
            return make_error(env, "i18n_missing_segment_close");
#endif
#ifdef U_MULTIPLE_ANTE_CONTEXTS
        case U_MULTIPLE_ANTE_CONTEXTS:
            return make_error(env, "i18n_multiple_ante_contexts");
#endif
#ifdef U_MULTIPLE_CURSORS
        case U_MULTIPLE_CURSORS:
            return make_error(env, "i18n_multiple_cursors");
#endif
#ifdef U_MULTIPLE_POST_CONTEXTS
        case U_MULTIPLE_POST_CONTEXTS:
            return make_error(env, "i18n_multiple_post_contexts");
#endif
#ifdef U_TRAILING_BACKSLASH
        case U_TRAILING_BACKSLASH:
            return make_error(env, "i18n_trailing_backslash");
#endif
#ifdef U_UNDEFINED_SEGMENT_REFERENCE
        case U_UNDEFINED_SEGMENT_REFERENCE:
            return make_error(env, "i18n_undefined_segment_reference");
#endif
#ifdef U_UNDEFINED_VARIABLE
        case U_UNDEFINED_VARIABLE:
            return make_error(env, "i18n_undefined_variable");
#endif
#ifdef U_UNQUOTED_SPECIAL
        case U_UNQUOTED_SPECIAL:
            return make_error(env, "i18n_unquoted_special");
#endif
#ifdef U_UNTERMINATED_QUOTE
        case U_UNTERMINATED_QUOTE:
            return make_error(env, "i18n_unterminated_quote");
#endif
#ifdef U_RULE_MASK_ERROR
        case U_RULE_MASK_ERROR:
            return make_error(env, "i18n_rule_mask_error");
#endif
#ifdef U_MISPLACED_COMPOUND_FILTER
        case U_MISPLACED_COMPOUND_FILTER:
            return make_error(env, "i18n_misplaced_compound_filter");
#endif
#ifdef U_MULTIPLE_COMPOUND_FILTERS
        case U_MULTIPLE_COMPOUND_FILTERS:
            return make_error(env, "i18n_multiple_compound_filters");
#endif
#ifdef U_INVALID_RBT_SYNTAX
        case U_INVALID_RBT_SYNTAX:
            return make_error(env, "i18n_invalid_rbt_syntax");
#endif
#ifdef U_INVALID_PROPERTY_PATTERN
        case U_INVALID_PROPERTY_PATTERN:
            return make_error(env, "i18n_invalid_property_pattern");
#endif
#ifdef U_MALFORMED_PRAGMA
        case U_MALFORMED_PRAGMA:
            return enif_make_atom(env, "i19n_malformed_pragma");
#endif
#ifdef U_UNCLOSED_SEGMENT
        case U_UNCLOSED_SEGMENT:
            return make_error(env, "i18n_unclosed_segment");
#endif
#ifdef U_ILLEGAL_CHAR_IN_SEGMENT
        case U_ILLEGAL_CHAR_IN_SEGMENT:
            return make_error(env, "i18n_illegal_char_in_segment");
#endif
#ifdef U_VARIABLE_RANGE_EXHAUSTED
        case U_VARIABLE_RANGE_EXHAUSTED:
            return make_error(env, "i18n_variable_range_exhausted");
#endif
#ifdef U_VARIABLE_RANGE_OVERLAP
        case U_VARIABLE_RANGE_OVERLAP:
            return make_error(env, "i18n_variable_range_overlap");
#endif
#ifdef U_ILLEGAL_CHARACTER
        case U_ILLEGAL_CHARACTER:
            return make_error(env, "i18n_illegal_character");
#endif
#ifdef U_INTERNAL_TRANSLITERATOR_ERROR
        case U_INTERNAL_TRANSLITERATOR_ERROR:
            return make_error(env, "i18n_internal_transliterator_error");
#endif
#ifdef U_INVALID_ID
        case U_INVALID_ID:
            return make_error(env, "i18n_invalid_id");
#endif
#ifdef U_INVALID_FUNCTION
        case U_INVALID_FUNCTION:
            return make_error(env, "i18n_invalid_function");
#endif
#ifdef U_PARSE_ERROR_LIMIT
        case U_PARSE_ERROR_LIMIT:
            return make_error(env, "i18n_parse_error_limit");
#endif
#ifdef U_UNEXPECTED_TOKEN
        case U_UNEXPECTED_TOKEN:
            return make_error(env, "i18n_unexpected_token");
#endif
#ifdef U_FMT_PARSE_ERROR_START
        case U_FMT_PARSE_ERROR_START:
            return make_error(env, "i18n_fmt_parse_error_start");
#endif
#ifdef U_MULTIPLE_DECIMAL_SEPARATORS
        case U_MULTIPLE_DECIMAL_SEPARATORS:
            return make_error(env, "i18n_multiple_decimal_separators");
#endif
#ifdef U_MULTIPLE_DECIMAL_SEPERATORS
        case U_MULTIPLE_DECIMAL_SEPERATORS:
            return make_error(env, "i18n_multiple_decimal_seperators");
#endif
#ifdef U_MULTIPLE_EXPONENTIAL_SYMBOLS
        case U_MULTIPLE_EXPONENTIAL_SYMBOLS:
            return make_error(env, "i18n_multiple_exponential_symbols");
#endif
#ifdef U_MALFORMED_EXPONENTIAL_PATTERN
        case U_MALFORMED_EXPONENTIAL_PATTERN:
            return make_error(env, "i18n_malformed_exponential_pattern");
#endif
#ifdef U_MULTIPLE_PERCENT_SYMBOLS
        case U_MULTIPLE_PERCENT_SYMBOLS:
            return make_error(env, "i18n_multiple_percent_symbols");
#endif
#ifdef U_MULTIPLE_PERMILL_SYMBOLS
        case U_MULTIPLE_PERMILL_SYMBOLS:
            return make_error(env, "i18n_multiple_permill_symbols");
#endif
#ifdef U_MULTIPLE_PAD_SPECIFIERS
        case U_MULTIPLE_PAD_SPECIFIERS:
            return make_error(env, "i18n_multiple_pad_specifiers");
#endif
#ifdef U_PATTERN_SYNTAX_ERROR
        case U_PATTERN_SYNTAX_ERROR:
            return make_error(env, "i18n_pattern_syntax_error");
#endif
#ifdef U_ILLEGAL_PAD_POSITION
        case U_ILLEGAL_PAD_POSITION:
            return make_error(env, "i18n_illegal_pad_position");
#endif
#ifdef U_UNMATCHED_BRACES
        case U_UNMATCHED_BRACES:
            return make_error(env, "i18n_unmatched_braces");
#endif
#ifdef U_UNSUPPORTED_PROPERTY
        case U_UNSUPPORTED_PROPERTY:
            return make_error(env, "i18n_unsupported_property");
#endif
#ifdef U_UNSUPPORTED_ATTRIBUTE
        case U_UNSUPPORTED_ATTRIBUTE:
            return make_error(env, "i18n_unsupported_attribute");
#endif
#ifdef U_ARGUMENT_TYPE_MISMATCH
        case U_ARGUMENT_TYPE_MISMATCH:
            return make_error(env, "i18n_argument_type_mismatch");
#endif
#ifdef U_DUPLICATE_KEYWORD
        case U_DUPLICATE_KEYWORD:
            return make_error(env, "i18n_duplicate_keyword");
#endif
#ifdef U_UNDEFINED_KEYWORD
        case U_UNDEFINED_KEYWORD:
            return make_error(env, "i18n_undefined_keyword");
#endif
#ifdef U_DEFAULT_KEYWORD_MISSING
        case U_DEFAULT_KEYWORD_MISSING:
            return make_error(env, "i18n_default_keyword_missing");
#endif
#ifdef U_DECIMAL_NUMBER_SYNTAX_ERROR
        case U_DECIMAL_NUMBER_SYNTAX_ERROR:
            return make_error(env, "i18n_decimal_number_syntax_error");
#endif
#ifdef U_FORMAT_INEXACT_ERROR
        case U_FORMAT_INEXACT_ERROR:
            return make_error(env, "i18n_format_inexact_error");
#endif
#ifdef U_FMT_PARSE_ERROR_LIMIT
        case U_FMT_PARSE_ERROR_LIMIT:
            return make_error(env, "i18n_fmt_parse_error_limit");
#endif
#ifdef U_BRK_INTERNAL_ERROR
        case U_BRK_INTERNAL_ERROR:
            return make_error(env, "i18n_brk_internal_error");
#endif
#ifdef U_BRK_ERROR_START
        case U_BRK_ERROR_START:
            return make_error(env, "i18n_brk_error_start");
#endif
#ifdef U_BRK_HEX_DIGITS_EXPECTED
        case U_BRK_HEX_DIGITS_EXPECTED:
            return make_error(env, "i18n_brk_hex_digits_expected");
#endif
#ifdef U_BRK_SEMICOLON_EXPECTED
        case U_BRK_SEMICOLON_EXPECTED:
            return make_error(env, "i18n_brk_semicolon_expected");
#endif
#ifdef U_BRK_RULE_SYNTAX
        case U_BRK_RULE_SYNTAX:
            return make_error(env, "i18n_brk_rule_syntax");
#endif
#ifdef U_BRK_UNCLOSED_SET
        case U_BRK_UNCLOSED_SET:
            return make_error(env, "i18n_brk_unclosed_set");
#endif
#ifdef U_BRK_ASSIGN_ERROR
        case U_BRK_ASSIGN_ERROR:
            return make_error(env, "i18n_brk_assign_error");
#endif
#ifdef U_BRK_VARIABLE_REDFINITION
        case U_BRK_VARIABLE_REDFINITION:
            return make_error(env, "i18n_brk_variable_redfinition");
#endif
#ifdef U_BRK_MISMATCHED_PAREN
        case U_BRK_MISMATCHED_PAREN:
            return make_error(env, "i18n_brk_mismatched_paren");
#endif
#ifdef U_BRK_NEW_LINE_IN_QUOTED_STRING
        case U_BRK_NEW_LINE_IN_QUOTED_STRING:
            return make_error(env, "i18n_brk_new_line_in_quoted_string");
#endif
#ifdef U_BRK_UNDEFINED_VARIABLE
        case U_BRK_UNDEFINED_VARIABLE:
            return make_error(env, "i18n_brk_undefined_variable");
#endif
#ifdef U_BRK_INIT_ERROR
        case U_BRK_INIT_ERROR:
            return make_error(env, "i18n_brk_init_error");
#endif
#ifdef U_BRK_RULE_EMPTY_SET
        case U_BRK_RULE_EMPTY_SET:
            return make_error(env, "i18n_brk_rule_empty_set");
#endif
#ifdef U_BRK_UNRECOGNIZED_OPTION
        case U_BRK_UNRECOGNIZED_OPTION:
            return make_error(env, "i18n_brk_unrecognized_option");
#endif
#ifdef U_BRK_MALFORMED_RULE_TAG
        case U_BRK_MALFORMED_RULE_TAG:
            return make_error(env, "i18n_brk_malformed_rule_tag");
#endif
#ifdef U_BRK_ERROR_LIMIT
        case U_BRK_ERROR_LIMIT:
            return make_error(env, "i18n_brk_error_limit");
#endif
#ifdef U_REGEX_INTERNAL_ERROR
        case U_REGEX_INTERNAL_ERROR:
            return make_error(env, "i18n_regex_internal_error");
#endif
#ifdef U_REGEX_ERROR_START
        case U_REGEX_ERROR_START:
            return make_error(env, "i18n_regex_error_start");
#endif
#ifdef U_REGEX_RULE_SYNTAX
        case U_REGEX_RULE_SYNTAX:
            return make_error(env, "i18n_regex_rule_syntax");
#endif
#ifdef U_REGEX_INVALID_STATE
        case U_REGEX_INVALID_STATE:
            return make_error(env, "i18n_regex_invalid_state");
#endif
#ifdef U_REGEX_BAD_ESCAPE_SEQUENCE
        case U_REGEX_BAD_ESCAPE_SEQUENCE:
            return make_error(env, "i18n_regex_bad_escape_sequence");
#endif
#ifdef U_REGEX_PROPERTY_SYNTAX
        case U_REGEX_PROPERTY_SYNTAX:
            return make_error(env, "i18n_regex_property_syntax");
#endif
#ifdef U_REGEX_UNIMPLEMENTED
        case U_REGEX_UNIMPLEMENTED:
            return make_error(env, "i18n_regex_unimplemented");
#endif
#ifdef U_REGEX_MISMATCHED_PAREN
        case U_REGEX_MISMATCHED_PAREN:
            return make_error(env, "i18n_regex_mismatched_paren");
#endif
#ifdef U_REGEX_NUMBER_TOO_BIG
        case U_REGEX_NUMBER_TOO_BIG:
            return make_error(env, "i18n_regex_number_too_big");
#endif
#ifdef U_REGEX_BAD_INTERVAL
        case U_REGEX_BAD_INTERVAL:
            return make_error(env, "i18n_regex_bad_interval");
#endif
#ifdef U_REGEX_MAX_LT_MIN
        case U_REGEX_MAX_LT_MIN:
            return make_error(env, "i18n_regex_max_lt_min");
#endif
#ifdef U_REGEX_INVALID_BACK_REF
        case U_REGEX_INVALID_BACK_REF:
            return make_error(env, "i18n_regex_invalid_back_ref");
#endif
#ifdef U_REGEX_INVALID_FLAG
        case U_REGEX_INVALID_FLAG:
            return make_error(env, "i18n_regex_invalid_flag");
#endif
#ifdef U_REGEX_LOOK_BEHIND_LIMIT
        case U_REGEX_LOOK_BEHIND_LIMIT:
            return make_error(env, "i18n_regex_look_behind_limit");
#endif
#ifdef U_REGEX_SET_CONTAINS_STRING
        case U_REGEX_SET_CONTAINS_STRING:
            return make_error(env, "i18n_regex_set_contains_string");
#endif
#ifdef U_REGEX_OCTAL_TOO_BIG
        case U_REGEX_OCTAL_TOO_BIG:
            return make_error(env, "i18n_regex_octal_too_big");
#endif
#ifdef U_REGEX_MISSING_CLOSE_BRACKET
        case U_REGEX_MISSING_CLOSE_BRACKET:
            return make_error(env, "i18n_regex_missing_close_bracket");
#endif
#ifdef U_REGEX_INVALID_RANGE
        case U_REGEX_INVALID_RANGE:
            return make_error(env, "i18n_regex_invalid_range");
#endif
#ifdef U_REGEX_STACK_OVERFLOW
        case U_REGEX_STACK_OVERFLOW:
            return make_error(env, "i18n_regex_stack_overflow");
#endif
#ifdef U_REGEX_TIME_OUT
        case U_REGEX_TIME_OUT:
            return make_error(env, "i18n_regex_time_out");
#endif
#ifdef U_REGEX_STOPPED_BY_CALLER
        case U_REGEX_STOPPED_BY_CALLER:
            return make_error(env, "i18n_regex_stopped_by_caller");
#endif
#ifdef U_REGEX_ERROR_LIMIT
        case U_REGEX_ERROR_LIMIT:
            return make_error(env, "i18n_regex_error_limit");
#endif
#ifdef U_PLUGIN_ERROR_START
        case U_PLUGIN_ERROR_START:
            return make_error(env, "i18n_plugin_error_start");
#endif
#ifdef U_PLUGIN_TOO_HIGH
        case U_PLUGIN_TOO_HIGH:
            return make_error(env, "i18n_plugin_too_high");
#endif
#ifdef U_PLUGIN_DIDNT_SET_LEVEL
        case U_PLUGIN_DIDNT_SET_LEVEL:
            return make_error(env, "i18n_plugin_didnt_set_level");
#endif
#ifdef U_PLUGIN_ERROR_LIMIT
        case U_PLUGIN_ERROR_LIMIT:
            return make_error(env, "i18n_plugin_error_limit");
#endif
        default:
            return make_error(env, "i18n_unknown");
    }
}

















/**
 * i18n_string
 */
#ifdef I18N_STRING

static ErlNifResourceType* iterator_type = 0;
static const UNormalizer2* nfc_normalizer = 0;




// Called from erl_nif.
void iterator_dtor(ErlNifEnv* env, void* obj) 
{
    // Free memory
    cloner_destroy((cloner*) obj); 
}



// Called from cloner for each thread.
void iterator_destr(char* obj) 
{
    if (obj != NULL)
        ubrk_close((UBreakIterator*) obj);   
}
char* iterator_clone(char* obj) 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t size = U_BRK_SAFECLONE_BUFFERSIZE;

    obj = (char*) ubrk_safeClone(
        (UBreakIterator*) obj,
        NULL,
        &size,
        &status 
    );
    if(U_FAILURE(status)) { 
        return NULL;
    } 
    return obj;
}

int iterator_open(UBreakIterator * obj, cloner* c)
{
    return cloner_open((char *) obj, c, &iterator_clone, &iterator_destr);
} 








/**
 * Helpers
 */
int parseIteratorType(const char * type) 
{
    return (!strcmp((char*) "grapheme", type)) ? UBRK_CHARACTER :
           (!strcmp((char*) "word", type))     ? UBRK_WORD      :
           (!strcmp((char*) "line", type))     ? UBRK_LINE      :
           (!strcmp((char*) "sentence", type)) ? UBRK_SENTENCE  :
            -1;
}


/**
 * NIFs
 */

// Convert utf8 to utf16
static ERL_NIF_TERM from_utf8(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    // ulen is length in UChars, len is in bytes
    int32_t ulen, len; 
    UChar buf[STR_LEN], buf2[STR_LEN];
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;

    // First argument must be a binary 
    if(!enif_inspect_binary(env, argv[0], &in)) {
        return enif_make_badarg(env);
    }

    // 1. Convert a binary string in utf-8 to a binary string in utf-16.
    u_strFromUTF8((UChar*) &buf, // dest
        (int32_t) STR_LEN,       // capacity
        &ulen,                   // len of result
        (char*) in.data,         // src
        (int32_t) in.size,       // len of src
        &status);                // error code
    CHECK(env, status);

    // 2. Normolize it into NFC.
    ulen = unorm2_normalize(nfc_normalizer,
        (UChar*) &buf,      // src
        ulen,               // len of src
        (UChar*) &buf2,     // dest
        (int32_t) STR_LEN,  // capacity
        &status);           // error code
    CHECK(env, status);

    // length in bytes
    len = ulen * sizeof(UChar);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy((void*) bin, (void*)(&buf2), (size_t) len);
    return out;
}
// Convert utf16 to utf8
static ERL_NIF_TERM to_utf8(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    int32_t len; 
    char buf[BUF_SIZE];
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;

    // First argument must be a binary 
    if(!enif_inspect_binary(env, argv[0], &in)) {
        return enif_make_badarg(env);
    }

    u_strToUTF8((char*) (&buf), 
        BUF_SIZE, 
        &len, 
        (UChar *) (in.data), 
        div(in.size, sizeof(UChar)).quot, 
        &status);

    CHECK(env, status);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}
static ERL_NIF_TERM concat(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in, in2;
    ERL_NIF_TERM out;
    int32_t ulen, len; 
    UChar buf[STR_LEN];
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;

    if(!(enif_inspect_binary(env, argv[0], &in) 
      && enif_inspect_binary(env, argv[1], &in2))) {
        return enif_make_badarg(env);
    }

    memcpy(&buf, in.data, in.size);
    ulen = div(in.size, sizeof(UChar)).quot;

    ulen = unorm2_append(nfc_normalizer,
        (UChar*) (&buf),  // first
        ulen,             // first len
        STR_LEN,          // first capacity
        (UChar*) in.data, // second
        div(in2.size, sizeof(UChar)).quot, // second length
        &status);
    CHECK(env, status);

    // Length in bytes of the new first string
    len = ulen * sizeof(UChar);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy((void*) bin, (void*)(&buf), (size_t) len);
    return out;
}
static ERL_NIF_TERM to_upper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    int32_t len, ulen, locale_len; 
    UChar buf[STR_LEN];
    char locale[LOCALE_LEN];
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;

    // Second argument must be a binary 
    if(!enif_inspect_binary(env, argv[1], &in)) {
        return enif_make_badarg(env);
    }

    locale_len = enif_get_atom(env, argv[0], locale, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!locale_len) {
        return enif_make_badarg(env);
    }

    ulen = u_strToUpper((UChar*) (&buf), 
            (int32_t) STR_LEN, 
            (UChar*) in.data, 
            div(in.size, sizeof(UChar)).quot, 
            (char*) locale, 
            &status);

    // Length in bytes of the new first string
    len = ulen * sizeof(UChar);

    CHECK(env, status);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}
static ERL_NIF_TERM to_lower(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    int32_t len, ulen, locale_len; 
    UChar buf[STR_LEN];
    char locale[LOCALE_LEN];
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;

    // Second argument must be a binary 
    if(!enif_inspect_binary(env, argv[1], &in)) {
        return enif_make_badarg(env);
    }

    locale_len = enif_get_atom(env, argv[0], locale, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!locale_len) {
        return enif_make_badarg(env);
    }

    ulen = u_strToLower((UChar*) (&buf), 
            (int32_t) STR_LEN, 
            (UChar*) in.data, 
            div(in.size, sizeof(UChar)).quot, 
            (char*) locale, 
            &status);
    CHECK(env, status);

    // Length in bytes of the new first string
    len = ulen * sizeof(UChar);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}
static ERL_NIF_TERM to_title(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    int32_t len, ulen, locale_len; 
    UChar buf[STR_LEN];
    char locale[LOCALE_LEN];
    const char* locptr;
    UErrorCode status = U_ZERO_ERROR;
    unsigned char* bin;
    cloner* ptr; 
    UBreakIterator* iter; 

    // Second argument must be a binary 
    if(!enif_inspect_binary(env, argv[1], &in)) {
        return enif_make_badarg(env);
    }

    locale_len = enif_get_atom(env, argv[0], locale, LOCALE_LEN, ERL_NIF_LATIN1);
    if (locale_len) {
        // First element is an atom.
        locptr = (const char*) locale;
        iter = NULL;

    } else if (enif_get_resource(env, argv[0], iterator_type, (void**) &ptr)) {
        // First element is an iterator.
        iter = (UBreakIterator*) cloner_get(ptr);

        // Iterator contains a locale name. Extract it.
        locptr = ubrk_getLocaleByType((const UBreakIterator*) iter,
            ULOC_ACTUAL_LOCALE, // locale type
            &status);

        CHECK(env, status);

    } else {
        return enif_make_badarg(env);
    }  

    // lenght in bytes
    ulen = u_strToTitle((UChar*) (&buf), 
            (int32_t) STR_LEN, 
            (UChar*) in.data, 
            div(in.size, sizeof(UChar)).quot, 
            iter, // Iterator
            locptr, 
            &status);
    CHECK(env, status);

    // Length in bytes of the new first string
    len = ulen * sizeof(UChar);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}
static ERL_NIF_TERM len(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    int currb;
    cloner* ptr; 
    UBreakIterator* iter; 
    int len = -1;
    UErrorCode status = U_ZERO_ERROR;

    // Last argument must be a binary 
    if (!(enif_inspect_binary(env, argv[1], &in)
       && enif_get_resource(env, argv[0], iterator_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }    
    iter = (UBreakIterator*) cloner_get(ptr);
    if (iter == NULL) {
        return enif_make_badarg(env);
    }

    

    // Do count
    ubrk_setText(iter,
        (UChar *) in.data,
        div(in.size, sizeof(UChar)).quot, 
        &status);
    CHECK(env, status);

    for (currb = ubrk_first(iter);
        currb != UBRK_DONE;
        currb = ubrk_next(iter)) {
        len++;
    }

    return enif_make_int(env, len);
}
static ERL_NIF_TERM get_iterator(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM out;
    int32_t atom_len; 
    char atom[LOCALE_LEN];
    int type;

    UErrorCode status = U_ZERO_ERROR;
    UBreakIteratorType iterType;
    UBreakIterator* iter; 
    cloner* res;


    // Get iterator type
    atom_len = enif_get_atom(env, argv[1], atom, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!atom_len) {
        return enif_make_badarg(env);
    }
    // atom_len is free.

    // If -1 then throw error (unknown type).
      type = parseIteratorType((char*) atom);
    // atom is free.
    if (type == -1) {
        return enif_make_badarg(env);
    }
    iterType = (UBreakIteratorType) type;
    // currb is free.

    // Get locale id: reuse atom and atom_len variables.
    atom_len = enif_get_atom(env, argv[0], atom, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!atom_len) {
        return enif_make_badarg(env);
    }


    iter = ubrk_open(iterType, 
            (char*) atom, // locale
            NULL,
            0, 
            &status);
    CHECK(env, status);

    if(!iter) { 
        return enif_make_badarg(env);
    } 

    res = (cloner*) enif_alloc_resource(iterator_type, sizeof(cloner));
    if (iterator_open(iter, res)) {
        enif_release_resource(res);
        return enif_make_badarg(env);
    }
    out = enif_make_resource(env, res);
    enif_release_resource(res);
    /* resource now only owned by "Erlang" */
    return out;
}




static int i18n_string_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    UErrorCode status = U_ZERO_ERROR;
    char type[] = "nfc";

    iterator_type = enif_open_resource_type(env, NULL, "iterator_type",
        iterator_dtor, ERL_NIF_RT_CREATE, NULL); 

    if (iterator_type == NULL) return 1;

    // get NFC normalizer
    nfc_normalizer = unorm2_getInstance(NULL,
        (char*) type,
        UNORM2_COMPOSE,
        &status);
    if(U_FAILURE(status)) return 2;
    return 0;
}

#endif



































#ifdef I18N_COLLATION
UCollator* base_col;
static ErlNifResourceType* collator_type = 0;



// Called from erl_nif.
void collator_dtor(ErlNifEnv* env, void* obj) 
{
    // Free memory
    cloner_destroy((cloner*) obj); 
}



// Called from cloner for each thread.
void collator_destr(char* obj) 
{ 
    if (obj != NULL)
        ucol_close((UCollator*) obj);
}
char* collator_clone(char* obj) 
{
    UErrorCode status = U_ZERO_ERROR;
    int32_t size = U_COL_SAFECLONE_BUFFERSIZE;

    obj = (char*) ucol_safeClone(
        (UCollator*) obj,
        NULL,
        &size,
        &status 
    );
    if(U_FAILURE(status)) { 
        return NULL;
    } 
    return obj;
}

int collator_open(UCollator * obj, cloner* c)
{
    return cloner_open((char *) obj, c, &collator_clone, &collator_destr);
} 





int parseAttrValue(const char * type) 
{
    return (!strcmp((char*) "primary", type))       ? UCOL_PRIMARY :
           (!strcmp((char*) "secondary", type))     ? UCOL_SECONDARY :
           (!strcmp((char*) "tertiary", type))      ? UCOL_TERTIARY :
           (!strcmp((char*) "quaternary", type))    ? UCOL_QUATERNARY :
           (!strcmp((char*) "identical", type))     ? UCOL_IDENTICAL :
           (!strcmp((char*) "default", type))       ? UCOL_DEFAULT  :

           (!strcmp((char*) "non_ignorable", type)) ? UCOL_NON_IGNORABLE :
           (!strcmp((char*) "shifted", type))       ? UCOL_SHIFTED :

           (!strcmp((char*) "on", type))            ? UCOL_ON :
           (!strcmp((char*) "off", type))           ? UCOL_OFF :

           (!strcmp((char*) "lower_first", type))   ? UCOL_LOWER_FIRST :
           (!strcmp((char*) "upper_first", type))   ? UCOL_UPPER_FIRST :
            -1;
}
int parseAttrKey(const char * type) 
{
    return (!strcmp((char*) "alternate", type))      ? UCOL_ALTERNATE_HANDLING :
           (!strcmp((char*) "case_first", type))     ? UCOL_CASE_FIRST :
           (!strcmp((char*) "french_accents", type)) ? UCOL_FRENCH_COLLATION :
           (!strcmp((char*) "case_level", type))     ? UCOL_CASE_LEVEL  :
           (!strcmp((char*) "normalization", type))  ? UCOL_NORMALIZATION_MODE :
           (!strcmp((char*) "strength", type))       ? UCOL_STRENGTH :
           (!strcmp((char*) "hiragana", type))       ? UCOL_HIRAGANA_QUATERNARY_MODE :
           (!strcmp((char*) "numeric", type))        ? UCOL_NUMERIC_COLLATION :
            -1;
}




/**
 * NIFs
 */

// Get a collator
static ERL_NIF_TERM get_collator(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM out;
    int32_t locale_len; 
    char locale[LOCALE_LEN];
    UErrorCode status = U_ZERO_ERROR;
    UCollator* col;
    cloner* res;


    locale_len = enif_get_atom(env, argv[0], (char*) locale, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!locale_len) {
        return enif_make_badarg(env);
    }

    // get a collator
    col = ucol_open((char *) locale, &status);
    CHECK(env, status);



    res = (cloner*) enif_alloc_resource(collator_type, sizeof(cloner));

    if (collator_open(col, res)) {
        enif_release_resource(res);
        return enif_make_badarg(env);
    }
    out = enif_make_resource(env, res);
    enif_release_resource(res);
    /* resource now only owned by "Erlang" */
    return out;
}
static ERL_NIF_TERM sort_key(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in;
    ERL_NIF_TERM out;
    int32_t len; 
    uint8_t buf[BUF_SIZE];
    unsigned char* bin;
    UCollator* col;
    cloner* ptr;

    // Second argument must be a binary 
    if(!(enif_inspect_binary(env, argv[1], &in)
      && enif_get_resource(env, argv[0], collator_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }

    col = (UCollator*) cloner_get(ptr);

    len = ucol_getSortKey(col,
        (UChar*) in.data, 
        (int32_t) div(in.size, sizeof(UChar)).quot, // in UChars
        (uint8_t *) buf,
        BUF_SIZE); 


    // If there was an internal error generating the sort key, a zero value is returned.  
    if(!len) {
        return enif_make_badarg(env);
    } 

    // Delete NULL from the end of the key.
    len--;

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}
static ERL_NIF_TERM compare(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in, in2;
    UCollator* col;
    cloner* ptr;
    UCollationResult res;

    // Second argument must be a binary 
    if(!(enif_inspect_binary(env, argv[1], &in) 
      && enif_inspect_binary(env, argv[2], &in2)
      && enif_get_resource(env, argv[0], collator_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }
    col = (UCollator*) cloner_get(ptr);

    res = ucol_strcoll(col,
        (UChar*) in.data, 
        (int32_t) div(in.size, sizeof(UChar)).quot, 
        (UChar*) in2.data, 
        (int32_t) div(in2.size, sizeof(UChar)).quot 
    );   

    switch (res) {
        case UCOL_EQUAL:
            return enif_make_atom(env, "equal");
            break;
            
        case UCOL_GREATER: 
            return enif_make_atom(env, "greater");
            break;

        case UCOL_LESS:
            return enif_make_atom(env, "less");
            break;
    }
    return enif_make_badarg(env);
}
static ERL_NIF_TERM collator_set_attr(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UCollator* col;
    cloner* ptr;
    UErrorCode status = U_ZERO_ERROR;

    int32_t value_len,       key_len; 
    char    value[ATOM_LEN], key[ATOM_LEN];
    int     parsed_value,    parsed_key;


    key_len   = enif_get_atom(env, argv[1], (char*) key,   ATOM_LEN, ERL_NIF_LATIN1);
    value_len = enif_get_atom(env, argv[2], (char*) value, ATOM_LEN, ERL_NIF_LATIN1);

    if (!(key_len && value_len && 
        enif_get_resource(env, argv[0], collator_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }
    col = (UCollator*) cloner_get(ptr);

    parsed_key   = parseAttrKey(key);
    parsed_value = parseAttrValue(value);
    if ((parsed_value == -1) || (parsed_key == -1)) {
        return enif_make_badarg(env);
    }

    ucol_setAttribute(col,
        (UColAttribute)      parsed_key,
        (UColAttributeValue) parsed_value,
        &status);
    CHECK(env, status);

    return enif_make_atom(env, "ok");
}











static int i18n_collation_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    UErrorCode status = U_ZERO_ERROR;

    base_col = ucol_open("", &status);
    if(U_FAILURE(status)) { 
        return 3;
    }
    collator_type = enif_open_resource_type(env, NULL, "collator_type",
        collator_dtor, ERL_NIF_RT_CREATE, NULL); 
    if (collator_type == NULL) return 4;
    
    return 0;
}

static void i18n_collation_unload(ErlNifEnv* env, void* priv)
{
    ucol_close(base_col);
    return;
}
#endif
































#ifdef I18N_MESSAGE

static ErlNifResourceType* message_type = 0;


// Called from erl_nif.
void message_dtor(ErlNifEnv* env, void* obj) 
{
    // Free memory
    cloner_destroy((cloner*) obj); 
}



// Called from cloner for each thread.
void message_destr(char* obj) 
{ 
    if (obj != NULL)
        umsg_close((UMessageFormat*) obj);
}
char* message_clone(char* obj) 
{
    UErrorCode status = U_ZERO_ERROR;

    obj = (char*) umsg_clone(
        (UMessageFormat*) obj,
        &status 
    );
    if(U_FAILURE(status)) { 
        return NULL;
    } 
    return obj;
}

int message_open(UMessageFormat * obj, cloner* c)
{
    return cloner_open((char *) obj, c, &message_clone, &message_destr);
} 








/**
 * NIFs
 */

// Get a message format 
static ERL_NIF_TERM open_format(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM out;
    ErlNifBinary in;
    int32_t locale_len; 
    char locale[LOCALE_LEN];
    UErrorCode status = U_ZERO_ERROR;
    UMessageFormat* msg;
    cloner* res;


    locale_len = enif_get_atom(env, argv[0], (char*) locale, LOCALE_LEN, ERL_NIF_LATIN1);
    if (!(locale_len
          && enif_inspect_binary(env, argv[1], &in))) {
        return enif_make_badarg(env);
    }

    // get a message format
    msg = umsg_open((UChar *) in.data, 
            div(in.size, sizeof(UChar)).quot, 
            (char *) locale, 
            NULL, 
            &status);
    CHECK(env, status);


    res = (cloner*) enif_alloc_resource(message_type, sizeof(cloner));
    if (message_open(msg, res)) {
        enif_release_resource(res);
        return enif_make_badarg(env);
    }
    out = enif_make_resource(env, res);
    enif_release_resource(res);
    /* resource now only owned by "Erlang" */
    return out;
}
static ERL_NIF_TERM format(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in, name;
    ERL_NIF_TERM out, list;
    int32_t len, ulen; 
    unsigned char* bin;
    cloner* ptr;
    UMessageFormat* fmt;
    unsigned int count, i;
    UErrorCode status = U_ZERO_ERROR;
    UnicodeString appendTo;

    if(!(enif_get_list_length(env, argv[1], &count)
      && enif_get_resource(env, argv[0], message_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }

    fmt = (UMessageFormat*) cloner_get(ptr);
    if (fmt == NULL) {
        return enif_make_badarg(env);
    }

    if (argc == 3) {
        if (enif_inspect_binary(env, argv[2], &in)) {
            appendTo.append((UChar*) in.data, 
                (int32_t) div(in.size, sizeof(UChar)).quot);
        } else {
            // Third elem is not a string
            return enif_make_badarg(env);
        }
    }

    // Allocate at least one element.  Allocating an array of length
    // zero causes problems on some platforms (e.g. Win32).
    Formattable* args = new Formattable[count ? count : 1];
    UnicodeString* names = new UnicodeString[count ? count : 1];

    i = 0;
    list = argv[1];
    ERL_NIF_TERM* tuple;
    while (enif_get_list_cell(env, list, &out, &list)) {
        // out is a head
        // len is arity
        if (!(enif_get_tuple(env, out, &len, (const ERL_NIF_TERM**) &tuple)
             && (len == 2)
             && enif_inspect_binary(env, tuple[0], &name))) {
            delete[] args;
            delete[] names;
            
            // is not {name, value}
            return enif_make_badarg(env);
        }
        
        names[i].append((UChar*) name.data, 
            (int32_t) div(name.size, sizeof(UChar)).quot);
        
        // reuse name variable as an argument
        if (enif_inspect_binary(env, tuple[1], &name)) {
            args[i].setString(
                * new UnicodeString(
                    (const UChar*) name.data, 
                    (int32_t) div(name.size, sizeof(UChar)).quot));
        } else {
            delete[] args;
            delete[] names;
            
            // wtf?
            return enif_make_badarg(env);
        }
        
        i++;
    }
    
    
    ((const MessageFormat*) fmt)->format(
        (const UnicodeString *) names,
        (const Formattable *) args,
        (int32_t) count,
        (UnicodeString &) appendTo, // hate c++
        (UErrorCode &) status       // hate c++
    );
    delete[] args;
    delete[] names;

    CHECK(env, status);

    const UChar* buf2 = appendTo.getBuffer();
    if (!buf2) {
        return enif_make_badarg(env);
    }

    
    ulen = appendTo.length(); 
    // length in bytes
    len = ulen * sizeof(UChar);

    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy((void*) bin, (void*) buf2, (size_t) len);
    return out;
}







static int i18n_message_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    message_type = enif_open_resource_type(env, NULL, "message_type",
        message_dtor, ERL_NIF_RT_CREATE, NULL); 
    if (message_type == NULL) return 5;
    return 0;
}
#endif






































#ifdef I18N_REGEX

static ErlNifResourceType* regex_type = 0;


// Called from erl_nif.
void regex_dtor(ErlNifEnv* env, void* obj) 
{
    // Free memory
    cloner_destroy((cloner*) obj); 
}



// Called from cloner for each thread.
void regex_destr(char* obj) 
{ 
    if (obj != NULL)
        uregex_close((URegularExpression*) obj);
}
char* regex_clone(char* obj) 
{
    UErrorCode status = U_ZERO_ERROR;

    obj = (char*) uregex_clone(
        (URegularExpression*) obj,
        &status 
    );
    if(U_FAILURE(status)) { 
        return NULL;
    } 
    return obj;
}

int regex_open(URegularExpression * obj, cloner* c)
{
    return cloner_open((char *) obj, c, &regex_clone, &regex_destr);
} 








/**
 * NIFs
 */

// Get a message format 
static ERL_NIF_TERM open_regex(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM out;
    ErlNifBinary in;
    UErrorCode status = U_ZERO_ERROR;
    UParseError* pe = NULL;
    URegularExpression* re;
    cloner* res;

    re = uregex_open((UChar *) in.data, 
            div(in.size, sizeof(UChar)).quot, 
            0, 
            pe, 
            &status);
    CHECK(env, status);

    res = (cloner*) enif_alloc_resource(regex_type, sizeof(cloner));
    if (regex_open(re, res)) {
        enif_release_resource(res);
        return enif_make_badarg(env);
    }
    out = enif_make_resource(env, res);
    enif_release_resource(res);
    /* resource now only owned by "Erlang" */
    return out;
}


static ERL_NIF_TERM regex_replace(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary in, in2;
    ERL_NIF_TERM out;
    int32_t ulen, len; 
    UChar buf[STR_LEN];
    unsigned char* bin;
    URegularExpression* re;
    cloner* ptr;
    UErrorCode status = U_ZERO_ERROR;

    // Second argument must be a binary 
    if(!(enif_inspect_binary(env, argv[1], &in)
      && enif_inspect_binary(env, argv[2], &in2)
      && enif_get_resource(env, argv[0], regex_type, (void**) &ptr))) {
        return enif_make_badarg(env);
    }

    re = (URegularExpression*) cloner_get(ptr);
    uregex_setText(re,
        (const UChar *) in2.data,
        (int32_t) div(in2.size, sizeof(UChar)).quot, // in UChars
        &status); 
    CHECK(env, status);

    ulen = uregex_replaceAll(re,
        (UChar*) in.data, 
        (int32_t) div(in.size, sizeof(UChar)).quot, // in UChars
        (UChar *) buf,
        (int32_t) STR_LEN,
        &status); 
    CHECK(env, status);

    // length in bytes
    len = ulen * sizeof(UChar);


    bin = enif_make_new_binary(env, (size_t) len, &out);
    memcpy(bin, &buf, len);
    return out;
}








static int i18n_regex_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    regex_type = enif_open_resource_type(env, NULL, "regex_type",
        regex_dtor, ERL_NIF_RT_CREATE, NULL); 
    if (regex_type == NULL) return 6;
    return 0;
}
#endif











#ifdef I18N_LOCALE
static ERL_NIF_TERM locale_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UErrorCode status = U_ZERO_ERROR;

    int32_t value_len,         key_len; 
    char    value[LOCALE_LEN], key[LOCALE_LEN];


    key_len = enif_get_atom(env, argv[0], (char*) key, LOCALE_LEN, ERL_NIF_LATIN1);

    if (!key_len) {
        return enif_make_badarg(env);
    }

    
    value_len = uloc_getName((const char*) key, // Locale Id
        (char *)  value, // name
        (int32_t) LOCALE_LEN,
        &status);
    CHECK(env, status);

    return enif_make_atom(env, value);
}
static ERL_NIF_TERM locale_parent(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UErrorCode status = U_ZERO_ERROR;

    int32_t value_len,         key_len; 
    char    value[LOCALE_LEN], key[LOCALE_LEN];


    key_len = enif_get_atom(env, argv[0], (char*) key, LOCALE_LEN, ERL_NIF_LATIN1);

    if (!key_len) {
        return enif_make_badarg(env);
    }

    
    value_len = uloc_getParent((const char*) key, // Locale Id
        (char *)  value, // name
        (int32_t) LOCALE_LEN,
        &status);
    CHECK(env, status);

    return enif_make_atom(env, value);
}
static ERL_NIF_TERM locale_language_tag(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UErrorCode status = U_ZERO_ERROR;

    int32_t value_len,         key_len; 
    char    value[LOCALE_LEN], key[LOCALE_LEN];


    key_len = enif_get_atom(env, argv[0], (char*) key, LOCALE_LEN, ERL_NIF_LATIN1);

    if (!key_len) {
        return enif_make_badarg(env);
    }

    
    value_len = uloc_toLanguageTag((const char*) key, // Locale Id
        (char *)  value, // name
        (int32_t) LOCALE_LEN,
        FALSE,
        &status);
    CHECK(env, status);

    return enif_make_atom(env, value);
}
static ERL_NIF_TERM locale_base_name(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    UErrorCode status = U_ZERO_ERROR;

    int32_t value_len,         key_len; 
    char    value[LOCALE_LEN], key[LOCALE_LEN];


    key_len = enif_get_atom(env, argv[0], (char*) key, LOCALE_LEN, ERL_NIF_LATIN1);

    if (!key_len) {
        return enif_make_badarg(env);
    }

    
    value_len = uloc_getBaseName((const char*) key, // Locale Id
        (char *)  value, // name
        (int32_t) LOCALE_LEN,
        &status);
    CHECK(env, status);

    return enif_make_atom(env, value);
}
#endif















static int load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    int code;
    uloc_getDefault();

#ifdef I18N_STRING
    code = i18n_string_load(env, priv_data, load_info);
    if (code) return code;
#endif


#ifdef I18N_COLLATION
    code = i18n_collation_load(env, priv_data, load_info);
    if (code) return code;
#endif


#ifdef I18N_MESSAGE
    code = i18n_message_load(env, priv_data, load_info);
    if (code) return code;
#endif


#ifdef I18N_REGEX
    code = i18n_regex_load(env, priv_data, load_info);
    if (code) return code;
#endif

     
    return 0;
}

static int reload(ErlNifEnv* env, void** priv, ERL_NIF_TERM load_info)
{
    return 0;
}

static int upgrade(ErlNifEnv* env, void** priv, void** old_priv,
          ERL_NIF_TERM load_info)
{
    return 0;
}

static void unload(ErlNifEnv* env, void* priv)
{
#ifdef I18N_COLLATION
    i18n_collation_unload(env, priv);
#endif
    return;
}










static ErlNifFunc nif_funcs[] =
{
#ifdef I18N_STRING
    {"to_utf8", 1, to_utf8},
    {"from_utf8", 1, from_utf8},
    {"concat", 2, concat},

    // Locale dependible
    {"get_iterator", 2, get_iterator},
    {"len", 2, len},
    {"to_lower", 2, to_lower},
    {"to_upper", 2, to_upper},
    {"to_title", 2, to_title},
#endif





#ifdef I18N_COLLATION
    {"get_collator",      1, get_collator},
    {"sort_key",          2, sort_key},
    {"compare",           3, compare},
    {"collator_set_attr", 3, collator_set_attr},
#endif





#ifdef I18N_MESSAGE
    {"open_format", 2, open_format},
    {"format",      2, format},
    {"format",      3, format},
#endif




#ifdef I18N_REGEX
    {"open_regex", 1, open_regex},
    {"regex_replace", 3, regex_replace},
#endif




#ifdef I18N_LOCALE
    {"locale_name",         1, locale_name},
    {"locale_parent",       1, locale_parent},
    {"locale_language_tag", 1, locale_language_tag},
    {"locale_base_name",    1, locale_base_name},
#endif

};
ERL_NIF_INIT(i18n_nif,nif_funcs,load,reload,upgrade,unload)

