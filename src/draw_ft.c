#include <stdio.h>

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


struct motion_ft_struct
{
    FT_Library ft_lib;
    FT_Face ft_face;
} mftstr;

/**
 * initialize_chars
 */

int initialize_chars(void)
{
    // TODO: Error checking, proper font config
    
    FT_Error err = FT_Init_FreeType(&mftstr.ft_lib);
    err = FT_New_Face(mftstr.ft_lib, "./Nouveau_IBM.ttf", 0, &mftstr.ft_face);

};


/**
 *  * draw_text
 *   */
int draw_text(unsigned char *image, int width, int height, int startx, int starty, const char *text, int factor)
{
    int num_nl = 0;
    int font_factor = 1;
    const char *end, *begin;
    int line_space, txtlen;
    FT_Error err;

    printf("draw_text Image Specs: %i wide x %i high, %i startx and %i starty\n", width, height, startx, starty);

    /* Pick an appropriate text size based on factor */

    font_factor = 8 + (8 * factor);
    err = FT_Set_Pixel_Sizes(mftstr.ft_face, 0, font_factor);

    /* Count the number of newlines in "text" so we scroll it up the image. */
    begin = end = text;
    txtlen = 0;
    while ((end = strstr(end, NEWLINE))) {
        if ((end - begin)>txtlen) txtlen = (end - begin);
        num_nl++;
        end += sizeof(NEWLINE)-1;
    }
    if (txtlen == 0) txtlen = strlen(text);

    /* Adjust the factor if it is out of bounds
 *      * txtlen at this point is the approx length of longest line
 *          */
    if ((txtlen * font_factor) > width){
        font_factor = (width / (txtlen * 7));
        if (font_factor <= 0) font_factor = 1;
        err = FT_Set_Pixel_Sizes(mftstr.ft_face, 0, font_factor);
    }

    if (((num_nl+1) * font_factor) > height){
        font_factor = (height / ((num_nl+1) * 8));
        if (font_factor <= 0) font_factor = 1;
        err = FT_Set_Pixel_Sizes(mftstr.ft_face, 0, font_factor);
    }

    line_space = font_factor * 9;

    starty -= line_space * num_nl;

    begin = end = text;

    while ((end = strstr(end, NEWLINE))) {
        int len = end-begin;

        draw_textn(image, startx, starty, width, begin, len, font_factor);
        end += sizeof(NEWLINE)-1;
        begin = end;
        starty += line_space;
    }

    draw_textn(image, startx, starty, width, begin, strlen(begin), factor);

    return 0;
};

/*
 * draw_phrase: When this function is done, we will have a text buffer that has correct
 * dimensions for the one line of text that it rendered, and it will contain the 
 * rendered text.
 *
 * The Phrase buffer is a pointer to a char pointer that has NOT been allocated.  This
 * function will take care of allocating memory.  After the parent function has 
 *
 */

int draw_phrase(unsigned char **phrase_buffer, int *phrase_height, int *phrase_len, const char *text)
{




};


/*
 * image: Pointer to the image array
 * startx: X Start position in terms of image size.  EG: 640x480 and startx is 630, it's 10px from the right
 *         hand side.
 * starty: Y Start position in terms of image size.  EG: 640x480 and startx is 430, it's 50 px from the bottom
 *         of the image.
 * width: Width of the image, for example 640
 * text: a pointer to the text
 * len: Number of characters wide
 * factor: Size factor
 */

int draw_textn(unsigned char *image, int startx,  int starty,  int width, const char *text, int len, int factor)
{



};

int no_main() {
  FT_Library ft;
  FT_Error err = FT_Init_FreeType(&ft);
  if (err != 0) {
    printf("Failed to initialize FreeType\n");
    exit(EXIT_FAILURE);
  }

  FT_Int major, minor, patch;
  FT_Library_Version(ft, &major, &minor, &patch);
  printf("FreeType's version is %d.%d.%d\n", major, minor, patch);

  FT_Face face;
  err = FT_New_Face(ft, "./Nouveau_IBM.ttf", 0, &face);
  if (err != 0) {
    printf("Failed to load face\n");
    exit(EXIT_FAILURE);
  }

  err = FT_Set_Pixel_Sizes(face, 0, 16);
  if (err != 0) {
    printf("Failed to set pixel size\n");
    exit(EXIT_FAILURE);
  }

  FT_UInt glyph_index = FT_Get_Char_Index(face, 'g');

  FT_Int32 load_flags = FT_LOAD_DEFAULT;
  err = FT_Load_Glyph(face, glyph_index, load_flags);
  if (err != 0) {
    printf("Failed to load glyph\n");
    exit(EXIT_FAILURE);
  }

  FT_Int32 render_flags = FT_RENDER_MODE_NORMAL;
  err = FT_Render_Glyph(face->glyph, render_flags);
  if (err != 0) {
    printf("Failed to render the glyph\n");
    exit(EXIT_FAILURE);
  }

  FT_Bitmap bitmap = face->glyph->bitmap;

  printf("Dimensions: %iw * %ih * %i pitch\n", bitmap.width, bitmap.rows, bitmap.pitch);

  for (size_t i = 0; i < bitmap.rows; i++) {
      for (size_t j = 0; j < bitmap.width; j++) {
          unsigned char pixel_brightness =
              bitmap.buffer[i * bitmap.pitch + j];

          if (pixel_brightness > 169) {
              printf("*");
          } else if (pixel_brightness > 84) {
              printf(".");
          } else {
              printf(" ");
          }
      }
      printf("\n");
  }

  return 0;
}
