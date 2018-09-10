#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include "media_type.h"

void freeMemory();
void printAns(int ans);
char buffer[120];

mediaType_t * acceptRange = NULL;

int main(int argc, char * argv[])
{

    int i;
    char c;
    int flag = 1;

    mediaType_t * toAccept = NULL;

    // Revision of parameters

    if (argc != 2)
    {
        fprintf(stderr, "Too few arguments\n");
        return EXIT_FAILURE;
    }

    acceptRange = makeMediaRange(argv[1]);

    if (acceptRange == NULL)
    {
        return 0;
    }


    while (flag == 1)
    {
        i = 0;
        while ((c = (char) getchar()) != EOF && c != '\n')
        {
            buffer[i] = c;
            i++;
        }
        buffer[i] = '\0';

        if (strcmp(buffer,"exit") == 0 || strcmp(buffer,"-1") == 0)
        {
            flag = 0;
        }
        else
        {
            toAccept = makeMediaRange(buffer);
            printf("\n");
            printAns(isAcceptable(acceptRange , toAccept));
            freeMediaType(toAccept);
        }
    }

    freeMemory();

    return 0;
}

void printAns(int ans)
{
    switch (ans)
    {
        case printTrue:
            printf("true \n");
            break;
        case printNull:
            printf("null \n");
            break;
        case printFalse:
            printf("false \n");
            break;
        default:
            printf("null \n");
            break;
    }
}

void freeMemory()
{
    freeMediaType(acceptRange);
}