#include<stdio.h>

typedef enum enum_errors
{
    ARGUMENTS_QUANTITY = 1,
    FILE_DIRECTORY,
    INPUT_OUTPUT_ERROR
}ERRORS;


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Введите нужное количество аргументов\n");
        return ARGUMENTS_QUANTITY;
    }
    char buf[5];
    char* dir = *(argv + 1);
    FILE* insert;
    insert = fopen(dir, "wb");
    if(insert == NULL)
    {
        perror("FILE HASN'T BEEN OPENED FOR WRITING");
        return FILE_DIRECTORY;
    }

    char* str = "Hello World!";
    if (fwrite(str, sizeof(char),  12, insert) != 12)
    {
        printf("INSERTING STRING %s HAS BEEN FAILED\n", str);
        fclose(insert);
        return INPUT_OUTPUT_ERROR;
    }
    fclose(insert);

    FILE* output;
    output = fopen(dir, "rb");
    if(output == NULL)
    {
        perror("FILE HASN'T BEEN OPENED FOR READING");
        return INPUT_OUTPUT_ERROR;
    }

    char c = fgetc(output);
    while (!feof(output))
    {
        printf("Current byte: %c\n", c);
        printf("\tFlag: %d\n", output->_flags);
        printf("\tCurrent read pointer: %s\n", output->_IO_read_ptr);
        printf("\tEnd of get area: %s\n", output->_IO_read_end);
        printf("\tStart of putback + get area: %s\n", output->_IO_read_base);
        printf("\tStart of put area: %s\n", output->_IO_write_base);
        printf("\tCurrent put pointer: %s\n", output->_IO_write_ptr);
        printf("\tEnd of put area: %s\n", output->_IO_write_end);
        printf("\tStart of reverse area: %s\n", output->_IO_buf_base);
        printf("\tEnd of reverse area: %s\n", output->_IO_buf_end);
        printf("The following fields are used to support backing up and undo\n");
        printf("\tThe pointer to start of non-current get area: %s\n", output->_IO_save_base);
        printf("\tPointer to first backup area: %s\n", output->_IO_backup_base);
        printf("\tPointer to end of non-current get area: %s\n", output->_IO_save_end);
        printf("\t_flags2: %d\n", output->_flags2);
        printf("\tfileno: %d\n", output->_fileno);
        printf("Information about column\n");
        printf("\tCurrent column: %d\n", output->_cur_column);
        printf("\t_vtable_offset (Virtual table): %c\n", output->_vtable_offset);
        printf("\t_shortbuf: %s\n", output->_shortbuf);
        c = fgetc(output);
    }
    fclose(output);
    output = fopen(argv[1], "r");
    if(output == NULL)
    {
        printf("Can't open file at %s\n", argv[1]);
        return 0;
    }
    fseek(output, 3, SEEK_SET);
    fread(&buf, sizeof(char), 4, output);
    fclose(output);
    printf("\n%s", buf);
    return 0;
}