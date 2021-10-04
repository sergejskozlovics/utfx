#include "utfx.h"

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <wchar.h>
#include <string.h>

#include <assert.h>

// Example form https://docs.microsoft.com/en-us/dotnet/api/system.text.encoding.utf8
// The characters to encode:
//    Latin Small Letter Z (U+007A)
//    Latin Small Letter A (U+0061)
//    Combining Breve (U+0306)
//    Latin Small Letter AE With Acute (U+01FD)
//    Greek Small Letter Beta (U+03B2)
//    a high-surrogate value (U+D8FF)
//    a low-surrogate value (U+DCFF)
WCHAR wchars[] = {L'z', L'a', 0x0306, 0x01FD, 0x03B2, 0xD8FF, 0xDCFF, 0x0000};
CHAR utf8chars[] = {0x7A, 0x61, 0xCC, 0x86, 0xC7, 0xBD, 0xCE, 0xB2, 0xF1, 0x8F, 0xB3, 0xBF, 0x00};
CHAR mutf8chars[] = {0x7A, 0x61, 0xCC, 0x86, 0xC7, 0xBD, 0xCE, 0xB2, 0xED, 0xA3, 0xBF, 0xED, 0xB3, 0xBF, 0x00};
LCHAR utf32chars[] = {0x0000007A, 0x00000061, 0x00000306, 0x000001FD, 0x000003B2, 0x0004FCFF, 0x00000000};

int main(int argc, char *argv[]) {
    size_t n;
    int i;
    int test_result = 0;
    CHAR s[1024];
    WCHAR w[1024];
    LCHAR l[1024];

    printf("testing wchars length\n");
    assert(UTF16Length(wchars) == 7);
    assert(!errno);

    printf("testing utf8 length\n");
    assert(UTF8Length(utf8chars) == 12);
    assert(!errno);

    printf("testing modified utf8 length\n");
    assert(UTF8Length(mutf8chars) == 14);
    assert(!errno);

    printf("testing utf32 length\n");
    assert(UTF32Length(utf32chars) == 6);
    assert(!errno);

    printf("testing wchars length in std UTF-8\n");
    n = UTF16ToStandardUTF8(wchars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 12);

    printf("testing wchars convert to std UTF-8\n");
    UTF16ToStandardUTF8(wchars, 0, s, 1024, 1);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert(strcmp((char *) s, (char *) utf8chars) == 0);

    printf("testing utf32 length in std UTF-8\n");
    n = UTF32ToStandardUTF8(utf32chars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 12);

    printf("testing utf32chars convert to std UTF-8\n");
    UTF32ToStandardUTF8(utf32chars, 0, s, 1024, 1);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert(strcmp((char *) s, (char *) utf8chars) == 0);

    printf("testing wchars length in modif. UTF-8\n");
    n = UTF16ToModifiedUTF8(wchars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 14);

    printf("testing wchars convert to modif. UTF-8\n");
    UTF16ToModifiedUTF8(wchars, 0, s, 1024, 1);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert(strcmp((char *) s, (char *) mutf8chars) == 0);

    printf("testing utf32chars length in modif. UTF-8\n");
    n = UTF32ToModifiedUTF8(utf32chars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 14);

    printf("testing utf32chars convert to modif. UTF-8\n");
    UTF32ToModifiedUTF8(utf32chars, 0, s, 1024, 1);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert(strcmp((char *) s, (char *) mutf8chars) == 0);

    printf("testing wchars length in UTF-32\n");
    n = UTF16ToUTF32(wchars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 6 * 4);

    printf("testing wchars convert to UTF-32\n");
    UTF16ToUTF32(wchars, 0, l, 1024, 0);
    assert(!errno);
    for (i = 0; i < n/4; i++) {
        printf(" %08X", l[i]);
    }
    printf("\n\n");
    assert(memcmp(l, utf32chars, n) == 0);

    printf("testing utf32chars length in UTF-16\n");
    n = UTF32ToUTF16(utf32chars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 7 * 2);

    printf("testing utf32chars convert to UTF-16\n");
    UTF32ToUTF16(utf32chars, 0, w, 1024, 1);
    assert(!errno);
    for (i = 0; i < n/2; i++) {
        printf(" %04X", w[i]);
    }
    printf("\n\n");
    assert(memcmp(w, wchars, n + 2) == 0);

    printf("testing wchars convert to UTF-32 with max=8, with \'\\0\'\n");
    n = UTF16ToUTF32(wchars, 0, l, 8, 1);
    assert(errno==ERANGE);
    assert(n == 8);
    for (i = 0; i < n/4; i++) {
        printf(" %08X", l[i]);
    }
    printf("\n\n");

    printf("testing wchars convert to UTF-32 with max=9, no \'\\0\'\n");
    n = UTF16ToUTF32(wchars, 0, l, 9, 0);
    assert(errno==ERANGE);
    assert(n == 8);
    for (i = 0; i < n/4; i++) {
        printf(" %08X", l[i]);
    }
    printf("\n\n");

    printf("testing UTF-8 length in modified UTF-8\n");
    n = UTF8ToModifiedUTF8(utf8chars, 0, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 14);

    printf("testing wchars convert to modified UTF-8\n");
    UTF8ToModifiedUTF8(utf8chars, 0, s, 1024, 1);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert(strcmp((char*)s, (char*)mutf8chars) == 0);

    printf("testing UTF-8\' length in modified UTF-8\n");
    n = UTF8ToModifiedUTF8(utf8chars, 13, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 14 + 2);

    printf("testing wchars convert to modif. UTF-8\n");
    UTF8ToModifiedUTF8(utf8chars, 13, s, 1024, 0);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert((memcmp(s, mutf8chars, 14) == 0) && (s[14] == 0xC0) && (s[15] == 0x80));

    printf("testing modified UTF-8\' length in UTF-16\n");
    n = UTF8ToUTF16(s, 16, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == (7 + 1) * 2);

    printf("testing modified UTF-8\' convert to UTF-16\n");
    UTF8ToUTF16(s, 16, w, 1024, 0);
    assert(!errno);
    for (i = 0; i < n/2; i++) {
        printf(" %04X", w[i]);
    }
    printf("\n\n");
    assert((memcmp(w, wchars, 7*2) == 0) && (w[7] == 0x0000));

    printf("testing modified UTF-8\' length in std. UTF-8\n");
    n = UTF8ToStandardUTF8(s, 16, NULL, UTFX_MAX_SIZE, 0);
    assert(!errno);
    assert(n == 12 + 1);

    printf("testing modified UTF-8 convert to std. UTF-8\n");
    UTF8ToStandardUTF8(s, 16, s, 1024, 0);
    assert(!errno);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert((strncmp((char*)s, (char*)utf8chars, 12) == 0) && (s[12] == 0x00));

    printf("testing modified UTF-8\' length in std. UTF-8 with max=3, no \'\\0\'\n");
    n = UTF8ToStandardUTF8(utf8chars, 16, NULL, 3, 0);
    assert(errno==ERANGE);
    assert(n == 2);

    UTF8ToStandardUTF8(utf8chars, 16, s, 3, 0);
    printf("testing modified UTF-8 convert to std. UTF-8\n");
    assert(errno==ERANGE);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert((strncmp((char*)s, (char*)utf8chars, 2) == 0));

    printf("testing modified UTF-8\' length in std. UTF-8 with max=4, with \'\\0\'\n");
    n = UTF8ToStandardUTF8(utf8chars, 12, NULL, 4, 1);
    assert(errno==ERANGE);
    assert(n == 3);

    printf("testing modified UTF-8 convert to std. UTF-8\n");
    UTF8ToStandardUTF8(utf8chars, 12, s, 4, 1);
    assert(errno==ERANGE);
    for (i = 0; i < n; i++) {
        printf(" %02X", s[i]);
    }
    printf("\n\n");
    assert((strncmp(s, utf8chars, 2) == 0) && (s[2] == '\0'));

    printf("all passed\n");
    return 0;
}

