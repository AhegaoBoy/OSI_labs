#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef enum enum_key
{
    WRONG_KEY_IDENTIFIED = 1,
    XOR8,
    XOR32,
    MASK
}KEY;

typedef enum enum_errors {
    WRONG_QUANTITY_OF_ARGUMENTS = 1,
    FILE_ERROR,
    WRONG_KEY,
    NOT_IN_BASE,
    INCORRECT_SYMBOL
}ERRORS;

unsigned int key_validation(char* str);
unsigned int for_xor_8(FILE* input);
unsigned int for_xor_32(FILE* input);
unsigned int for_mask(FILE* input, unsigned int hex);
int binpow (int a, int n);
unsigned int convert_to_10(char* str, int base);
int validations_of_symbol(char* str, int base);

int main(int argc, char* argv[])
{
    if(argc < 3 || argc > 4)
    {
        printf("Wrong quantity of arguments, you need insert at least <FILE_DIRECTORY> <KEY> (<MASK> for key MASK)\n");
        return WRONG_QUANTITY_OF_ARGUMENTS;
    }

    char* dir = argv[1];
    int val_of_hex;
    FILE* file = fopen(dir, "rb");
    if(file == NULL)
    {
        perror("YOU'VE INPUT THE DIRECTORY OF THE FILE WRONGLY, TRY AGAIN");
        return FILE_ERROR;
    }
    unsigned int key = key_validation(argv[2]);
    switch(key)
    {
        case XOR8:
            printf("MODULO 2 SUMMARY OF ALL BYTES IN FILE WITH DIRECTORY %s\nEQUALS: %u\n", dir, for_xor_8(file));
            fclose(file);
            return 0;
        case XOR32:
            printf("MODULO 2 SUMMARY OF ALL 4-BYTES GROUPS IN FILE WITH DIRECTORY %s\nEQUALS: %u\n", dir, for_xor_32(file));
            fclose(file);
            return 0;
        case MASK:
            val_of_hex = validations_of_symbol(argv[3], 16);
            if(val_of_hex != 0)
            {
                printf("INCORRECT INPUT OF NUMBER IN BASE 16\n");
                return val_of_hex;
            }

            printf("QUANTITY OF 4-BYTES GROUPS, EQUALS TO HEX <%s>\tIN FILE: %s\nIS: %u",argv[3], dir, for_mask(file,convert_to_10(argv[3], 16)));
            fclose(file);
            return 0;
        default:
            printf("YOU'VE INPUT THE WRONG REY, PLEASE, USE ONLY <XOR32> <XOR32> <MASK> + 'MASK_IN_16_SYSTEM'\n");
            fclose(file);
            return WRONG_KEY;
    }
}

unsigned int key_validation(char* str)
{
    int cmp_xor8 = strcmp(str, "xor8");
    int cmp_xor32 = strcmp(str, "xor32");
    int cmp_mask = strcmp(str, "mask");
    if(cmp_mask != 0 && cmp_xor32 != 0 && cmp_xor8 != 0) return WRONG_KEY_IDENTIFIED;
    if(cmp_xor8 == 0) return XOR8;
    if(cmp_xor32 == 0) return XOR32;
    if(cmp_mask == 0) return MASK;
}

unsigned int for_xor_8(FILE* input)
{
    unsigned int result = 0;
    unsigned char c;
    while(!feof(input))
    {
        fread(&c, sizeof(unsigned char), 1, input);
        if(feof(input)) break;
        result ^= c;
    }
    return result;
}

unsigned int for_xor_32(FILE* input)
{
    unsigned int result = 0;
    unsigned char c;
    while(!feof(input))
    {
        int i = 1;
        unsigned int current_group = 0;
        for(i; i<=4;  ++i)
        {
            fread(&c, sizeof(unsigned char), 1, input);
            if(feof(input)) break;
            unsigned int tmp = 0;
            tmp |= c;
            tmp <<= 8*(4 - i);
            current_group |=tmp;
        }
        result ^= current_group;
    }
    return result;
}

int binpow (int a, int n)
{
    int res = 1;
    while (n) {
        if (n & 1)
            res *= a;
        a *= a;
        n >>= 1;
    }
    return res;
}

unsigned int convert_to_10(char* str, int base)
{
    int length = strlen(str);
    int pow = 0;
    int result = 0;
    for(int i = length - 1; i>=0; i--)
    {
        if(isdigit(str[i]))
        {
            int tmp = (int)(str[i] - '0');
            result += tmp * (binpow(base, pow++));
        }
        else
        {
            result += ((int)(str[i] - 'a') + 10)*binpow(base, pow++);
        }
    }
    return result;
}

int validations_of_symbol(char* str, int base)
{
    int i;
    if(str[0] == '-') i = 1;
    else i = 0;
    for(i; i<strlen(str); ++i)
    {
        if(isdigit(str[i]))
        {
            if(str[i] - '0' >= base) return NOT_IN_BASE;
        }
        else
        {
            if(str[i] >= 'a' && str[i] <= 'z')
            {
                if(str[i] - 'a' + 10 >= base) return NOT_IN_BASE;
            }
            else
            {
                return INCORRECT_SYMBOL;
            }
        }
    }
    return 0;
}

unsigned int for_mask(FILE* input, unsigned int hex)
{
    int count = 0;
    unsigned char c;
    while(!feof(input))
    {
        unsigned int current_number = 0;
        int i = 1;
        for(i; i<=4; ++i)
        {
            fread(&c, sizeof(unsigned char), 1, input);
            if(feof(input)) break;
            unsigned int tmp = 0;
            tmp |= c;
            current_number |= tmp;
            if(current_number == hex)
            {
                count++;
                break;
            }
            current_number <<= 8;
        }
        if(feof(input)) break;
        fseek(input, i == 5 ? -3: -i + 1, SEEK_CUR);
    }
    return count;
}
