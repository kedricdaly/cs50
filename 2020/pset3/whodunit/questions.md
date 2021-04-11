# Questions

## What's `stdint.h`?

A library with useful "standard" functions. Technically a headerfile.
We use this so that we don't have to repeat the same code other people have already written.

Malloc and related functions are in this library, among others.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

It allows us to set the size of certain keywords/types.

E.g. in the bmp.h headerfile, we define the WORD type as an unsigned, 16-bit integer (uint16_t).
This means each WORD will have 16 bits (2 bytes) with which to store data.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

Type | Number of bytes
-------|--------
'BYTE' | 1 byte
'DWORD' | 4 bytes
'LONG' | 2 bytes
'Word' 

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

Letter|ASCII | Hexadecimal
------|------|------------
B|65|0x41
M|77|0x4D


## What's the difference between `bfSize` and `biSize`?

bfSize: The size, in bytes, of the bitmap file.
    This tells you how big the entire file is.

biSize: The number of bytes required by the structure (BITMAPINFO).
    This tells you how big the BITMAPINFO header is.

## What does it mean if `biHeight` is negative?

If biHeight is negative, the bitmap is a top-down DIB and its origin is the upper-left corner.
    The image is "right-side up" in the bitmap.
(Also implies that, biCompression must be either BI_RGB or BI_BITFIELDS. Top-down DIBs cannot be compressed.)

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount

## Why might `fopen` return `NULL` in `copy.c`?

If we are unable to open the infile or the outfile, NULL may be returned.

## Why is the third argument to `fread` always `1` in our code?

We want to read 1 byte at a time.

## What value does `copy.c` assign to `padding` if `bi.biWidth` is `3`?

int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
Size of RGBTRIPLE is 3 bytes. Multiplied by bi.biWidth of 3 is 9.
9 % 4 = 1.
(4 - 1) % 4 = 3

## What does `fseek` do?

Allows you to rewind or fastforward in a file. (Goes to a specific location).

## What is `SEEK_CUR`?

The current position of the file pointer (where we are currently in the file).
