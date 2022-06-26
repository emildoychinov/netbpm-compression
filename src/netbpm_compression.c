#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pbm_pgm_ppm.c"

int main(int argc, char *argv[]){
	
	char filename[1000];
	strcpy(filename, argv[2]);
	filename[strlen(argv[2])] = '\0';

	switch(get_extension(filename)){
		
		case PPM :

			printf("PPM : compression and decompression not supported for this format \n");
			PPM_image *ppm_ptr = PPM_read(filename);
			return 0;

		case PGM :
			
			printf("PGM\n");
			
			if(!strcmp(argv[1], "-c")){
				strcat(filename, ".pgm");
				PBM_PGM_compress(filename);
				printf("compressed successfully!\n");
			}

			else if(!strcmp(argv[1], "-d")){
				strcat(filename, ".cpgm");
				PBM_PGM_decompress(filename);
				printf("decompressed successfully!\n");
			}

			else
				printf("invalid command");
				return -1;

			return 0;

		case PBM :

			printf("PBM\n");

                         if(!strcmp(argv[1], "-c")){
                                strcat(filename, ".pbm");
                                PBM_PGM_compress(filename);
				printf("compressed successfully!\n");
                        }

                        else if(!strcmp(argv[1], "-d")){
                                strcat(filename, ".cpbm");
                                PBM_PGM_decompress(filename);
				printf("decompressed successfully!\n");
                        }

                        else
                                printf("invalid command");
                                return -1;
			return 0;

		default :
			printf("error : files such as %s are not supported\n", filename);
			return -1;

	}

}
