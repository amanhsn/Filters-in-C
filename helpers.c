#include "helpers.h"
#include <math.h> // to use round function
#include <stdbool.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //take average of rgb
            RGBTRIPLE pixel = image[i][j];
            int average = round((pixel.rgbtRed + pixel.rgbtGreen + pixel.rgbtBlue) / 3.0);
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
    return;
}

//to cap value at 255
int cap(int value)
{
    return (value > 255 ? 255 : value);
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //formula given
            RGBTRIPLE pixel = image[i][j];
            int originalRed = pixel.rgbtRed;
            int originalGreen = pixel.rgbtGreen;
            int originalBlue = pixel.rgbtBlue;
            image[i][j].rgbtRed = cap(round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue));
            image[i][j].rgbtGreen = cap(round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue));
            image[i][j].rgbtBlue = cap(round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue));

        }
    }
    return;
}

//to swap the values
void swap(RGBTRIPLE *p1, RGBTRIPLE *p2)
{
    RGBTRIPLE temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        //loop till mid value
        for (int j = 0; j < width / 2; j++)
        {
            //we swap pixels
            swap(&image[i][j], & image[i][width - 1 - j]);
        }
    }
    return;
}

//check pixel constraints
bool is_valid_pixel(int i, int j, int height, int width)
{
    return i >= 0 && i < height && j >= 0 && j < width;
}

//taking 9 pixels
RGBTRIPLE get_blurred_pixel(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int redValue = 0, blueValue = 0, greenValue = 0;
    int numOfValidPixels = 0;
    //di is change in i
    for (int di = -1; di <= 1; di++)
    {
        //dj is change in j
        for (int dj = -1; dj <= 1; dj++)
        {
            int new_i = i + di;
            int new_j = j + dj;
            if (is_valid_pixel(new_i, new_j, height, width))
            {
                numOfValidPixels++;
                redValue += image[new_i][new_j].rgbtRed;
                blueValue += image[new_i][new_j].rgbtBlue;
                greenValue += image[new_i][new_j].rgbtGreen;
            }
        }
    }
    //generating blurred pixel
    RGBTRIPLE blurred_pixel;
    blurred_pixel.rgbtRed = round((float) redValue / numOfValidPixels);
    blurred_pixel.rgbtGreen = round((float) greenValue / numOfValidPixels);
    blurred_pixel.rgbtBlue = round((float) blueValue / numOfValidPixels);

    return blurred_pixel;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //blur in another image and then copy to original
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //blurring pixels of copy
            new_image[i][j] = get_blurred_pixel(i, j, height, width, image);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // pasting copy pixel to original pixel
            image[i][j] = new_image[i][j];
        }
    }
    return;
}
// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Declaring gx and gy kernels
    int gxArray[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyArray[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE edgeImage[height][width];

    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            float gxRed = 0, gxGreen = 0, gxBlue = 0;
            float gyRed = 0, gyGreen = 0, gyBlue = 0;

            // Look at each cell in 3x3 matrix, around current cell
            for (int row = i - 1 ; row <= i + 1 ; row++)
            {
                for (int col = j - 1 ; col <= j + 1 ; col++)
                {
                    // If cell is out of bounds, skip
                    if (row == -1 || row == height || col == -1 || col == width)
                    {
                        continue;
                    }

                    // Add weighted gx/gy value of each color
                    gxRed += image[row][col].rgbtRed * gxArray[row - (i - 1)][col - (j - 1)];
                    gxGreen += image[row][col].rgbtGreen * gxArray[row - (i - 1)][col - (j - 1)];
                    gxBlue += image[row][col].rgbtBlue * gxArray[row - (i - 1)][col - (j - 1)];

                    gyRed += image[row][col].rgbtRed * gyArray[row - (i - 1)][col - (j - 1)];
                    gyGreen += image[row][col].rgbtGreen * gyArray[row - (i - 1)][col - (j - 1)];
                    gyBlue += image[row][col].rgbtBlue * gyArray[row - (i - 1)][col - (j - 1)];

                }
            }

            // Add up gx and gy value
            int red = round(sqrt(gxRed * gxRed + gyRed * gyRed));
            int green = round(sqrt(gxGreen * gxGreen + gyGreen * gyGreen));
            int blue = round(sqrt(gxBlue * gxBlue + gyBlue * gyBlue));

            // Cap values at 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

            edgeImage[i][j].rgbtRed = red;
            edgeImage[i][j].rgbtGreen = green;
            edgeImage[i][j].rgbtBlue = blue;
        }
    }

    // Copy edge image array to normal image array
    for (int i = 0 ; i < height ; i++)
    {
        for (int j = 0 ; j < width ; j++)
        {
            image[i][j] = edgeImage[i][j];
        }
    }
    return;
}
//cap array 255x255
//project shown on 13-1-2023 <jaudat liked >
