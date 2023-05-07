#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
             // Compute the average value of the red, green, and blue
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            // Set the value of the red, green, and blue
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate over each pixel in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
             // reflect the pixel horizontally
            image[i][j].rgbtBlue = image[i][2 * width - j].rgbtBlue;
            image[i][j].rgbtGreen = image[i][2 * width - j].rgbtGreen;
            image[i][j].rgbtRed = image[i][2 * width - j].rgbtRed;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a temporary array to hold blurred image
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Initialize RGB values and indices for kernel (neighborhood) bounds
            float rgbtBlue = 0;
            float rgbtGreen = 0;
            float rgbtRed = 0;
            int k_s = -1;
            int k_e = 2;
            int l_s = -1;
            int l_e = 2;
            // Update bounds for special cases (corners and edges)
            //  top-left corner
            if (i == 0 && j == 0)
            {
                k_s = 0;
                l_s = 0;
            }
            //  top-right corner
            if (i == 0 && j == width - 1)
            {
                k_s = 0;
                l_e = 1;
            }
            //  bottom-left corner
            if (i == height - 1 && j == 0)
            {
                k_e = 1;
                l_s = 0;
            }
            //  bottom-right corner
            if (i == height - 1 && j == width - 1)
            {
                k_e = 1;
                l_e = 1;
            }
            //  top edge
            if (i == 0)
            {
                k_s = 0;
            }
            //  bottom edge
            if (i == height - 1)
            {
                k_e = 1;
            }
            //  left edge
            if (j == 0)
            {
                l_s = 0;
            }
            //  right edge
            if (j == width - 1)
            {
                l_e = 1;
            }
            // Iterate over neighborhood of current pixel
            for (int k = k_s; k < k_e; k++)
            {
                for (int l = l_s; l < l_e; l++)
                {
                    // Accumulate RGB values of neighboring pixels
                    rgbtBlue += image[i + k][j + l].rgbtBlue;
                    rgbtGreen += image[i + k][j + l].rgbtGreen;
                    rgbtRed += image[i + k][j + l].rgbtRed;
                }
            }
            // Average RGB values of neighborhood and store in temporary array
            temp[i][j].rgbtBlue = round(rgbtBlue / 9);
            temp[i][j].rgbtGreen = round(rgbtGreen / 9);
            temp[i][j].rgbtRed = round(rgbtRed / 9);
        }
    }
    // Update original image with blurred values from temporary array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int rgbtBlue_x = 0;
            int rgbtGreen_x = 0;
            int rgbtRed_x = 0;
            int rgbtBlue_y = 0;
            int rgbtGreen_y = 0;
            int rgbtRed_y = 0;

            // Loop over the neighboring pixels of the current pixel
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1)
                    {
                        rgbtBlue_x = 0;
                        rgbtGreen_x = 0;
                        rgbtRed_x = 0;

                        rgbtBlue_y = 0;
                        rgbtGreen_y = 0;
                        rgbtRed_y = 0;
                        break;
                    }
                    // Compute the gradient values using Gx
                    rgbtBlue_x += image[i + k][j + l].rgbtBlue * Gx[k + 1][l + 1];
                    rgbtGreen_x += image[i + k][j + l].rgbtGreen * Gx[k + 1][l + 1];
                    rgbtRed_x += image[i + k][j + l].rgbtRed * Gx[k + 1][l + 1];
                    // Compute the gradient values using Gy
                    rgbtBlue_y += image[i + k][j + l].rgbtBlue * Gy[k + 1][l + 1];
                    rgbtGreen_y += image[i + k][j + l].rgbtGreen * Gy[k + 1][l + 1];
                    rgbtRed_y += image[i + k][j + l].rgbtRed * Gy[k + 1][l + 1];
                }
            }
            // Compute the magnitude and set the pixel value
            int rgbtBlue = (int)fmin(255, round(sqrt(pow(rgbtBlue_x, 2) + pow(rgbtBlue_y, 2))));
            int rgbtGreen = (int)fmin(255, round(sqrt(pow(rgbtGreen_x, 2) + pow(rgbtGreen_y, 2))));
            int rgbtRed = (int)fmin(255, round(sqrt(pow(rgbtRed_x, 2) + pow(rgbtRed_y, 2))));
            temp[i][j].rgbtBlue = rgbtBlue;
            temp[i][j].rgbtGreen = rgbtGreen;
            temp[i][j].rgbtRed = rgbtRed;
        }
    }
    // Update original image with blurred values from temporary array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
        }
    }
    return;
}
