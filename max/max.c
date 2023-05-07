// Practice writing a function to find a max value

#include <cs50.h>
#include <stdio.h>

int max(int array[], int n);

int main(void)
{
    int n;
    do
    {
        n = get_int("Number of elements: ");
    } while (n < 1);
    // int n = 7;

    int arr[n];

    // arr[0] = 7;
    // arr[1] = 1;
    // arr[2] = 4;
    // arr[3] = 10;
    // arr[4] = 3;
    // arr[5] = 1;
    // arr[6] = 3;

    // max(arr, n);

    for (int i = 0; i < n; i++)
    {
        arr[i] = get_int("Element %i: ", i);
    }


    printf("The max value is %i.\n", max(arr, n));
}

// TODO: return the max value
int max(int array[], int n)
{
    //THE ALGORITHM BUBBLE!! IN FOR LOOP CHANGE n-1 TO p and unhash

    // int swap = -1;
    // while (swap != 0)
    // {
    //     int counter = 0;
    //     int p = n-1;
        for (int i = 0; i < n - 1; i++)
        {
            int current = array[i];
            int next = array[i + 1];
            if (array[i] > array[i + 1])
            {
                array[i+1] = current;
                array[i] = next;
                // counter++;
            }
        }
        // p++;
        // if (counter == 0)
        // {
        //     swap = 0;
        // }
    // }
    // for (int i = 0; i < n; i++)
    // {
    // printf("%i\n", array[i]);
    // }

    return array[n-1];
}
