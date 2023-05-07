#include "helpers.h"

typedef struct
{
    int rgbtRed;
    int rgbtGreen;
    int rgbtBlue;
} rgb_colors;

rgb_colors shape_colors = {0, 0, 0};
rgb_colors face_colors = {0, 0, 0};

typedef char *string;

void change_colors(rgb_colors *colors, int red, int green, int blue);

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    change_colors(&shape_colors, 120, 153, 0);
    change_colors(&face_colors, 255, 200, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j].rgbtBlue == 0 && image[i][j].rgbtGreen == 0 && image[i][j].rgbtRed == 0)
            {
                if (i == 0 || i == 1 || i == 6 || i == 7 || j == 0 || j == 7)
                {
                    image[i][j].rgbtBlue = shape_colors.rgbtBlue;
                    image[i][j].rgbtGreen = shape_colors.rgbtGreen;
                    image[i][j].rgbtRed = shape_colors.rgbtRed;
                }
            }
        }
    }
    for (int i = 2; i < 6; i++)
    {
        for (int j = 2; j < 6; j++)
        {
            if (image[i][j].rgbtBlue == 0 && image[i][j].rgbtGreen == 0 && image[i][j].rgbtRed == 0)
            {
                image[i][j].rgbtBlue = face_colors.rgbtBlue;
                image[i][j].rgbtGreen = face_colors.rgbtGreen;
                image[i][j].rgbtRed = face_colors.rgbtRed;
            }
        }
    }
}

void change_colors(rgb_colors *colors, int red, int green, int blue)
{
    colors->rgbtRed = red;
    colors->rgbtGreen = green;
    colors->rgbtBlue = blue;
}
