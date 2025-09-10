# Simple Binary Format (SBF)

A simple file data format, created as a hobby.

## Usage

Reading from a file:
```cpp
    unsigned char version;  // optional
    Node *node = SBF_ReadFile("data.sav", &version);

    switch (SBF_GetNodeType(node)) {
        // ...
    }

    SBF_DestroyNode(node);

```
Write a node to a file:
```cpp
    Node *node = ...;

    SBF_WriteFile("data.sav", node);

    SBF_DestroyNode(node);
```

There're no complete examples of usage for now.

## The layout

The binary data is written in little-endian order. The library -when compiled- automatically adjusts to the CPU's architecture.
The file always begins with a byte indicating the format version followed by the rest of the data.

### The body

Data is split into chunks, sorrounded by an opening tag and closing tag.
There are multiple types of tags containing different types of data:

- 32-bit integer
- 64-bit integer
- 32-bit float
- 64-bit float
- Byte
- Unsigned 32-bit integer
- Unsigned 64-bit integer
- Unsigned byte
- Array of one of the previously mentioned types
- Table

In binary, the tag is a single byte indicating the type and whether it's closing or not:

| Binary | Decimal | Type |Name  |Size (bytes) |
| ------ | ------- | ---- | ---- | ----------- |
|00000001|1|opening|32-bit integer tag|4|
|00000010|2|opening|64-bit integer tag|8|
|00000011|3|opening|32-bit float tag|4|
|00000100|4|opening|64-bit float tag|8|
|00000101|5|opening|byte tag|1|
|00000110|6|opening|unsigned 32-bit integer tag|4|
|00000111|7|opening|unsigned 64-bit integer tag|8|
|00001000|8|opening|unsigned byte tag|1|
|00001001|9|opening|character tag|1|
|00001010|10|opening|32-bit integer array tag|8 + 4*length|
|00001011|11|opening|64-bit integer array tag|8 + 8*length|
|00001100|12|opening|32-bit float array tag|8 + 4*length|
|00001101|13|opening|64-bit float array tag|8 + 8*length|
|00001111|14|opening|byte array tag|8 + length|
|00001110|15|opening|unsigned 32-bit integer array tag|8 + 4*length|
|00010000|16|opening|unsigned 64-bit integer array tag|8 + 8*length|
|00010001|17|opening|unsigned byte array tag|8 + length|
|00010010|18|opening|string tag|8 + length|
|00010011|19|opening|table|*|

| Binary | Decimal | Type |Name  |Size (bytes) |
| ------ | ------- | ---- | ---- | ----------- |
|11111111| -1|closing|32-bit integer tag|4|
|11111110| -2|closing|64-bit integer tag|8|
|11111101| -3|closing|32-bit float tag|4|
|11111100| -4|closing|64-bit float tag|8|
|11111011| -5|closing|byte tag|1|
|11111010| -6|closing|unsigned 32-bit integer tag|4|
|11111001| -7|closing|unsigned 64-bit integer tag|8|
|11111000| -8|closing|unsigned byte tag|1|
|11110111| -9|closing|character tag|1|
|11110110|-10|closing|32-bit integer array tag|8 + 4*length|
|11110101|-11|closing|64-bit integer array tag|8 + 8*length|
|11110100|-12|closing|32-bit float array tag|8 + 4*length|
|11110011|-13|closing|64-bit float array tag|8 + 8*length|
|11110010|-14|closing|byte array tag|8 + length|
|11110001|-15|closing|unsigned 32-bit integer array tag|8 + 4*length|
|11110000|-16|closing|unsigned 64-bit integer array tag|8 + 8*length|
|11101111|-17|closing|unsigned byte array tag|8 + length|
|11101110|-18|closing|string tag|8 + length|
|11101101|-19|closing|table|*|

Each array opening tag is followed by a 8 bytes indicating the length (64-bit unsigned integer). 
Each opening/closing tag has a size of 1 byte.
2 bytes are added to the size calculation to any data type.

A table is a list of string/node pairs. The library does not account for duplicate keys. The library was literally written in a span of days, so there's nothing special about it.
