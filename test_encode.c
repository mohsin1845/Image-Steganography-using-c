/*
Name: Mohsin Mohiuddin
Date: 03/05/2024
Admission Id: 23034_031
Batch: 23040(E)
Project Title: Image Steganography using LSB mechanism 
Project Description:  The art and science of hiding information by embedding messages within other,
		      seemingly harmless messages. Bits of unused data are replaced by bits of valuable
		      information using LSB mechanism. Sender and receiver will have individual key 
		      secret based on which they will be able to extract the actual data from the image.
Technologies used : Function pointers, File I/O operations, Bitwise operations
*/
#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include<unistd.h>

int main(int argc,char *argv[])
{ 
     if(argc<4)
	{
	   if( argc==1)
	    {
                printf("\033[1;31mERROR : \033[0m\t");
                printf("Please use below format\n");
		printf("./lsb_steg : Encoding:  ./lsb_steg -e <.bmp file> <.txt file> [output file]\n");
		printf("./lsb_steg : Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
		return 0;
	    }

           else if(argc==2)
            {
               if(!strcmp(argv[1],"-e"))
           	{
                    printf("\033[1;31mINFO : \033[0m\t");
		    printf("For encoding please pass minimum 4 arguments like--> ./a.out -e source_image secret_data_file.\n");
                    printf("*********************usage*******************\n");
                    printf(" ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
                    printf("*********************************************\n");
                   return 0;
                }
                 else if(!strcmp(argv[1],"-d"))
                 {
		    printf("\033[1;31mINFO : \033[0m\t");
		    printf("For decoding please pass minimum 3 arguments like--> ./a.out -d source_image.\n");
                    printf("*******************usage*******************\n");
                    printf("./a.out -d stego.bmp output.txt\n");
                    printf("*******************************************\n");
		    return 0;
	         }
             }
     else if(argc==3)
	 {
	     if(!strcmp(argv[1],"-e"))
             {
                 printf("\033[1;31mINFO : \033[0m\t");
                 printf("For encoding please pass minimum 4 arguements like--> .a.out -e source_image secret_data_file with extension\n");
                 printf("********************usage********************\n");
                 printf("./a.out -e beautiful.bmp secret.txt stego.bmp\n");
                 printf("*********************************************\n");
                  return 0;
              }
         }
      }
    if(check_operation_type(argv) == e_encode)
    {
	EncodeInfo encInfo;
        printf("\n");
        printf("\033[1;32mINFO : \033[0m\t");
	printf("You selected Encoding\n");
	if(read_and_validate_encode_args(argv, &encInfo) == e_success)
	{
            sleep(2);
            printf("\033[1;32mINFO : \033[0m\t");
	    printf("Read and Validate argument is successful\n");
	    sleep(2);
		if(do_encoding(&encInfo) == e_success)
		{         printf("\n");
                          sleep(2);
                          printf("\033[1;33m############### SUCCESSFULL ###############\033[0m\n");
    			  printf("\t \033[1;36m       ENCODE COMPLETED\033[0m \t \n");
                          printf("\033[1;33m###########################################\033[0m\n");
                          printf("\n");
		}
		else
		{
                        printf("\033[1;31mINFO : \033[0m\t");
	    		printf("Failed to do the encoding\n");
	        }
	}
	else
	{
            printf("\033[1;31mINFO : \033[0m\t");
	    printf("Failed to Read and Validate argument\n");
    	}
    }
    else if(check_operation_type(argv) == e_decode)
    {
	DecodeInfo decInfo;
        printf("\n");
        printf("\033[1;32mINFO :\033[0m\t");
	printf("You selected Decoding\n");
	if(read_and_validate_decode_args(argv, &decInfo) == d_success)
	{
            sleep(2);
            printf("\033[1;32mINFO : \033[0m\t");
	    printf("Read and validate argument is successful\n\n");
	    if(do_decoding(&decInfo) == d_success)
	    {
		 printf("\n");
                 printf("\033[1;33m############### SUCCESSFULL ###############\033[0m\n");
    	         printf("\t \033[1;36m     DECODE COMPLETED\033[0m \t \n");
                 printf("\033[1;33m###########################################\033[0m\n");
                 printf("\n");
		sleep(2);
	    }
	    else
	    {
                printf("\n");
		printf("\033[1;31m############### UNSUCCESSFULL ###############\033[0m\n");
		printf("\t \033[1;31m       DECODE FAILED\033[0m \t \n");
		printf("\033[1;31m###########################################\033[0m\n");
		printf("\n");

	    }
	}
	else
	{
            printf("\033[1;31mINFO : \033[0m\t");
	    printf("Failed to read and validate argument\n");
	}
    }
    else
    {
	printf("\t   Invalid Option\n***************usage***************\n");
	printf("./a.out -e beautiful.bmp secret.txt stego.bmp \n");
	printf("./a.out -d stego.bmp output.txt\n");
    }	

    return 0;
}
/*------------------------------------------ENCODING---------------------------------------*/


OperationType check_operation_type (char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
	return e_encode;
    else if(strcmp(argv[1], "-d")== 0)
	return e_decode;
    else 
	return e_unsupported;
}
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(argv[2] != NULL && strcmp(strstr(argv[2],"."),".bmp") == 0)
    {
	encInfo -> src_image_fname=argv[2];
    }
    else
    {
	return e_failure;
    }
    if(argv[3] != NULL && strstr(argv[3], ".") != NULL && strcmp(strstr(argv[3], "."), ".txt") == 0)
    {
	encInfo -> secret_fname = argv[3];
    }
    else
    {
	return e_failure;
    }
    if(argv[4] != NULL)
    {
	encInfo -> stego_image_fname = argv[4];
    }
    else
    {
	encInfo -> stego_image_fname = "stego.bmp";
        sleep(2);
        printf("\033[1;32mINFO :\033[0m\t");
        printf("Output File not mentioned. Creating \033[1;33m\"stego.bmp\"\033[0m image as default.\n");
    }
    return e_success;
}

/*-------------------------------------DECODING-------------------------------------------*/

Status(read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo))
{
     if(argv[2] != NULL && strstr(argv[2],".") != NULL && strcmp(strstr(argv[2],"."), ".bmp") == 0)
     {
	 decInfo -> stego_fname=argv[2];
     }
     else
     {
         return e_failure;
     }
     if(argv[3] != NULL && strcmp(strstr(argv[3],"."),".txt") == 0)
     {
	decInfo -> output_fname = argv[3];
     }
    else
    {
	decInfo -> output_fname = "output.txt";
    }
     
     return d_success;
}
