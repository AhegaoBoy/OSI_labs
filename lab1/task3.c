#include<stdio.h>

typedef enum enum_errors
{
    ARGUMENTS_QUANTITY = 1,
    FILE_DIRECTORY
}ERRORS;

void copy_file(FILE* input, FILE* output);

int main(int argc, char* argv[])
{
    if(argc!=3)
    {
        printf("INVALID QUANTITY OF ARGUMENTS\n");
        return ARGUMENTS_QUANTITY;
    }
    FILE* input = NULL;
    FILE* output = NULL;
    char* dir_in = argv[1];
    char* dir_out = argv[2];
    input = fopen(dir_in, "rb");
    output = fopen(dir_out, "wb");

    if(input == NULL)
    {
        perror("INVALID INPUT FILE DIRECTORY");
        fclose(output);
        return FILE_DIRECTORY;
    }
    if(output == NULL)
    {
        perror("INVALID OUTPUT FILE DIRECTORY");
        fclose(input);
        return FILE_DIRECTORY;
    }

    copy_file(input, output);

    fclose(input);
    fclose(output);
    printf("FILE WITH DIRECTORY %s\nWAS SUCCESSFULLY COPIED IN FILE WITH DIRECTORY %s\n", dir_in, dir_out);
    return 0;
}

void copy_file(FILE* input, FILE* output)
{
    unsigned char c;
    while(!feof(input))
    {
        fread(&c, sizeof(unsigned char), 1, input);
        if(feof(input)) break;
        fwrite(&c, sizeof(unsigned char), 1 ,output);
    }
}