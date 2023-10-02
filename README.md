# m_json
`m_json` is a lightweight JSON parser implemented in C, created primarily as a learning experience. This parser provides a set of functions to parse JSON strings, manipulate JSON structures, and convert them to and from string representations.

## JSON structs and enums
```c
typedef struct m_json {
    enum m_json_type type;
    struct m_json* prev;
    struct m_json* next;
    char* key;
    struct m_json* child;
    char* string;
    double number;
} m_json;

enum m_json_type {
    m_json_NULL,
    m_json_TRUE,
    m_json_FALSE,
    m_json_STRING,
    m_json_NUMBER,
    m_json_ARRAY,
    m_json_OBJECT,
};
```
The JSON struct variables are the following:
* `type` - type of the node
* `prev` - previous node in the JSON tree
* `next` - next node in the JSON tree
* `key` - key of the JSON object item (available only when `type` of parent is equal to `m_json_OBJECT`)
* `child` - items of the object or array (available only when `type` is equal to `m_json_OBJECT` or `m_json_ARRAY`)
* `string` - data string (available only when `type` is equal to `m_json_STRING`)
* `number` - data number (available only when `type` is equal to `m_json_NUMBER`)

## Functions
```c
m_json* m_json_parse_object(char* json);
m_json* m_json_parse_array(char* json);
m_json* m_json_parse(char* json); // parses string -- returns NULL if an error occurred.
void m_json_push_child(m_json* json, m_json* child); 
m_json* m_json_create();
char* m_json_to_str(m_json* json); // remember to call free on the string
void m_json_free(m_json* root);
```
Remember to destroy any m_json* objects with m_json_free().
## Usage
### Editing JSON
```c
m_json* json = m_json_parse_object(str);
if (json == NULL) return 1;
json->child->key = "edited key name";
char* json_str = m_json_to_str(json); // edited json string
printf("%s\n", json_str);
free(json_str);
m_json_free(json);
```
### Creating JSON
```c
m_json* json = m_json_create();
json->type = m_json_ARRAY;
for (int i=0; i < 15; i++) {
    m_json* array_item = m_json_create();
    array_item->type = m_json_NUMBER;
    array_item->number = i;
    m_json_push_child(json, array_item);
}
char* str = m_json_to_str(json);
printf("%s\n", str);
free(str);
m_json_free(json);
```
#### Output
```json
[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14]
```
## Known issues
* Doesn't convert unicode control characters (eg; `\u1234`)
* Doesn't conform to the spec exactly in many areas.

Feel free to use and modify this JSON parser for your learning purposes. Keep in mind that it may not be suitable for production use due to the known issues and limitations.
