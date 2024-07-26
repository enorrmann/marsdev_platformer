#include "camera.h"

#include "dev.h"

void debug(s16 var_a, const char *message, int pos)
{
    // VDP_setTextPlane(WINDOW);
    VDP_setTextPlane(BG_B);

    char string_buffer[32];
    // char str_format [16];
    // strcat(str_format, message);
    // strcat(str_format, " %d ");

    // sprintf(string_buffer, str_format, var_a);
    sprintf(string_buffer, "%d ", var_a);
    strcat(string_buffer, message);

    VDP_drawText(string_buffer, 1, 4+pos);
}