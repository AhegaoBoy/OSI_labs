#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<string.h>
#include <sys/wait.h>

typedef enum errors
{
    QUANTITY_OF_ARGUMENTS = 1,
    FILE_OPEN_ERROR,
    MEMORY_ALLOCATION_ERROR,
    FINE
}ERRORS;

char** create_array_of_paths(FILE* input, int* quantity_of_files);
int find_str_in_file(char* directory, char* str, int len_of_str, int* status);
char* get_string(int* len);


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("INVALID AMOUNT OF ARGUMENTS\n");
        return QUANTITY_OF_ARGUMENTS;
    }

    FILE *file_with_files = NULL;
    file_with_files = fopen(argv[1], "r");
    if (file_with_files == NULL) {
        perror("FILE OPEN ERROR");
        return FILE_OPEN_ERROR;
    }

    int status_code = FINE;
    char* string = "\n";
    int len = strlen(string);

    int quantity_of_files = 0;
    char **files = create_array_of_paths(file_with_files, &quantity_of_files);
    if (files == NULL) {
        for (int i = 0; i < quantity_of_files; ++i) free(files[i]);
        free(files);
        fclose(file_with_files);
        return MEMORY_ALLOCATION_ERROR;
    }
    fclose(file_with_files);

    for (int i = 0; i < quantity_of_files; ++i) {
        pid_t child = fork();
        int status;
        waitpid(child, &status, 0);
        if (child < 0) {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        if(child == 0)
        {
            int status;
            waitpid(child, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS) {
                printf("ERRORS DURING PROCESS\n");
                return WEXITSTATUS(status);
            }
            else
            {
                int count = find_str_in_file(files[i], string, len, &status_code);
               if(status_code != FINE)
               {
                   printf("FILE WITH DIRECTORY %s COULDN'T BEEN OPENED\n", files[i]);
                   exit(status);
               }

                printf("QUANTITY OF STRING %s IN FILE WITH DIRECTORY %s\t%d\n", string, files[i], count);
                exit(EXIT_SUCCESS);
            }
        }
    }
    return 0;
}


char** create_array_of_paths(FILE* input, int* quantity_of_files)
{
    char** files = (char**) malloc(sizeof(char*));
    if(files == NULL)
    {
        return NULL;
    }
    files[0] = (char*)malloc(sizeof(char));
    if(files[0] == NULL)
    {
        free(files);
        return NULL;
    }

    int capacity = 1;
    int path_number = 0;
    int length_of_path = 0;
    int capacity_of_path = 1;
    char c;
    while(!feof(input))
    {
        c = fgetc(input);
        if(c == EOF) break;
        if(c == '\n' || c == '\t' || c==' ')
        {
            path_number++;
            if(path_number == capacity) {
                char **for_realloc = (char **) realloc(files, 2 * capacity * sizeof(char *));
                capacity <<= 1;
                if (for_realloc == NULL) {
                    free(files[0]);
                    free(files);
                    return NULL;
                }
                files = for_realloc;

                for (int i = capacity - path_number; i < capacity; ++i) {
                    files[i] = (char *) malloc(sizeof(char));
                    if (files[i] == NULL) {
                        free(files);
                        return NULL;
                    }
                }
            }
            length_of_path = 0;
            capacity_of_path = 1;
        }
        else
        {
            if(capacity_of_path == length_of_path)
            {
                files[path_number] = (char*)realloc(files[path_number], capacity_of_path * 2 * sizeof (char));
                if(files[path_number] == NULL) {
                    free(files);
                    return NULL;
                }

                capacity_of_path<<=1;
            }
            files[path_number][length_of_path++] = c;
        }
    }
    *quantity_of_files = path_number;
    return files;
}
int find_str_in_file(char* directory, char* str, int len_of_str, int* status)
{
    FILE* input = fopen(directory, "r");
    if(input == NULL)
    {
        *status = FILE_OPEN_ERROR;
        return 1;
    }
    int count = 0;
    if(str[0] == '\0')
    {
        while(!feof(input))
        {
            char c = getc(input);
            count++;
        }

    }

    while(!feof(input))
    {
        char c = getc(input);
        if(c == str[0])
        {
            int i;
            for (i = 1; i < len_of_str; ++i)
            {
                c = getc(input);
                if (c != str[i] || feof(input)) break;
            }
            if(i == len_of_str)
            {
                count++;
                fseek(input, -len_of_str + 1, SEEK_CUR);
            }
            else fseek(input, -i+1, SEEK_CUR);
        }
    }
    fclose(input);
    return count;
}