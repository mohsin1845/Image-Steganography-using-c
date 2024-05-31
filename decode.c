#include <stdio.h>
#include<string.h>
#include "decode.h"
#include "types.h"
#include "common.h"
#include<unistd.h>
#include<stdlib.h>

Status open_files_decoding(DecodeInfo *decInfo)
{
    //    stego File
   decInfo -> fptr_stego =fopen(decInfo -> stego_fname,"r");
   sleep(2);
   printf("\033[1;32mINFO : \033[0m\t");
   printf("Opened the Encoded Image\n");
   //    Error handling
   if(decInfo -> fptr_stego == NULL)
   {
       perror("fopen");
       fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> stego_fname);
       return d_failure;
   }
   //  secret file
   decInfo -> fptr_output =fopen(decInfo -> output_fname, "w");
   // Error Handling
   if(decInfo -> fptr_output == NULL)
   {
       perror("fopen");
       fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> output_fname);
       return d_failure;
   }
   return d_success;
}
Status decode_byte_from_lsb(char magic, char *data)
{
    unsigned char mask= 1, dec_data=0;
    for(int i=0;i<8;i++)
    {
    	dec_data=dec_data | ((data[i] & mask) <<(7-i));
    }
    //printf("%c\n",dec_data); // printing magic string
    return dec_data;

}
Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo)
{
    printf("\033[1;32mINFO : \033[0m\t");
    printf("checking magic string is matching\n");
    int size=strlen(magic_string);
    char magic_buffer[3]={0};
    fseek(decInfo -> fptr_stego, 54,SEEK_SET);
    char data[8];
    for(int i=0;i<size; i++)
    {
	fread(data,8, sizeof(char),decInfo -> fptr_stego);
	magic_buffer[i]=decode_byte_from_lsb(magic_string[i],data);
	//fwrite(data, 8, sizeof(char),decInfo -> fptr_output);
    }
    magic_buffer[size] = '\0';
    //printf("%s\n",magic_buffer);
    if (strcmp(magic_buffer, magic_string) != 0)
    {
      return d_failure;
    }
    else
    {
    return d_success;
    }
}
Status decode_size_from_lsb(char *str, int size)
{
    
     unsigned char mask= 1, dec_data=0;
     for(int i=0;i<31;i++)
     {
	 dec_data=dec_data | ((str[i] & mask) <<(31-i));
     }
     //printf("%d\n",dec_data);
	     
      return dec_data;
}

Status decode_secret_extension_file_size(int size, FILE *fptr_stego,DecodeInfo *decInfo)
{
     
    char str[32];
    fread(str, sizeof(char), 32, fptr_stego);
    decode_size_from_lsb(str ,size);          
    //fwrite(str, sizeof(char), 32,decInfo -> file_size);
    return d_success;
}

Status decode_data_from_lsb_stego(char *data,char *str, int k)
{      
        unsigned mask=1;
        
   // decode character by character
        for(int i=0;i<8;i++)
	{
	//decode byte to lsb
	data[k]=(data[k] | ((str[i] & mask) << (7-i)));
	}
 	return d_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
   	file_extn=".txt";
    	int i; char str[8];
    	int size= strlen(file_extn);
    	for(i=0;i<size;i++)
     	{
             decInfo -> extn_fname[i] =0;
             fread(str,8,sizeof(char),decInfo -> fptr_stego);
             decode_data_from_lsb_stego(decInfo -> extn_fname, str,i);
        }
        decInfo->extn_fname[i]='\0';
        printf("\033[1;32mINFO : \033[0m\t");
        printf("The extension of secret file is %s\n",decInfo -> extn_fname);
        //printf("%ld\n",ftell(decInfo -> fptr_stego)); // 
            
        return d_success;
}
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    char sec_file_size[32];
    int size=0,length;
    //printf("%ld\n",ftell(decInfo -> fptr_stego));
    fread(sec_file_size, sizeof(char), 32,decInfo -> fptr_stego);
    length=decode_size_from_lsb(sec_file_size,size);
    decInfo ->length_secret_data=length;
   // printf("The size obtained %d\n",decInfo ->length_secret_data);
    return d_success;
}


Status decode_byte_from_lsb_to_output(char *data,FILE *fptr_output)
{
    unsigned char mask= 1, dec_data=0;
    for(int i=0;i<8;i++)
    {
    	dec_data=dec_data | ((data[i] & mask) <<(7-i));
    }
    //printf("%c\n",dec_data); 
    fwrite(&dec_data,1,sizeof(char),fptr_output);
    return d_success;

}
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char data[8];
    fseek(decInfo -> fptr_output, 0, SEEK_SET);
    
   
	for(int i=0;i<decInfo -> length_secret_data ; i++)
    	{
	    fread(data,8, sizeof(char),decInfo -> fptr_stego);
 	    decode_byte_from_lsb_to_output(data,decInfo -> fptr_output);
    	}
       
    
    return d_success;
}
 

   
Status do_decoding(DecodeInfo *decInfo)
{
    sleep(2);
    printf("\033[1;33m------------------------------------------------\033[0m\n");
    printf("\t \033[1;33mDECODING PROCEDURE STARTED\033[0m \t \n");
    printf("\033[1;33m-------------------------------------------------\033[0m\n\n");
    printf("\033[1;32mINFO : \033[0m\t");
    printf("Opening required files\n");
    sleep(2);

    if(open_files_decoding(decInfo) == d_success)
    {
        printf("\033[1;32mINFO : \033[0m\t");
	printf("Successfully opened all the required files\n");
        sleep(2);
	if(decode_magic_string(MAGIC_STRING, decInfo) == d_success)
	{
            printf("\033[1;32mINFO : \033[0m\t");
	    printf("\033[0;31m\"MAGIC STRING MATCHED SUCCESSFULLY\"\n\033[0m");
	    sleep(2);
	    if(decode_secret_extension_file_size(strlen(".txt"),decInfo -> fptr_stego, decInfo) == d_success)
	    {
                printf("\033[1;32mINFO : \033[0m\t");
		printf("Decoded secret file extension size successfully\n");
		sleep(2);
		if(decode_secret_file_extn(decInfo -> file_extn, decInfo) == d_success)
		{
                    printf("\033[1;32mINFO : \033[0m\t");
		    printf("Decoded secret file extension successfully\n");
		    sleep(2);
		    if(decode_secret_file_size(decInfo) == d_success)
		    {
                        printf("\033[1;32mINFO : \033[0m\t");
			printf("Decoded secret file size successfully\n");
			sleep(2);
			if(decode_secret_file_data(decInfo) == d_success)
			{
                            printf("\033[1;32mINFO : \033[0m\t");
			    printf("Successfully decoded secret data\n");
			    sleep(2);
			}
			else
			{
                            printf("\033[1;31mINFO : \033[0m\t");
			    printf("Failed to decode secret data\n");
			    sleep(1.5);
			    return d_failure;
			}
		    }
		    else
		    {
                        printf("\033[1;31mINFO : \033[0m\t");
			printf("Failed to decode secret file size\n");
			sleep(1.5);
			return d_failure;
		    }
		}
		else
		{
                    printf("\033[1;31mINFO : \033[0m\t");
		    printf("Failed to Decode secret file extension\n");
		    sleep(1.5);
		    return d_failure;
		}

	    }
	    else
	    {
		printf("Failed to decode secret file extension size\n");
		sleep(1.5);
		return d_failure;

	    }

	}
	else
	{
            printf("\033[1;31mINFO : \033[0m\t");
	    printf("Failed to Decode MAGIC STRING\n");
	    sleep(1.5);
	    return d_failure;
	}
    }
    else
    {
        printf("\033[1;31mINFO : \033[0m\t");
	printf("Failed to open the files\n");
	return d_failure;
    }
    fclose(decInfo -> fptr_output);
    fclose(decInfo -> fptr_stego);
    return d_success;
}





