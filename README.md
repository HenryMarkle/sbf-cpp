# Simple Binary Format (SBF)

## The layout

Little endian binary order.

## Usage

```cpp
    // Read from a file

    unsigned char version;  // optional
    Node *node = SBF_ReadFile("data.sav", &version);
    
    SBF_DestroyNode(node);

```

```cpp
    // Write a node to a file

    SBF_WriteFile("data.sav", node);
```

### The header

- 1 byte for the format version.

### The body

Data is split into chunks or "tags", sorrounded by an opening tag and closing tag.
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

Binary|Decimal|Type|Name
------------------------
00000001|1|opening|32-bit integer tag|
00000010|2|opening|64-bit integer tag|
00000011|3|opening|32-bit float tag|
00000100|4|opening|64-bit float tag
00000101|5|opening|byte tag
00000110|6|opening|unsigned 32-bit integer tag
00000111|7|opening|unsigned 64-bit integer tag
00001000|8|opening|unsigned byte tag
00001001|9|opening|character tag
00001010|10|opening|32-bit integer array tag
00001011|11|opening|64-bit integer array tag
00001100|12|opening|32-bit float array tag
00001101|13|opening|64-bit float array tag
00001111|14|opening|byte array tag
00001110|15|opening|unsigned 32-bit integer array tag
00010000|16|opening|unsigned 64-bit integer array tag
00010001|17|opening|unsigned byte array tag
00010010|18|opening|string tag
00010011|19|opening|table

11111111 ( -1) (closing) 32-bit integer tag
11111110 ( -2) (closing) 64-bit integer tag
11111101 ( -3) (closing) 32-bit float tag
11111100 ( -4) (closing) 64-bit float tag
11111011 ( -5) (closing) byte tag
11111010 ( -6) (closing) unsigned 32-bit integer tag
11111001 ( -7) (closing) unsigned 64-bit integer tag
11111000 ( -8) (closing) unsigned byte tag
11110111 ( -9) (closing) character tag
11110110 (-10) (closing) 32-bit integer array tag
11110101 (-11) (closing) 64-bit integer array tag
11110100 (-12) (closing) 32-bit float array tag
11110011 (-13) (closing) 64-bit float array tag
11110010 (-14) (closing) byte array tag
11110001 (-15) (closing) unsigned 32-bit integer array tag
11110000 (-16) (closing) unsigned 64-bit integer array tag
11101111 (-17) (closing) unsigned byte array tag
11101110 (-18) (closing) string tag
11101101 (-19) (closing) table

Each array opening tag is followed by a 8 bytes indicating the length (64-bit unsigned integer). 
