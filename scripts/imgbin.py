import argparse
from PIL import Image
import pathlib

class ImgBin:
    # VGA color constants (each value is 0xXX; lower nibble is the 4bpp index)
    VGA_PALETTE = {
        'black':     0x00,
        'blue':      0x11,
        'green':     0x22,
        'cyan':      0x33,
        'red':       0x44,
        'magenta':   0x55,
        'brown':     0x66,
        'ltgray':    0x77,
        'dkgray':    0x88,
        'ltblue':    0x99,
        'ltgreen':   0xAA,
        'ltcyan':    0xBB,
        'ltred':     0xCC,
        'ltmagenta': 0xDD,
        'yellow':    0xEE,
        'white':     0xFF
    }

    VGA_RGB = {
        'black':     (0x00, 0x00, 0x00),
        'blue':      (0x00, 0x00, 0xAA),
        'green':     (0x00, 0xAA, 0x00),
        'cyan':      (0x00, 0xAA, 0xAA),
        'red':       (0xAA, 0x00, 0x00),
        'magenta':   (0xAA, 0x00, 0xAA),
        'brown':     (0xAA, 0x55, 0x00),
        'ltgray':    (0xAA, 0xAA, 0xAA),
        'dkgray':    (0x55, 0x55, 0x55),
        'ltblue':    (0x55, 0x55, 0xFF),
        'ltgreen':   (0x55, 0xFF, 0x55),
        'ltcyan':    (0x55, 0xFF, 0xFF),
        'ltred':     (0xFF, 0x55, 0x55),
        'ltmagenta': (0xFF, 0x55, 0xFF),
        'yellow':    (0xFF, 0xFF, 0x55),
        'white':     (0xFF, 0xFF, 0xFF)
    }

    @staticmethod
    def get_binary(img: Image.Image) -> bytes:
        """
        Converts the image into a packed 4bpp binary representation.
        Each pixel is mapped to a 4-bit value (the lower nibble of the VGA palette value),
        and two pixels are packed into one byte.
        """

        pixels = img.getdata()
        num_pixels = len(pixels)
        nibble_values = [0] * num_pixels  # will store the 4-bit values for each pixel
        cache = {}
        vga_palette = ImgBin.VGA_PALETTE
        vga_rgb_items = list(ImgBin.VGA_RGB.items())

        # Compute the 4-bit index for each pixel.
        # Since each VGA_PALETTE value is of the form 0xXX,
        # shifting right by 4 yields the index (0x00->0, 0x11->1, ..., 0xFF->15)
        for idx, pixel in enumerate(pixels):
            if pixel in cache:
                nibble_values[idx] = cache[pixel]
            else:
                r, g, b = pixel
                # Initialize with the first color from the palette
                color_name, (vr, vg, vb) = vga_rgb_items[0]
                min_distance = (r - vr) ** 2 + (g - vg) ** 2 + (b - vb) ** 2
                best_value = vga_palette[color_name]
                # Find the closest VGA color
                for color_name, (vr, vg, vb) in vga_rgb_items[1:]:
                    d = (r - vr) ** 2 + (g - vg) ** 2 + (b - vb) ** 2
                    if d < min_distance:
                        min_distance = d
                        best_value = vga_palette[color_name]
                # Get the 4-bit index by shifting the value right 4 bits
                color_index = best_value >> 4
                cache[pixel] = color_index
                nibble_values[idx] = color_index

        # Pack two 4-bit values into one byte
        packed_length = (num_pixels + 1) // 2
        output = bytearray(packed_length)
        for i in range(0, num_pixels, 2):
            if i + 1 < num_pixels:
                # Upper nibble: first pixel, lower nibble: second pixel
                output[i // 2] = (nibble_values[i] << 4) | (nibble_values[i + 1] & 0x0F)
            else:
                # For odd number of pixels, pad the lower nibble with 0
                output[i // 2] = (nibble_values[i] << 4)
        return bytes(output)

    @staticmethod
    def convert_file(input_file, verbose=False):
        # Only process BMP files
        if input_file.suffix.lower() != '.bmp':
            if verbose:
                print(f"Skipping {input_file.name}: not a BMP file.")
            return False

        try:
            output_file = str(input_file.with_suffix('.bin'))
            img = Image.open(input_file).convert('RGB')

            if verbose:
                print(f"\nProcessing {input_file.name}...")
                print(f"Image size: {img.size[0]}x{img.size[1]} pixels")

            binary_data = ImgBin.get_binary(img)
            with open(output_file, 'wb') as f:
                f.write(binary_data)

            if verbose:
                print(f"Successfully converted to {output_file}")
                print(f"Output file size: {len(binary_data)} bytes")
            return True

        except Exception as e:
            print(f"Error processing {input_file.name}: {str(e)}")
            return False

    @staticmethod
    def convert_directory(directory, verbose=False):
        dir_path = pathlib.Path(directory)
        if not dir_path.is_dir():
            print(f"Error: {directory} is not a directory")
            return False

        # Search only for BMP files
        bmp_files = list(dir_path.glob("*.bmp"))
        if not bmp_files:
            print(f"No BMP files found in {directory}")
            return False

        if verbose:
            print(f"Found {len(bmp_files)} BMP files in {directory}")

        success_count = sum(1 for f in bmp_files if ImgBin.convert_file(f, verbose))

        if verbose:
            print(f"\nProcessing complete: {success_count}/{len(bmp_files)} files converted successfully")
        return success_count > 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Convert BMP images (4bpp) to a packed 4bpp binary format')
    parser.add_argument('input', help='BMP file or directory containing BMP files')
    parser.add_argument('-v', '--verbose', action='store_true', help='Enable verbose output')
    args = parser.parse_args()

    input_path = pathlib.Path(args.input)
    if input_path.is_dir():
        ImgBin.convert_directory(input_path, args.verbose)
    else:
        ImgBin.convert_file(input_path, args.verbose)
