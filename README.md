[![License](https://img.shields.io/badge/license-MIT-green)](./LICENSE)

# utfx

The utfx library is able to convert unicode strings between UTF-8, modified UTF-8 (used by Java), UTF-16 and UTF-32 encodings. Additional features:

* the ability to compute the lengths of these strings in Unicode *characters* (not bytes, words, or quadrabytes);
* functions for accessing files and folders in UTF-8 (`fopen_utf8`, `mkdir_utf8`, etc.);
* the library prevents buffer overflow errors by being able to consider the available size of the target buffer.

The library is written in plain C (99). You only need `utfx.h` and `utfx.c`.

The `tests` subdirectory contains the tests, which can be used as an alternate documentation on how to invoke the functions provided by utfx.

The library comes with the CMake build file (`CMakeLists.txt`).

## utfx.h

The utfx.h header file defines 3 cross-platform datatypes:

* `CHAR`, a one-byte UTF-8 character
* `WCHAR`, a two-bytes UTF-16 character (a "wide" character)
* `LCHAR`, a four-bytes UTF-32 character (a "long" character)

Notice that `wchar_t` is 2 bytes in Windows but 4 bytes in *NIX. Thus, we encourage using our types `WCHAR` and `LCHAR` instead.

Code points may be in the range U+000000 to U+10FFFF. In case of an illegal sequence, errno is set to `EILSEQ` but we will still try to preserve all the characters, although the target sequence might also become illegal.

The following functions are defined in `utfx.h` and implemented in `utfx.c`.

## Length functions

```c
size_t UTF8Length(const void* source);
size_t UTF16Length(const void* source);
size_t UTF32Length(const void* source);
```

These functions return the length of the string in Unicode *characters* (not bytes).

## Conversion functions

```c
size_t UTF8ToStandardUTF8(const void* source, size_t utf8lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF8ToModifiedUTF8(const void* source, size_t utf8lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF8ToUTF16(const void* source, size_t utf8lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF8ToUTF32(const void* source, size_t utf8lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);

size_t UTF16ToStandardUTF8(const void* source, size_t utf16lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF16ToModifiedUTF8(const void* source, size_t utf16lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF16ToUTF32(const void* source, size_t utf16lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);

size_t UTF32Length(const void* source);
size_t UTF32ToStandardUTF8(const void* source, size_t utf32lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF32ToModifiedUTF8(const void* source, size_t utf32lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
size_t UTF32ToUTF16(const void* source, size_t utf32lengthInUnits, void* target, size_t targetSizeInBytes, int writeTerminatingNull);
```

All functions take the following arguments:

* **source** - a pointer to a source string:
  * for UTF8ToXXX functions - either standard, or modified UTF-8 string (null-terminated or not);
  * for UTF16ToXXX functions - a UTF-16 (little-endian or big-endian according to the *current* compiler target), null-terminated or not;
  * for UTF32ToXXX functions - a UTF-32 (little-endian or big-endian according to the *current* compiler target), null-terminated or not;
  
* **utfXXlengthInUnits** - 0 to use a terminating '\0' to determine the length of the source string (the source must be null-terminated), or non-zero value to use it as a length (UTF-8 units = bytes, UTF-16 units = double bytes, UTF-32 units = quadrabytes) despite of a possible presence of '\0' in the middle of the source string (the source may be null-terminated or not);
  
* **target** - a buffer for the result or NULL, if only the size of the result is needed;
  
* **targetSizeInBytes** - the size of the target buffer in *bytes* or an upper bound for the target size if target == NULL; we will return the result, which is <= targetSizeInBytes  (errno may be set to `ERANGE`, if targetSizeInBytes is not sufficient); if (target != NULL) we will write no more than targetSizeInBytes bytes into the target buffer;
  
* **writeTerminatingNull** - a boolean (0 or 1) specifying whether we need to write a terminating '\0' *unit* (1, 2, or 4 bytes depending on the target encoding) into the target buffer (only when target != NULL and targetSizeInBytes is not less than a space needed for one target unit).



**Return value:** All functions return the size of the target result in *bytes*.

## Functions for working with files and folders in UTF-8

The following functions with the `_utf8` postfix are provided by the library. The semantics correspond to the corresponding functions from the C standard library.

```c
FILE *fopen_utf8(const char *utf8FileName, const char *mode);
int mkdir_utf8(const char *utf8Path);
int chdir_utf8(const char* utf8Path);
char *getcwd_utf8(char *buf, size_t buflen); // on error returns NULL

void *opendir_utf8(const char *utf8DirName);
int readdir_utf8(void *hOpenDir); // returns 1 (true) or 0 (false)
char *get_d_name_utf8(void *hOpenDir);
void closedir_utf8(void *hOpenDir);

char *getenv_utf8_malloc(const char *utf8name); // the result (if not NULL) must be freed by free()
```

