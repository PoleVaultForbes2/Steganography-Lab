#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#pragma pack(1) //turn on packing
typedef struct {
    uint16_t Type;
    uint32_t Size;
    uint16_t Reserved1; 
    uint16_t Reserved2;
    uint32_t startBits; //Start of pixel data
}BMPHeader;

typedef struct {
    uint32_t Size;
    int32_t Width;  //Image width
    int32_t Height;  //Image height
    uint16_t Planes; 
    uint16_t BitCount; 
    uint32_t Compression;
    uint32_t SizeImage; //size of the image data
    int32_t XPelsPerMeter;
    int32_t YPelsPerMeter;
    uint32_t ColorUsed;
    uint32_t ColorImportant;
}DIBHeader;
#pragma pack() //turn off packing

// change the LSB of the byte, to the bit of the secret message
uint8_t setLSB(uint8_t byte, uint8_t bit){
    //set the LSB to 0 using 0xFE, then change it to the passed in bit
    return (byte & 0xFE) | (bit & 0x01); 
}

int main(int argc, char *argv[]){
    // Check for right arguemnts
    if(argc != 4){
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    //Open the BMP file
    FILE *image = fopen(argv[1], "rb");
    if(image == NULL){
        printf("Error: Could not open the image file %s\n", argv[1]);
        return 1;
    }

    //Open the encoded file
    FILE *outFile = fopen(argv[2], "wb");
    if(outFile == NULL){
        printf("Error: Could not open the output file %s\n", argv[2]);
        return 1;
    }

    //store the meta data from the first bmp file into the new one
    BMPHeader bmpHeader;
    fread(&bmpHeader, sizeof(BMPHeader), 1, image);
    fwrite(&bmpHeader, sizeof(BMPHeader), 1, outFile); 

    // same for the DIB
    DIBHeader dibHeader;
    fread(&dibHeader, sizeof(DIBHeader), 1, image);
    fwrite(&dibHeader, sizeof(DIBHeader), 1, outFile);

    // get secret message
    char *secret = argv[3];
    int secretLen = strlen(secret) + 1; // + 1 for null terminator

    // set it to the first pixel of data
    fseek(image, bmpHeader.startBits, SEEK_SET);

    uint8_t pixel[4]; //will hold RBG bytes + the alpha channel
    int byteIndex = 0; //tracks which byte we are on
    int bitCounter = 0; //track which bit we are on in the byte starting at the front

    // read a pixel from the image until you can't
    while(fread(pixel, 1, 4, image) == 4){
        for(int i = 0; i < 3; i++) { // for RGB
                if(byteIndex < secretLen) { //don't go past message length
                    // get the byte of the secret message
                    uint8_t secretByte = secret[byteIndex];

                    //get the LSB, and then the next LSB etc.
                    uint8_t bit = (secretByte >> bitCounter) & 1;

                    //change the LSB of the pixel with the on from the message
                    pixel[i] = setLSB(pixel[i], bit);

                    bitCounter++; //next LSB 

                    //After going through each bit in the first byte, go to next byte
                    if(bitCounter > 7){
                        bitCounter = 0;
                        byteIndex++;
                    }
                }
        }
        // write the pixel to the outputfile
        fwrite(pixel, 1, 4, outFile);
    }

    //close file
    fclose(image);
    fclose(outFile);

    printf("Pixel data starts at byte: %u\n", bmpHeader.startBits);
    printf("Image Width: %d pixels\n", dibHeader.Width);
    printf("Image Height: %d pixels\n", dibHeader.Height);
    return 0;
}
