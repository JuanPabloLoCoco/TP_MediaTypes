//
// Created by lococo on 06/09/18.
//

#ifndef UNTITLED_MEDIA_TYPE_H
#define UNTITLED_MEDIA_TYPE_H

typedef struct mediaType{
    struct mediaType * next;
    char * type;
    struct mediaSubType * subType;

}mediaType_t;

typedef struct mediaSubType{
    struct mediaSubType * next;
    char * subType;
}mediaSubType_t;

enum status {TYPE, SUBTYPE, FINISH, ERROR, ASTERISK1, ASTERISK2};

#define printTrue 1
#define printFalse 0
#define printNull -1

int isAcceptable(mediaType_t * from, mediaType_t * toAccept);
void printRange (mediaType_t * toPrint);
mediaType_t * makeMediaRange(char * mediaRange);
void increaseArray (char * array, int i);
void addSubType(mediaSubType_t * subTypeIterator, mediaType_t * toAdd);
void freeOneMediaType(mediaType_t * toFree);
mediaType_t * addMediaType(mediaType_t * dest, mediaType_t * toAdd);
mediaType_t * newMediaType(char * type, int typeLen, char * subType, int subTypeLen);
void freeSubMediaType(mediaSubType_t * head);
void freeMediaType(mediaType_t * head);

#endif //UNTITLED_MEDIA_TYPE_H
