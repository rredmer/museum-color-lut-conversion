# Color LUT Conversion

This is a 24-Bit Color Lookup Table (LUT) conversion utility for device color calibration (1994).

## Overview

The `LUTUTIL.C` utility translates LUT data between binary tables and comma-delimited text files. It was originally developed for the ColorFlex Data System (CDS) and Enhanced Imaging Editor equipped with the JVC TK-F7300U camera.

## Features

- Convert BrookTree LUT binary files to text format
- Convert text files back to BrookTree LUT format
- Support for 256-entry RGB color tables with alpha channel
- Linear LUT table generation capability

## File Structure

```
src/
  LUTUTIL.C       - Main source code
  LUTUTIL.EXE     - Compiled executable
  LUTUTIL.OBJ     - Object file
tables/           - Sample LUT and text files
  DISPLAY.LUT     - Display calibration table
  DMIN1.LUT       - Minimum density table 1
  HRCAP.LUT       - High resolution capture table  
  LINEAR.LUT      - Linear response table
  NEGA.LUT        - Negative film table
  *.TXT           - Corresponding text versions
```

## Building

Compile `LUTUTIL.C` using a C compiler. Example using GCC:

```bash
gcc -o LUTUTIL src/LUTUTIL.C
```

## Usage

The utility accepts the following command line arguments:

```bash
LUTUTIL lut_file text_file <switches>

Switches:
  /l  - Convert text file to LUT format
  /t  - Convert LUT file to text format
```

### Examples

Convert LUT to text:
```bash
LUTUTIL tables/DISPLAY.LUT tables/DISPLAY.TXT /t
```

Convert text to LUT:
```bash
LUTUTIL tables/LINEAR.TXT tables/LINEAR.LUT /l
```

## Text File Format

The text files use comma-separated values with one RGBA entry per line:
```
R,G,B,A
255,255,255,0
254,254,254,0
...
```

## License

This project is licensed under the MIT License - see the [LICENSE](museum-color-lut-conversion/LICENSE) file for details.

## Author

Ronald D. Redmer (1994)