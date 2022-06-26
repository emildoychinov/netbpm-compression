#include "pbm_pgm_ppm.h"

int get_extension(char *filename){
	
        strtok(filename, ".");
	
	printf("name of file : %s\n", filename);

        char *token = strtok(NULL, ".");
	printf("token : %s\n", token);

        if(!strcmp(token, "pbm") || !strcmp(token, "cpbm"))
                return PBM;

        if(!strcmp(token, "pgm") || !strcmp(token, "cpgm"))
                return PGM;

        if(!strcmp(token, "ppm"))
                return PPM;

	else
		return -1;

}

PBM_PGM_image *PBM_read(char *filename){

        PBM_PGM_image *img_ptr = malloc(sizeof(PBM_PGM_image));
        img_ptr->color_depth = -1;

        FILE *img = fopen(filename, "r");
        char buff[256];

        if(!img){

                printf("no file named %s\n", filename);
                return 0;

        }

        fgets(buff, sizeof(buff), img);

        if(buff[0] != 'P' || buff[1] != '1'){

                printf("invalid file format : expected .pbm");
                return 0;

        }

	//fscanf(img, "%s", buff);
	
	//while(buff[0] = '#'){

		//fgets(buff, 255, img);
          	//fscanf(img, "%s", buff);

	//}

        fscanf(img, "%d %d", &img_ptr->x, &img_ptr->y);
        img_ptr->data = malloc(sizeof(int) * img_ptr->x * img_ptr->y);

        int bound = 0;

        for(int i=0; i<img_ptr->x * img_ptr->y; i++)
                fscanf(img, "%d", &img_ptr->data[i]);

        /*for(int i=0; i<img_ptr->x * img_ptr->y; i++){

                bound++;
                printf("%d ", img_ptr->data[i]);
                if(bound == img_ptr->x){
                        printf("\n");
                        bound = 0;
                }

        }*/

        fclose(img);

        return img_ptr;
}

PBM_PGM_image *PGM_read(char *filename){

        PBM_PGM_image *img_ptr = malloc(sizeof(PBM_PGM_image));
        FILE *img = fopen(filename, "r");
        char buff[256];

        if(!img){

                printf("no file named %s\n", filename);
                return 0;

        }

        fgets(buff, sizeof(buff), img);

        if(buff[0] != 'P' || buff[1] != '2'){

                printf("invalid file format : expected .pgm");
                return 0;

        }

	//fscanf(img, "%s", buff);

        //while(buff[0] = '#'){

                //fgets(buff, 255, img);
                //fscanf(img, "%s", buff);

        //}

        fscanf(img, "%d %d", &img_ptr->x, &img_ptr->y);
        fscanf(img, "%d", &img_ptr->color_depth);

        img_ptr->data = malloc(sizeof(int) * img_ptr->x * img_ptr->y);

        for(int i=0; i<img_ptr->x * img_ptr->y; i++)
                fscanf(img, "%d", &img_ptr->data[i]);

        /*for(int i=0; i<img_ptr->x * img_ptr->y; i++){

                bound++;
                printf("%d ", img_ptr->data[i]);
                if(bound == img_ptr->x){
                        printf("\n");
                        bound = 0;
                }

        }*/

        fclose(img);

        return img_ptr;

}

void PBM_PGM_compress(char *filename){

        PBM_PGM_image *img;

        switch(get_extension(filename)){

                case PBM:
                        strcat(filename, ".pbm");
                        img = PBM_read(filename);
                        break;

                case PGM :
                        strcat(filename, ".pgm");
                        img = PGM_read(filename);
                        break;
                default :
                        return;
        }

        int x,y,type;

        x = img->x;
        y = img->y;

        type = img->color_depth == -1 ? 1 : 2;

        int start_ix = 0;

        unsigned char compressed_data[2*x*y];

        int bound = 0;

        for(int i=0; i<x*y; i++){

                //printf("%d ", img->data[i]);
                int count = 1;
                bound++;

                while(bound != x && i < x*y - 1 && img->data[i] == img->data[i+1]){

                        i++;
                        count++;
                        if(count == 254)
                                break;
                        bound++;

                }

                if(bound >= x)
                        bound = 0;
                //printf("count : %d, data : %d\n", count, img->data[i]);
                compressed_data[start_ix++] = count;
                compressed_data[start_ix++] = img->data[i];
                //printf("count : %d\n", compressed_data[start_ix-2]);
                //printf("data : %d\n", compressed_data[start_ix-1]);
        }

        compressed_data[start_ix] = '\0';

        strtok(filename, ".");
        strcat(filename, (img->color_depth == -1) ? ".cpbm" : ".cpgm");

        FILE *compressed_image = fopen(filename, "wb");

        fwrite(&type, sizeof(int), 1, compressed_image);
        fwrite(&x, sizeof(int), 1, compressed_image);
        fwrite(&y, sizeof(int), 1, compressed_image);
        if(type == 2)
                fwrite(&img->color_depth, sizeof(int), 1, compressed_image);
        fwrite(compressed_data, start_ix, 1, compressed_image);

        fclose(compressed_image);

}

void PBM_PGM_decompress(char *filename){

        FILE *f = fopen(filename, "rb");

        if(!f){
                printf("error : file %s does not exist\n");
                return;
        }

        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        printf("size : %d\n\n\n", size);

        rewind(f);
        fclose(f);

        f = fopen(filename, "rb");

        int x,y,color_depth,type;

        fread(&type, sizeof(int), 1, f);
        fread(&x, sizeof(int), 1, f);
        fread(&y, sizeof(int), 1, f);

        if(type == 2){
                fread(&color_depth, sizeof(int), 1, f);
                size-=sizeof(int);
        }

        size-=3*sizeof(int);

        unsigned char *compressed_data = malloc(sizeof(unsigned char)*size);
        fread(compressed_data, size, 1, f);

        fclose(f);

        /*for(int i = 1; i<size; i++){
                printf("%d ", compressed_data[i]);
        }*/

        fclose(f);

        //int x,y,color_depth;

        int start_index = 0;
        //int type = (int)compressed_data[start_index++];

        //x = (int)compressed_data[start_index++];
        //y = (int)compressed_data[start_index++];

        strtok(filename, ".");
        char *fmt;

        if(type == 2){

                //color_depth = (int)compressed_data[start_index++];
                strcat(filename, "_decompressed.pgm");
                fmt = "%c%d\n%d %d\n%d\n";
        }
        else{
                strcat(filename, "_decompressed.pbm");
                fmt = "%c%d\n%d %d\n";
        }


        f = fopen(filename, "w");

        //lamersko e ama ne mi se misli po failove pisha algorithmi

        if(type == 2)
                fprintf(f, fmt, 'P', type, x, y, color_depth);

        else
                fprintf(f, fmt, 'P', type, x, y);

        int bound = 0;

        for(int i=start_index; i<size; i+=2){

                int count = (int)compressed_data[i];

                int num = (int)compressed_data[i+1];

                //printf("%d %d ", count, num);

                bound+=count;

                for(int j = 0; j<count; j++)
                        fprintf(f, "%d ", num);

                //printf("bound : %d\n", bound);
                if(bound == x){
                        fprintf(f, "\n");
                        bound = 0;
                }

        }

        fclose(f);

}

PPM_image *PPM_read(char *filename){

        PPM_image *img_ptr;
        FILE *img = fopen(filename, "rb");
        char buff[256];
        int rgb_component;

        if(!img){

                printf("no file named %s\n", filename);
                return 0;

        }

        fgets(buff, sizeof(buff), img);

        if(buff[0] != 'P' || buff[1] != '3'){

                printf("invalid file format : expected .ppm");
                return 0;

        }

	//fscanf(img, "%s", buff);

        //while(buff[0] = '#'){

             //   fgets(buff, 255, img);
               // fscanf(img, "%s", buff);

        //}

        img_ptr = malloc(sizeof(PPM_image));
        fscanf(img, "%d %d", &img_ptr->x, &img_ptr->y);
        fscanf(img, "%d", &rgb_component);

        img_ptr->pixels = malloc(img_ptr->x * img_ptr->y * sizeof(PPM_pixel));
        while(fgetc(img) != '\n');
        fread(img_ptr->pixels, 3*img_ptr->x, img_ptr->y, img);

        fclose(img);

        return img_ptr;



}
