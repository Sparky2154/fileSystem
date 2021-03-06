#include "simfs.h"

#define SIMFS_FILE_NAME "simfsFile.dta"

int main()
{
//    srand(time(NULL)); // uncomment to get true random values in get_context()

    unsigned long long test = UINTMAX_MAX;
    printf("MAX unique identifier: %llu\n", test);

    if (simfsCreateFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    // TODO: implement thorough testing of all the functionality

    char* fileName = "myFirstFile";
    if (simfsCreateFile(fileName, SIMFS_FILE_CONTENT_TYPE) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);
    SIMFS_FILE_DESCRIPTOR_TYPE* fileDescriptor = malloc(sizeof(SIMFS_FILE_DESCRIPTOR_TYPE));
    if(simfsGetFileInfo(fileName, fileDescriptor))
        exit(EXIT_FAILURE);
    int b = SIMFS_INVALID_OPEN_FILE_TABLE_INDEX;
    if(simfsOpenFile(fileName, &b))
        exit(EXIT_FAILURE);
    if(simfsCloseFile(b))
        exit(EXIT_FAILURE);

    if (simfsDeleteFile(fileName) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);


    // the following is just some sample code for simulating user and process identifiers that are
    // needed in the simfs functions
    int count = 10;
    char *content;
    struct fuse_context *context;
    for (int i = 0; i < count; i++)
    {
        context = simfs_debug_get_context();
        printf("user ID = %02i, process ID = %02i, group ID = %02i, umask = %04o\n",
               context->uid, context->pid, context->gid, context->umask);
        content = simfsGenerateContent(i * 10);
        printf("content = \"%s\"\nhash(content) = %ld\n", content, hash((unsigned char *) content));
    }

    if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    if (simfsMountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    if (simfsUmountFileSystem(SIMFS_FILE_NAME) != SIMFS_NO_ERROR)
        exit(EXIT_FAILURE);

    unsigned char testBitVector[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    simfsFlipBit(testBitVector, 44);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    simfsClearBit(testBitVector, 33);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));
    simfsSetBit(testBitVector, 33);
    printf("Found free block at %d\n", simfsFindFreeBlock(testBitVector));

    return EXIT_SUCCESS;
}
