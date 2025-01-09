Image Steganography
===================

A command-line interface based program for Image Steganography.<br>
It uses Least Significant Bit(LSB) algorithm for Image Steganography.

### Compiling the Program

1. <b>Compile the program : </b>
  ```
  $ cd ImageSteganography
  $ chmod +x *.c
  $ gcc *.c -o lsb_steg
  ```

### Running the Program

1. Place your normal .bmp image inside the Image Steganography Directory.
2. Place your .txt file with the secret data in same Directory.
3. How to Encode the message

  ```
  $ ./lsb_steg -e star.bmp secret.txt output_image.bmp
  ```
  A new image will be created in same directory with the secret message hidden in it.

4. How to Decode the message

  ```
  $ ./lsb_steg -d output.bmp output.txt
  ```
  A new file with name output_image.txt will be created containing the hidden message.
  ## Image steganography
Below is the Menu for Image Steganography using c:

![Image_stegnography](images/car-black-box-diagram.png)


