### Cursor shape values (4 bits):

| hex  | dec | shape           | size       | status   |
|------|-----|-----------------|------------|----------|
| 0x00 | 0   | block cursor    | full       | blinking |
| 0x01 | 1   | block cursor 2  | big        | blinking |
| 0x02 | 2   | block cursor 3  | big        | blinking |
| 0x03 | 3   | block cursor 4  | big        | blinking |
| 0x04 | 4   | block cursor 5  | big        | blinking |
| 0x05 | 5   | block cursor 6  | big        | blinking |
| 0x06 | 6   | block cursor 7  | big        | blinking |
| 0x07 | 7   | block cursor 8  | medium     | blinking |
| 0x08 | 8   | block cursor 9  | medium     | blinking |
| 0x09 | 9   | block cursor 10 | medium     | blinking |
| 0x0A | 10  | block cursor 11 | normal     | blinking |
| 0x0B | 11  | line cursor     | little     | blinking |
| 0x0C | 12  | line cursor 2   | little     | blinking |
| 0x0D | 13  | line cursor 3   | little     | blinking |
| 0x0E | 14  | line cursor 4   | little     | blinking |
| 0x0F | 15  | hidden cursor   | too little | hidden   |

- The values ​​actually change the size of the cursor, going from a full block, to a medium one, then little, until finally disappearing
