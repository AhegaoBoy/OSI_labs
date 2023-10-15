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
    unsigned char buf[5];
    char* dir = *(argv + 1);
    FILE* insert;
    insert = fopen(dir, "wb");
    if(insert == NULL)
    {
        perror("FILE HASN'T BEEN OPENED FOR WRITING");
        return FILE_DIRECTORY;
    }

    unsigned char byte_3 = 3;
    unsigned char byte_1 = 1;
    unsigned char byte_4 = 4;
    unsigned char byte_5 = 5;
    unsigned char byte_9 = 9;
    unsigned char byte_2 = 2;
    unsigned char byte_6 = 6;
    if (fwrite(&byte_3, sizeof(byte_3), 1, insert) != 1 || fwrite(&byte_1, sizeof(byte_1), 1, insert) != 1 || fwrite(&byte_4, sizeof(byte_4), 1, insert) != 1 || fwrite(&byte_1, sizeof(byte_1), 1, insert) != 1 || fwrite(&byte_5, sizeof(byte_5), 1, insert) != 1 || fwrite(&byte_9, sizeof(byte_9), 1, insert) != 1 || fwrite(&byte_2, sizeof(byte_2), 1, insert) != 1 ||fwrite(&byte_6, sizeof(byte_6), 1, insert) != 1 || fwrite(&byte_5, sizeof(byte_5), 1, insert) != 1 || fwrite(&byte_3, sizeof(byte_3), 1, insert) != 1 || fwrite(&byte_5, sizeof(byte_5), 1, insert) != 1)
    {
        printf("INSERTING BYTES HAS BEEN FAILED\n");
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

    unsigned char c;
    do
    {
        fread(&c, sizeof(c), 1, output);
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

    }while (!feof(output));
    fclose(output);
    output = fopen(argv[1], "rb");
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