#include <cs50.h>
#include <stdio.h>

int get_height(void);
void draw_pyramids(int height);
void left_pyramid(int height, int number_of_blocks);
void right_pyramid(int number_of_blocks);
void draw_gap(int gap_width);

int main(void)
{
    int height = get_height();
    draw_pyramids(height);
}

int get_height(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 2 || height > 10);

    return height;
}

void draw_pyramids(int height)
{
    for (int i = 1; i <= height; i++)
    {
        left_pyramid(height, i);
        draw_gap(2);
        right_pyramid(i);
        printf("\n");
    }
}

void left_pyramid(int height, int number_of_blocks)
{
    for (int j = height; j > 0; j--)
    {
        if (j <= number_of_blocks)
        {
            printf("#");
        }
        else
        {
            printf("%s", " ");
        }
    }
}

void right_pyramid(int number_of_blocks)
{
    for (int j = 0; j < number_of_blocks; j++)
    {
        printf("#");
    }
}

void draw_gap(int gap_width)
{
    printf("%*s", gap_width ," ");
}