# edJSON
A very small footprint, low-to-none memory usage, event-driven JSON decoding library written in pure C.

## Overview
edJSON is a very small yet very powerful JSON-parsing **static** library.

While originally it was intended for usage in embedded applications, it is highly used in applications which need to parse information from JSON files, and don't need complicated data structures to do so.

### Main features
- **low footprint**: target is under 1000 C-code lines
- **low memory usage**: besides the memory provided upon calling, the parser uses less than 64 bytes of memory
- **event-driven**: instead of building a memory-costly tree with data, the parser calls a predefined callback upon finding any JSON values
- **easy to use**: check examples and [Usage](#Usage) section
- **powerful parser**: while the parser was intended to be used on small JSON content, in embedded applications, it does not actually have any limitations

### How it works ?
The idea behind **event-driven** part of the libarary is that on embedded systems there's usually little memory to be spared for different operations. If you're parsing a big JSON file just to get a specific value with a traditional parser, the parser will build up a big tree structure, having all the data from the JSON inside, practically duplicating both data and memory usage.

With **edJSON** the parser runs through the JSON content and whenever a value is found, a **callback** (aka "event") is called back to the user, having the value and the JSON path where the value was found. This way, there is no extra memory usage, especially for costly strings.

Inside the callback, the user can return 0 for the parser to continue parsing, or if the required value was found, the user can return a non-null value and the praser will gracefully stop.
## Usage
### Installation
edJSON is so small that it's always best to just include the two source files (*edJSON.h* and *edJSON.c*) inside your project. The only dependencies they have is the standard library, or at least availablity to the following functions:
- strchr()
- atof()
- atoll()
- snprintf() - can be easily modified with simple code

### Parsing
Parsing is done by the edJSON_parse() function:
```
int edJSON_parse(const char *json, edJSON_path_t *path_mem, size_t path_max_depth, edJSON_cb_t jsonEvent);
```

Parser parameters are:
- **json** - a null-terminated char array containing the JSON content; the parser itself **does not modify this content**. Howver, be aware that the user may choose to actually modify it inside the callback for even further memory optimization.
- **path_mem** - pointer to a pre-allocated (by the user) array in which the parser will put path information
- **path_max_depth** - the size of the path_mem array; also corresponds to the maximum depth that the parser can move inside the JSON content; if the JSON content has more depth than this number, then **EDJSON_ERR_NO_MEMORY** is returned
- **jsonEvent** - pointer to the user-defined callback which will be called by the parser whenever a value is found; if this pointer is null, no callback will be called; this is a special usage case and is used to validate the JSON content (return code will still have meaning).

Parser return codes are:
- **EDJSON_SUCCESS** - successfuly parsed the JSON content
- **EDJSON_ERR_NO_MEMORY** - ran out of memory while parsing; this means that the path_max_depth value is too low for the current JSON content
- **EDJSON_ERR_NO_INPUT** - pointers passed to the parser are invalid
- any number greater than 0 still measn **UNSUCCESFUL**, and represents the character index in the **json** char array which triggered the error


### Event callback
Event callback definition is the following:
```
int edJSON_callback(const edJSON_path_t *path, size_t path_size, edJSON_value_t value);
```

Through the callback, the parser sends the following parameters to the user:
- **path** - an array of *edJSON_path_t* elements which will compose the current JSON path; see description of paths in  [Callback callback](#Callback-callback)
- **path_size** - the number of elements in path - also reflects the current parsing depth
- **value** - found value; see description of value [Callback value](#Callback-value)

The callback expects the user to return an integer:
- null (0) if the parser should continue to parse the JSON content
- non-null if the praser should successfully stop parsing

### Callback path
The current path is provided by the parser to the user via the event callback (see [Event callback](#Event-callback)). It is provided as an array of path nodes, and a path node is described by the structure below.
```
typedef struct {
    char _prev;             /**< internal data, do not use and do not modify. */

    int index;              /**< -1 if value is non-array, otherwise current array index. */
    const char *value;      /**< Path value. Exists only when index < 0. NOT null-terminated. */
    size_t value_size;      /**< Value size. Exists only when index < 0. */
} edJSON_path_t;
```

NOTE that the value string **IS ESCAPED**.

### Callback value
The current value is provided by the parser to the user vla the event callback (see [Event callback](#Event-callback)). The value is represented by the structure below:
```
typedef struct {
    enum {
        EDJSON_VT_INTEGER,      /**< Returned value is an integer. */
        EDJSON_VT_STRING,       /**< Returned value is a string. */
        EDJSON_VT_DOUBLE,       /**< Returned value is a double. */
        EDJSON_VT_BOOL,         /**< Returned value is a boolean. */
    } value_type;               /**< Returned value type. */

    union {
        int integer;            /**< Union member to read if value_type is integer. */
        struct {
            const char *value;  /**< String value. NOT null-terminated */
            size_t value_size;  /**< String value size. */
        } string;               /**< Union member to read if value_type is string. */
        double floating;        /**< Union member to read if value_type is double. */
        bool boolean;           /**< Union member to read if value_type is boolean. */
    } value;                    /**< Returned value. To see which type it is, check value_type. */
} edJSON_value_t;
```
When the user needs the value, value_type needs to be checked first, and depending on it the value is represented by one of the union members.

NOTE that the value string **IS ESCAPED**.


### Utility functions
```
int edJSON_string_unescape(char *dest, size_t dest_size, const char *source, size_t source_size);
```
**edJSON_string_unescape** builds an unescaped version of the string. 
- the input of this function is *source* and *source_size* and they should be the exact values from either edJSON_path_t or edJSON_value_t structures.
- the output of this function is *dest* and *dest_size*:
    - **dest** is a pointer to a pre-allocated (by user) memory location to put the result; the result is a C-style null-terminated string
    - **dest_size** is the total size of the pre-allocated memory
- return value is either positive for success, representing the number of characters (excluding the null-character) written in **dest**, or a negative value (see return codes) if an error occured (EDJSON_ERR_NO_MEMORY, EDJSON_ERR_NO_INPUT or EDJSON_ERR_BAD_STRING).

**NOTES and SPECIAL CASES**:

**dest** can be the same as **source** (after casting, to ignore the "const" part - !!!BE AWARE for volatile optimizations by the compiler!!!), in which case the memory is reused; this is an optimization to not use further memory for unescaping a string; however, this "trashes" the parser's **json** char array, making it unparsable once more. This can be acceptable if the parsing of a content is done only once, and useful values are extracted from the JSON, and then the content is not needed any more.

```
int edJSON_build_path_string(char *dest, size_t dest_size, const edJSON_path_t *path, size_t path_size);
```
**edJSON_build_path_string** builds a C-style null-terminated full path from a provided path array. This is very useful for using standard C string manipulation functions on the path itself. The function requires additional memory provided by the user in which the result will be stored.
- the input of this function is *path* and *path_size* and they should be the exact values provided by the jsonEvent callback.
- the output of this function is *dest* and *dest_size*:
    - **dest** is a pointer to a pre-allocated (by user) memory location to put the result; the result is a C-style null-terminated string
    - **dest_size** is the total size of the pre-allocated memory
- return value is either positive for success, representing the number of characters (excluding the null-character) written in **dest**, or a negative value (see return codes) if an error occured (EDJSON_ERR_NO_MEMORY, EDJSON_ERR_NO_INPUT or EDJSON_ERR_BAD_STRING).

## Release notes
### version 1.0.0
- initial release
- not yet supported: null, \u-escaped values, pure-array JSON
