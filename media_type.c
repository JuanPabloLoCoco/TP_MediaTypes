//
// Created by lococo on 06/09/18.
//


#include <memory.h>
#include <malloc.h>
#include <ctype.h>
#include "media_type.h"

char type [BUFF_SIZE];
char subType[BUFF_SIZE];
int typeIndex = 0;
int subTypeIndex = 0;

mediaType_t * addMediaType(mediaType_t * dest, mediaType_t * toAdd)
{
    if (toAdd == NULL)
    {
        return dest;
    }

    if (dest == NULL)
    {
        return toAdd;
    }

    mediaType_t * ptr = dest;

    if (toAdd->type[0] == '*')
    {
        if (dest->type[0] =='*')
        {
            if (toAdd->subType->subType[0] == '*')
            {
                if (dest->subType->subType[0] == '*')
                {
                    return dest;
                }
                toAdd->subType->next = dest->subType;
                dest->subType = toAdd->subType;
                freeOneMediaType(toAdd);
                return dest;
            }
            mediaSubType_t * subTypeIterator = dest->subType;
            addSubType(subTypeIterator,toAdd);
        }
        toAdd->next = dest;
        dest = toAdd;
        return dest;
    }

    while (ptr != NULL)
    {
        if (strcmp(dest->type, toAdd->type) == 0)
        {
            mediaSubType_t * subTypeIterator = dest->subType;
            addSubType(subTypeIterator, toAdd);
            return dest;
        }
        if (ptr->next == NULL)
        {
            ptr->next = toAdd;
            return dest;
        }
        ptr = ptr->next;
    }
    return dest;
}


void addSubType(mediaSubType_t * subTypeIterator, mediaType_t * toAdd)
{
    while (subTypeIterator != NULL)
    {
        if (strcmp(subTypeIterator->subType, toAdd->subType->subType) == 0)
        {
            freeMediaType(toAdd);
            return;
        }
        if (subTypeIterator->next == NULL)
        {
            subTypeIterator->next = toAdd->subType;
            freeOneMediaType(toAdd);
            return;
        }
        subTypeIterator= subTypeIterator->next;
    }
}


mediaType_t * newMediaType(char * type, int typeLen, char * subType, int subTypeLen)
{
    mediaType_t * thisMediaType = malloc(sizeof(mediaType_t));
    thisMediaType->next = NULL;
    thisMediaType->type = malloc((size_t) typeLen);
    strncpy(thisMediaType->type, type, (size_t) typeLen);

    mediaSubType_t * thisSubMediaType = malloc(sizeof(mediaSubType_t));
    thisSubMediaType->subType = malloc((size_t) subTypeLen);
    strncpy(thisSubMediaType->subType, subType, (size_t) subTypeLen);
    thisSubMediaType->next = NULL;

    thisMediaType->subType = thisSubMediaType;
    return thisMediaType;
}

void freeOneMediaType(mediaType_t * toFree)
{
    free(toFree->type);
    free(toFree);
}

void freeMediaType(mediaType_t * myType)
{
    if (myType == NULL)
    {
        return;
    }
    freeMediaType(myType->next);
    freeSubMediaType(myType->subType);
    free(myType->type);
    free(myType);
    return;
}

void freeSubMediaType(mediaSubType_t * mySubType)
{
    if (mySubType == NULL)
    {
        return;
    }
    freeSubMediaType(mySubType->next);
    free(mySubType->subType);
    free(mySubType);
    return;
 }


mediaType_t * makeMediaRange(char * mediaRange)
{
    mediaType_t * head = NULL;          //HEAD OF ACCEPT-RANGE

    int i = 0;

    typeIndex = 0;
    subTypeIndex = 0;

    enum status status1;
    status1 = TYPE;                      // SHOW THE STATUS

    while (mediaRange[i] != '\0')
    {
        switch (status1)
        {
            case TYPE:
            {
                if (typeIndex > BUFF_SIZE-1)
                {
                    status1 = ERROR;
                    break;
                }
                if (isalpha(mediaRange[i]))
                {
                    type[typeIndex] = (char) tolower(mediaRange[i]);
                    typeIndex++;
                    i++;
                    break;
                }
                else if (mediaRange[i] == '*' && typeIndex == 0)
                {
                    type[typeIndex] = mediaRange[i];
                    typeIndex++;
                    i++;
                    status1 = ASTERISK1;
                    break;
                }
                else if(mediaRange[i] == '/' && typeIndex > 0)
                {
                    type[typeIndex]='\0';
                    status1 = SUBTYPE;
                    i++;
                    break;
                } else {
                    status1 = ERROR;
                    break;
                }
            }
            case SUBTYPE:
            {
                if (subTypeIndex > BUFF_SIZE-1)
                {
                    status1 = ERROR;
                    break;
                }

                if (isalpha(mediaRange[i]))
                {
                    subType[subTypeIndex] = (char) tolower(mediaRange[i]);
                    subTypeIndex++;
                    i++;
                    break;
                }
                else if (mediaRange[i] == '*' && subTypeIndex == 0)
                {
                    subType[subTypeIndex] = mediaRange[i];
                    subTypeIndex++;
                    i++;
                    status1 = ASTERISK2;
                    break;
                }
                else if (isdigit(mediaRange[i]) || mediaRange[i] == '*' || mediaRange[i] == '+' || mediaRange[i]== '.' || mediaRange[i] == '-')
                {
                    subType[subTypeIndex] = mediaRange[i];
                    subTypeIndex++;
                    i++;
                    break;
                }
                else if (mediaRange[i] == ',' || mediaRange[i] == ';')
                {
                    if (type[0] == '*')
                    {
                        status1 = ERROR;
                        break;
                    }
                    subType[subTypeIndex] = '\0';

                    mediaType_t * toAdd = newMediaType(type, typeIndex + 1, subType, subTypeIndex + 1);
                    head = addMediaType(head, toAdd);

                    typeIndex = 0;
                    subTypeIndex = 0;
                    i++;

                    if (mediaRange[i] == ';')
                    {
                        status1 = FINISH;
                    } else {
                        status1 = TYPE;
                    }
                    break;
                } else {
                    status1 = ERROR;
                    break;
                }
            }
            case FINISH:
            {
                if (mediaRange[i] == ',')
                {
                    status1 = TYPE;
                }
                i++;
                break;
            }
            case ERROR:
            {
                if (mediaRange[i] == ',')
                {
                    typeIndex = 0;
                    subTypeIndex = 0;
                    status1 = TYPE;
                }
                i++;
                break;
            }
            case ASTERISK1:
            {
                if (mediaRange[i] == '/')
                {
                    type[typeIndex]='\0';
                    status1 = SUBTYPE;
                    i++;
                    break;
                } else {
                    status1 = ERROR;
                    break;
                }
            }
            case ASTERISK2:
            {
                if (mediaRange[i] == ',' || mediaRange[i] == ';')
                {
                    subType[subTypeIndex] = '\0';

                    mediaType_t * toAdd = newMediaType(type, typeIndex + 1, subType, subTypeIndex + 1);
                    head = addMediaType(head, toAdd);

                    typeIndex = 0;
                    subTypeIndex = 0;
                    i++;

                    if (mediaRange[i] == ';')
                    {
                        status1 = FINISH;
                    } else {
                        status1 = TYPE;
                    }
                    break;
                } else {
                    status1 = ERROR;
                    break;
                }
            }
        }

    }

    if (status1 == SUBTYPE && subTypeIndex > 0 && type[0] != '*')
    {
        subType[subTypeIndex] = '\0';
        mediaType_t * toAdd = newMediaType(type, typeIndex + 1, subType, subTypeIndex + 1);
        head = addMediaType(head, toAdd);
    }
    if (status1 == ASTERISK2)
    {
        subType[subTypeIndex] = '\0';
        mediaType_t * toAdd = newMediaType(type, typeIndex + 1, subType, subTypeIndex + 1);
        head = addMediaType(head, toAdd);
    }

    return head;
}



/*
 * Returns 0 if is TRUE;
 * Return -1 if is NULL
 * Returns 1 if is false;
 */

int isAcceptable(mediaType_t * from, mediaType_t * toAccept)
{
    if (toAccept == NULL)
    {
        return printNull;
    }

    if (from->type[0] == '*' && from->subType->subType[0] == '*')
    {
        return printTrue;
    }

    mediaType_t * typeIterator = from;
    mediaSubType_t * subTypeIterator = NULL;
    while (typeIterator != NULL)
    {
        if (strcmp(typeIterator->type, toAccept->type) == 0)
        {
            subTypeIterator = typeIterator->subType;
            if (subTypeIterator->subType[0] =='*')
            {
                return printTrue;
            }

            while (subTypeIterator != NULL)
            {
                if (strcmp(typeIterator->subType->subType,toAccept->subType->subType) == 0)
                {
                    return printTrue;
                }
                subTypeIterator = subTypeIterator->next;
            }
        }

        typeIterator = typeIterator->next;
    }
    return printFalse;
}

void printRange (mediaType_t * toPrint)
{
    if (toPrint == NULL)
    {
        printf("The media range to Print is EMPTY \n");
    }
    printf("The medias accept Range is: \n");
    mediaType_t * typeIterator = toPrint;
    mediaSubType_t * subTypeIterator = NULL;

    while (typeIterator != NULL)
    {
        subTypeIterator = typeIterator->subType;
        while (subTypeIterator != NULL)
        {
            printf ("%s / %s ,\n",typeIterator->type,subTypeIterator->subType);
            subTypeIterator = subTypeIterator->next;
        }
        typeIterator = typeIterator->next;
    }
}
