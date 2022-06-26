typedef struct image_pgm{

        int x,y;
        int color_depth;
        int *data;

}PBM_PGM_image;

typedef enum {PBM, PGM, PPM} FILE_TYPE;

typedef struct pixel_ppm{

        unsigned int r,g,b;

}PPM_pixel;

typedef struct image_ppm{

        int x,y;
        PPM_pixel *pixels;

}PPM_image;

PBM_PGM_image *PBM_read(char *filename);
PBM_PGM_image *PGM_read(char *filename);
void PBM_PGM_compress(char *filename);
void PBM_PGM_decompress(char *filename);
int get_extension(char *filename);
PPM_image *PPM_read(char *filename);
