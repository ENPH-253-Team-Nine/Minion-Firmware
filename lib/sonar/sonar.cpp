#include "sonar.h"

using namespace sonar;

Processor::Treasure::Treasure(int startingIndex)
{
    this->startingIndex = startingIndex;
    this->endingIndex = endingIndex;
}

void Processor::Treasure::setEndingIndex(int endingIndex)
{
    this->endingIndex = endingIndex;
}

bool Processor::Treasure::isValid()
{
    int angularWidth = 5 * (endingIndex - startingIndex);
    int width = getDistance() * angularWidth; // approximating with arc length. Should be reasonably accurate for low angles

    uint8_t minWidth = StateData::sonarProcessing::idolWidth - StateData::sonarProcessing::idolWidthError;
    uint8_t maxWidth = StateData::sonarProcessing::idolWidth + StateData::sonarProcessing::idolWidthError;
    return (minWidth < width) && (maxWidth > width);
}

int Processor::Treasure::getDistance()
{
    int length = endingIndex - startingIndex + 1;

    int sum = 0;

    for (int i = startingIndex; i <= endingIndex; i++)
    {
        sum += StateData::sonarSweep[i];
    }

    return sum / length;
}

int Processor::Treasure::getHeading()
{
    return 5 * ((endingIndex - startingIndex) / 2 + startingIndex);
}

bool Processor::backgroundValid(int distance)
{
    int matchingCount;
    for (int i = 0; i < 36; i++)
    {
        if (StateData::sonarSweep[i] >= distance * (double)StateData::sonarProcessing::backgroundMatchFactorPercent / 100)
        {
            matchingCount++;
        }
    }
    return matchingCount >= (double)StateData::sonarProcessing::backgroundMatchAmountPercent / 100;
}

void Processor::process()
{
    switch (StateData::sonarProcessing::algorithm)
    {
    case 0:
    {
        bool backgroundValid = false;
        while (!backgroundValid)
        {
            int *backgroundDistanceIter = std::max_element(StateData::sonarSweep, &StateData::sonarSweep[36]);
            backgroundDistance = *backgroundDistanceIter;
            backgroundValid = Processor::backgroundValid(backgroundDistance);
            if (!backgroundValid)
            {
                StateData::sonarSweep[std::distance(StateData::sonarSweep, backgroundDistanceIter)] = 0;
            }
        }
        bool inTreasure = false;
        for (int i = 0; i < 36; i++)
        {
            if (StateData::sonarSweep[i] != 0 && StateData::sonarSweep[i] < backgroundDistance * (double)StateData::sonarProcessing::backgroundMatchFactorPercent / 100)
            {
                if (inTreasure)
                {
                    treasures.back()->setEndingIndex(i);
                }
                else
                {
                    treasures.push_back(new Treasure(i));
                    inTreasure = true;
                }
            }
            else
            {
                inTreasure = false;
            }
        }
        Treasure *bestTreasure;
        for (Treasure *treasure : treasures)
        {
            if (treasure->isValid() && treasure->getDistance() < bestTreasure->getDistance())
            {
                bestTreasure = treasure;
            }
        }

        StateData::sonarHeadingOutput = bestTreasure->getHeading();
        break;
    }
    case 1:
    {
        int leastDistanceIndex = 0;
        for (int i = 1; i < 36; i++)
        {
            if (StateData::sonarSweep[i] > 0 && StateData::sonarSweep[i] < StateData::sonarSweep[leastDistanceIndex])
            {
                leastDistanceIndex = i;
            }
        }
        StateData::sonarHeadingOutput = 5 * leastDistanceIndex;
        break;
    }
    case 2:
    {
        double leastDistanceIndex = 0;
        for (int i = 1; i < 36; i++)
        {
            if (StateData::sonarSweep[i] > 0 && StateData::sonarSweep[i] < StateData::sonarSweep[(int)leastDistanceIndex] && !generallyClose(StateData::sonarSweep[(int)i], StateData::sonarSweep[(int)i-1],0.2))
            {
                leastDistanceIndex = i;
            }else if(StateData::sonarSweep[i] > 0 && generallyClose(StateData::sonarSweep[i],StateData::sonarSweep[(int)leastDistanceIndex],0.2) && leastDistanceIndex==i-1){
                leastDistanceIndex+=0.5;
            }
        } 
        StateData::sonarHeadingOutput = 5 * leastDistanceIndex;
        break;
    }
    }
}

bool Processor::generallyClose(int valueOne, int valueTwo, double factor){
    double maxValue = valueTwo+valueTwo*factor;
    double minValue = valueTwo-valueTwo*factor;

    return valueOne<maxValue && valueTwo>minValue;
}