# wiegand-26bit-calculator
Input any 26bit Wiegand form i.e Facility code, and unique code, Hex, binary or Proxmark TagID the output will be all corresponding formats. This can be helpfull when cloning HID cards when you only have the facility code and unique code. The Proxmark ID is a 'special' hex code generated in the client application
```
Variables:
-p Proxmark generated TagID
-f Facility code of the HID tag. Must use with -u
-u Unique code of the HID tag. Must use with -f
-x Hex value of the 26bit HID tag
-b Binary value of the 26bit HID tag
```
```
Examples:
./wiegland-calculator -p 2005c7c200
./wiegland-calculator -f 227 -u 57600
./wiegland-calculator -x 1C7C200
./wiegland-calculator -b 01110001111100001000000000
```

```
Output Example:
-----------  Wiegland Card Information  -----------
| Facility Code                               227 |
| Unique Code                               57600 |
| Proxmark Code                        2005c7c200 |
| Hex Value                               1C7C200 |
| Binary Value         01110001111100001000000000 |
---------------------------------------------------
```

The current compiled excutable was compiled on MacOS 10.11.6 with gcc ```Apple LLVM version 7.3.0 (clang-703.0.31)```
Have not tested it yet on Linux
