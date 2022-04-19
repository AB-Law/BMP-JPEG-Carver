#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

//Creates a BMP file from the image given as a parameter
void BMP(char argv[50]){
    unsigned char jpeg_header[2] = {
      0x42,
      0x4D
    }, f_data[500000];
    char file_name[100];
    int i = 1;

    FILE * f = fopen(argv, "rb");
    FILE * ftemp;

    if (f == NULL) {
      printf("File %s can't open\n", argv);
      exit(0);
    } else 
    {
        printf("File %s has been opened\n", argv);
    }
    //while reading the file check if the header is correct.
    while (!feof(f)) {
        //create last_pos, store the current position of the file pointer.
        long last_pos = ftell(f);
        fread(f_data, 1, 16, f);
        long last_pos2 = ftell(f);
        if (f_data[0] == jpeg_header[0] && f_data[1] == jpeg_header[1]) {
            //read the size of the bmp file by reading the next 4 values from f_data array.
            unsigned char temp_storage[4];
            temp_storage[0] = f_data[2];
            temp_storage[1] = f_data[3];
            temp_storage[2] = f_data[4];
            temp_storage[3] = f_data[5];
            //convert the 4 bytes into integer in little endian format.
            int size = temp_storage[0] + temp_storage[1] * 256 + temp_storage[2] * 256 * 256 + temp_storage[3] * 256 * 256 * 256;
            if(size<0){
                continue;
            }
            sprintf(file_name, "%s_%d.bmp", getlogin(), i-1);
            printf("file %s%d is a bmp file, ", file_name, i);
            printf("Size: %d\n",size);
            //create a new file and write the data into it.
            //go back to the location of last_pos
            fseek(f, last_pos, SEEK_SET);
            //Read the next size bytes
            fread(f_data, 1, size, f);
            ftemp = fopen(file_name, "wb");
            fwrite(f_data, 1, size, ftemp);
            fclose(ftemp);
            i++;
            //go back to the location of last_pos
            fseek(f, last_pos2, SEEK_SET);
        }
         else {
        }
    }
  printf("Closed the file (BMP module) \n");
  fclose(f);
}


//Create comments for the below function
void JPEG(char argv[50])
{
  unsigned char jpeg_header[6] = {
    0xFF,
    0xD8,
    0xFF,
    0xE0,
    0x00,
    0x10
  }, data, temp_byte = 0x00;
  char file_name[100];

  int i = 1, j = 0, k = 0;

  FILE * f = fopen(argv, "rb");
  FILE * ftemp;


  if (f == NULL) {
    printf("file %s cannot be opened\n", argv);
    exit(0);
  } else {
    printf("file %s opened\n", argv);
  }

//Goes through the first 6 bytes to check if they match the jpeg header.
  while (!feof(f)) {
    fread( & data, 1, 1, f);
    j = 0;
    if (data == jpeg_header[j]) { 
      j++;
      fread( & data, 1, 1, f);
      if (data == jpeg_header[j]) { 
        j++;
        fread( & data, 1, 1, f);
        if (data == jpeg_header[j]) { 
          j++;
          fread( & data, 1, 1, f);
          if (data == jpeg_header[j]) { 
            j++;
            fread( & data, 1, 1, f);
            if (data == jpeg_header[j]) { 
              j++;
              fread( & data, 1, 1, f);
              if (data == jpeg_header[j]) { 
                j++;
              }
            }
          }
        }
      }
    }
    k = 0;

//if they do, create a new file and write the data into it.
    if (j == 6) {
      sprintf(file_name, "%s_%d.jpg", getlogin(), i-1);
      ftemp = fopen(file_name, "wb");

      if (ftemp == NULL) {
        printf("File %s corrupted\n", file_name);
        exit(0);
      } else {
        printf("File %s created\n", file_name);
      }

      while (k != j) {
        fwrite( & jpeg_header[k], 1, 1, ftemp);
        k++;
      }
      temp_byte = data;

      while (data != 0xd9 || temp_byte != 0xFF) {
        temp_byte = data;
        fread( & data, 1, 1, f);
        fwrite( & data, 1, 1, ftemp);
        if (temp_byte == 0xFF && data == 0xD9) {
          fclose(ftemp);
          i++;
          break;
        }
      }
    }
  }

  printf("closed \n");
  fclose(f);
}

int main(int argc, char ** argv) {
  if (argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(0);
  };
  //printf ("Extracted the files with log in name %s on host %d\n", getlogin(), gethostname);
  BMP((char *)argv[1]);
  JPEG((char *)argv[1]);
  
}