#include <cs50.h>
#include <stdio.h>
#include <string.h>

int getPopulationSize(string kindOfpopulation);
int calculateNumberOfYears(int startingSize, int endingSize);

int main(void)
{
    // TODO: Prompt for start size
    int startingPopulationSize = getPopulationSize("start");
    // TODO: Prompt for end size
    int endingPopulationSize = getPopulationSize("end");
    // TODO: Calculate number of years until we reach threshold
    int numberOfYears = calculateNumberOfYears(startingPopulationSize, endingPopulationSize);
    // TODO: Print number of years
    printf("Years: %i\n", numberOfYears);
}


















int getPopulationSize(string kindOfpopulation)
{
    int populationSize;
    if (strcmp(kindOfpopulation, "start") == 0)
    {
        do
        {
            populationSize = get_int("Start size: ");
        }
        while (populationSize < 9);
    }
    if (strcmp(kindOfpopulation, "end") == 0)
    {
        do
        {
            populationSize = get_int("End size: ");
        }
        while (populationSize < 9);
    }

    return populationSize;
}

int calculateNumberOfYears(int startingSize, int endingSize)
{
    int numberOfYears = 0;
    int sizePopulation = startingSize;
    while(sizePopulation <= endingSize)
    {
        int gain = sizePopulation / 3;
        int loss = sizePopulation / 4;
        sizePopulation = sizePopulation + gain - loss;
        numberOfYears++;
    }

    return numberOfYears;
}
