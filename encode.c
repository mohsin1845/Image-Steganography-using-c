#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include<unistd.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    sleep(2);
    printf("\t\t------------\n");
    printf("\t\twidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    sleep(2);
    printf("\t\theight = %u\n", height);
     printf("\t\t------------\n");
    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    sleep(2);
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Opened the image\n");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    sleep(2);
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Opened the secret file\n");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    sleep(2);
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Opened the output image\n");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char header[54];
    fseek(fptr_src_image, 0, SEEK_SET);
    fread(header,sizeof(char), 54,fptr_src_image);
    fwrite(header,sizeof(char), 54, fptr_stego_image);
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);

}
Status check_capacity(EncodeInfo *encInfo)
{
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Checking for image capacity to handle secret data\n");
    encInfo -> image_capacity=get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file=get_file_size(encInfo -> fptr_secret);
    if(encInfo ->image_capacity > (54+16+32+32+32+ encInfo -> size_secret_file))
	  {

    		return e_success;
	  }
    else
    	  {
	      return e_failure;
	  }

}
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    unsigned char mask = 1<<7;
    for(int i=0;i<8;i++)
    {
	image_buffer[i]=(image_buffer[i] & 0xFE) | ((data & mask) >> (7-i));
	mask=mask >> 1;
    }
    return e_success;
}
Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo)
{
    //encode character by character
    for(int i=0;i<size;i++)
    {
	//read 8 bytes of rgb from source file
	fread(encInfo -> image_data, 8 ,sizeof(char),fptr_src_image);
	//encode byte to lsb
	encode_byte_to_lsb(data[i],encInfo -> image_data);
	fwrite(encInfo -> image_data, 8 ,sizeof(char), fptr_stego_image);
    }
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo-> fptr_src_image,encInfo -> fptr_stego_image,encInfo);
    return e_success;
}
Status encode_size_to_lsb(char *str,int size)
{
        unsigned int mask = 1<<31;
        for(int i=0;i<32;i++)
	        {
		    str[i]=(str[i] & 0xFE) | ((size & mask) >> (31-i));
		    mask=mask >> 1;
	         }
	    return e_success;
}

Status encode_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char str[32];
    fread(str, sizeof(char), 32, fptr_src_image);
    encode_size_to_lsb(str ,size);
    fwrite(str, sizeof(char), 32, fptr_stego_image);
    return e_success;
}
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    file_extn =".txt";
    encode_data_to_image(file_extn,strlen(file_extn),encInfo -> fptr_src_image,encInfo ->fptr_stego_image,encInfo);
    return e_success;
}
Status encode_secret_file_size(long size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, sizeof(char), 32,encInfo -> fptr_src_image);
    encode_size_to_lsb(str,size);
    fwrite(str, sizeof(char), 32,encInfo -> fptr_stego_image);
    return e_success;
} 
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char ch;
    fseek(encInfo -> fptr_secret, 0, SEEK_SET);
    for(int i=0; i < encInfo -> size_secret_file; i++)
    {
	fread(&ch, 1,sizeof(char), encInfo -> fptr_secret);
	fread(encInfo -> image_data, 8, sizeof(char), encInfo -> fptr_src_image);
	encode_byte_to_lsb(ch, encInfo -> image_data);
	fwrite(encInfo -> image_data,8 ,sizeof(char), encInfo -> fptr_stego_image);
    }
    return e_success;
}
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_stego)
{ 
    char ch;
    while(fread(&ch, 1, 1, fptr_src) > 0)
    {
	fwrite(&ch, 1, 1, fptr_stego);
    }
    return e_success;
}



Status do_encoding(EncodeInfo *encInfo)
{   
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Opening required files\n");
    if(open_files(encInfo) == e_success)
    { 
        sleep(2);
        printf("\033[1;32mINFO : \033[0m\t");
	printf("Read and validate completed\n");
        sleep(2);
        printf("\n");
        printf("\033[1;33m------------------------------------------------\033[0m\n");
    	printf("\t \033[1;33mENCODING PROCEDURE STARTED\033[0m \t \n");
        printf("\033[1;33m-------------------------------------------------\033[0m\n");
        sleep(2);
	if(check_capacity(encInfo) == e_success)
	{
            sleep(2);
            printf("\033[1;32mINFO : \033[0m\t");
	    printf("Done. Encoding is possible\n");
            sleep(2);
	}
	else
	{   printf("\033[1;32mINFO : \033[0m\t");
	    printf("Failed to encode secret file extension successfully\n");
            sleep(2);
	}
            printf("\033[1;32mINFO : \033[0m\t");
  	    printf("Copying Image header.\n");
            sleep(2);
	    if(copy_bmp_header(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
	    {
                printf("\033[1;32mINFO : \033[0m\t");
		printf("Done. copied the header successfully\n");
                sleep(2);
                printf("\033[1;32mINFO : \033[0m\t");
                printf("Encoding \"MAGIC STRING\"signature\n");
		if(encode_magic_string(MAGIC_STRING,encInfo) == e_success)
		{
                    printf("\033[1;32mINFO : \033[0m\t");
                    printf("Done.\n");
                    sleep(2);
		    if(encode_size(strlen(".txt"), encInfo -> fptr_src_image, encInfo ->fptr_stego_image) == e_success)
		    {
                        printf("\033[1;32mINFO : \033[0m\t");
			printf("Encoded secret file extension size successfully\n");
                        sleep(2);
                        printf("\033[1;32mINFO : \033[0m\t");
                        printf("Encoding secret file extension\n");
			if(encode_secret_file_extn(encInfo -> extn_secret_file, encInfo) == e_success)
			{
                            printf("\033[1;32mINFO : \033[0m\t");
			    printf("Done.\n");
                            sleep(2);
                            printf("\033[1;32mINFO : \033[0m\t");
                            printf("Encoding secret file data size\n");
			    if(encode_secret_file_size(encInfo ->size_secret_file, encInfo) == e_success)
			    {
                                printf("\033[1;32mINFO : \033[0m\t");
				printf("Done.\n");
				sleep(2);
                                printf("\033[1;32mINFO : \033[0m\t");
                                printf("Encoding secret File data\n");
				if(encode_secret_file_data(encInfo) == e_success)
				{
                                    printf("\033[1;32mINFO : \033[0m\t");
				    printf("Done.\n");
				    sleep(2);
                                    printf("\033[1;32mINFO : \033[0m\t");
                                    printf("Copying Left Over Data\n");
				    if(copy_remaining_img_data(encInfo -> fptr_src_image, encInfo -> fptr_stego_image) == e_success)
				    {
                                        printf("\033[1;32mINFO : \033[0m\t");
					printf("Done. Copied remaining bytes successfully\n");
                                        sleep(2);
				    }
				    else
				    {
					printf("Failed to copy remaining bytes\n");
					sleep(1.5);
					return e_failure;
				    }
				}
				else
				{
				    printf("Failed to encode secret data\n");
				    sleep(1.5);
				    return e_failure;
				}
			    }
			    else
			    {
				printf("Failed to encode secret file size\n");
				sleep(1.5);
				return e_failure;
			    }
			}
		    	else
		    	{
			printf("Failed to encode secret file extension size\n");
			sleep(1.5);
			return e_failure;
		    	}

		}
		else
		{
		    printf("Failed to Encode MAGIC STRING\n");
		    sleep(1.5);
		    return e_failure;
		}
	    }
	    else
	    {
		printf("Failed to copy the header\n");
		sleep(1.5);
		return e_failure;
	    }
	}
	else
	{
	    printf("image does not have suffecient size for encoding\n");
	    sleep(1.5);
	    return e_failure;
	} 
    }
    else
    {
	printf("Failed to open the files\n");
	return e_failure;
    }
    fclose(encInfo -> fptr_src_image);
    fclose(encInfo -> fptr_stego_image);
    fclose(encInfo -> fptr_secret);
    return e_success;
}

