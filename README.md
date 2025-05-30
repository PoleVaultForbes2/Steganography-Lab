# Steganography-Lab
This project is a simple C program that hides a secret message inside a 32-bit BMP image using Least Significant Bit (LSB) steganography. The program modifies the LSB of each RGB byte in the image to encode the bits of your message while keeping the image visually unchanged.

📌 Features

Supports 32-bit uncompressed BMP images.

Encodes a null-terminated secret message inside the image's pixel data.

Preserves the original BMP header and metadata.

Simple command-line usage.

🛠️ How It Works

The encoder modifies the least significant bit (LSB) of each byte in the image’s RGB pixel data to store the bits of the secret message. It encodes one bit of the message per color channel (R, G, B), cycling through the bytes of the message until all are embedded, including the null terminator (\0).


🖥️ Usage

Compile the program:

~~~~~~~~~~~~~~~~~~~~~~~
gcc -o bmp_encoder main.c
~~~~~~~~~~~~~~~~~~~~~~~~
Run the encoder:

./bmp_encoder input.bmp output.bmp "Your secret message"


input.bmp — Original BMP image (must be 32-bit and uncompressed).

output.bmp — New BMP image with the encoded message.

"Your secret message" — The message to be hidden inside the image.


📄 License
This project is provided for educational purposes and is licensed under the MIT License.

