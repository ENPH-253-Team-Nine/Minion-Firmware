#ifndef SONAR_H
#define SONAR_H

#include <stateData.h>
#include <vector>
#include <math.h>
#include <algorithm>

namespace sonar
{
    class Processor
    {
    public:
        void process(); // only call when there is a full, accurate sweep in memory.
    private:
        class Treasure
        {
        public:
            Treasure(int startingIndex);
            void setEndingIndex(int endingIndex);
            bool isValid();
            int getDistance();
            int getHeading();

        private:
            int startingIndex;
            int endingIndex;
        };
        std::vector<Treasure *> treasures;
        bool backgroundValid(int distance);
        int backgroundDistance;
        bool generallyClose(int valueOne, int valueTwo, double factor);
    };
}

#endif