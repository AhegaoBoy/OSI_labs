#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>

#define MAX_PINCODE 100000
#define STR_SIZE 256

typedef enum status_code {
    SUCCESS = 1,
    MEMORY_ALLOCATION_ERROR,
    INVALID_PARAM,
    REGISTER_AUTHORIZED,
    REGISTER_NON_AUTHORIZED,
    NOT_NUMBER
} status_code;

typedef enum status_cmd {
    TIME,
    DATE,
    LOGOUT,
    HOWMUCH,
    SANCTIONS,
    MEMORY_ERROR,
    NOT_PARAM,
    EXIT
} cur_command;


typedef struct User {
    char login[6];
    int pincode;
    int sanctions;
} User;

typedef struct Node {
    User* data;
    struct Node* next;
    struct Node* prev;
} Node;

void free_all(void* pointer,...);
cur_command command(char** arg_one, char** arg_two);
status_code register_user(Node** head, User** person);
status_code find_user(Node* list, const char* log, int pin, User** user);
void destroy_storage(Node** head);
status_code create_user(const char* log, int pin, User** person);
status_code make_sanctions(Node* list, const char* log, char* number, User* user);
status_code get_elapsed_time(char* time_start, char *flag);
status_code validation_of_number(char* number);
void print_menu_non_authorized();
void print_menu_authorized();
void get_time();
void get_date();
int get_sanctions(User* id);
char *get_string(int *len);

int main() {
    Node* storage = NULL;
    bool authorized = false;
    int pincode;
    char login[32];
    char password[32];
    int counter_procedures = 0;
    int approve_value;
    User* _user;
    while (true) {
        char *input_arg2 = NULL;
        char *input_arg3 = NULL;
        if (!authorized) {
            print_menu_non_authorized();
            printf("login: ");
            scanf("%s", login);
            getchar();
            printf("Pincode: ");
            scanf("%s", password);
            getchar();
            status_code valid_number = validation_of_number(password);
            if(valid_number != SUCCESS)
            {
                printf("PASSWORD CONTAINS ONLY NUMBERS\n");
                continue;
            }
            pincode = atoi(password);
            status_code creation = create_user(login, pincode, &_user);
            switch (creation) {
                case SUCCESS:
                    break;
                case MEMORY_ALLOCATION_ERROR:
                    printf("MEMORY ALLOCATION ERROR\n");
                    exit(1);
                case INVALID_PARAM:
                    printf("INVALID USERNAME OR PASSWORD\n");
                    continue;
            }
            status_code status_exist = register_user(&storage, &_user);
            switch (status_exist) {
                case REGISTER_AUTHORIZED:
                    printf("You've already created account, no need to registrate again, user: %s\n", login);
                    authorized = true;
                    break;
                case REGISTER_NON_AUTHORIZED:
                    printf("You successfully registered, user: %s\n", login);
                    authorized = true;
                    break;
                case MEMORY_ALLOCATION_ERROR:
                    printf("Error malloc detected!\n");
                    return MEMORY_ALLOCATION_ERROR;
                default:
                    printf("Wrong password, try again :)\n");
            }
        }
        else
        {
            status_code elaps_status;
            status_code sanction_status;
            print_menu_authorized();
            if (_user->sanctions - counter_procedures == 0 && _user->sanctions != 0) {
                _user->sanctions = 0;
                counter_procedures = 0;
                printf("You can`t do anything in this session. Logging out...\n");
                authorized = false;
                free_all(input_arg2, input_arg3, NULL);
                continue;
            }
            if (get_sanctions(_user))
                printf("You have only %d procedures\n", _user->sanctions - counter_procedures);

            cur_command cmd = command(&input_arg2, &input_arg3);
            switch (cmd) {
                case TIME:
                    if (get_sanctions(_user))
                        counter_procedures++;
                    get_time();
                    break;
                case DATE:
                    if (get_sanctions(_user))
                        counter_procedures++;
                    get_date();
                    break;
                case HOWMUCH:
                    elaps_status = get_elapsed_time(input_arg2, input_arg3);
                    if (elaps_status == INVALID_PARAM)
                        printf("Invalid parameter detected, try again!\n");

                    if (get_sanctions(_user) != 0 && elaps_status != INVALID_PARAM)
                        counter_procedures++;
                    break;
                case SANCTIONS:
                    printf("Enter 12345 to confirm: ");
                    scanf("%d", &approve_value);
                    getchar();
                    if (approve_value == 12345) {
                        sanction_status = make_sanctions(storage, input_arg2, input_arg3, _user);
                        if (sanction_status == INVALID_PARAM)
                            printf("Invalid parameter detected, try again!\n");
                        if (get_sanctions(_user) && sanction_status != INVALID_PARAM)
                            counter_procedures++;
                    }
                    else printf("Cancelled!\n");
                    break;
                case LOGOUT:
                    if (get_sanctions(_user) != 0)
                    {
                        counter_procedures = 0;
                        _user->sanctions = 0;
                    }
                    printf("You have successfully logged out of your account");
                    authorized = false;
                    break;
                case MEMORY_ERROR:
                    printf("Error malloc detected!!!\n");
                    destroy_storage(&storage);
                    exit(2);
                case NOT_PARAM:
                    printf("Invalid parameter detected, try again!\n");
                    break;
                case EXIT:
                    break;
            }
            free_all(input_arg2, input_arg3, NULL);
            if (cmd == EXIT) {
                break;
            }
        }
    }


    destroy_storage(&storage);
    return 0;
}
status_code validation_of_number(char* number)
{
    int i = 0;
    while(number[i] != '\0')
    {
        if(!isdigit(number[i])) return NOT_NUMBER;
        ++i;
    }
    return SUCCESS;
}

int get_sanctions(User* id)
{
    return id->sanctions;
}

status_code register_user (Node** head, User** person) {
    char* log = (*person)->login;
    int pin = (*person)->pincode;
    status_code status_exit = find_user(*head, log, pin, person);

    if(status_exit == REGISTER_AUTHORIZED) return status_exit;


    (*person)->sanctions = 0;
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    new_node->data = (*person);
    new_node->next = NULL;

    if (*head == NULL) {
        new_node->prev = NULL;
        *head = new_node;
        return status_exit;
    }

    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;
    new_node->prev = current;
    return status_exit;
}

void destroy_storage(Node** head) {
    Node* current = *head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    *head = NULL;
}

status_code create_user(const char* log, int pin, User** person) {
    if (pin > MAX_PINCODE || strlen(log) > 6) {
        return INVALID_PARAM;
    }
    *person = malloc(sizeof(User));
    if (*person == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy((*person)->login, log);
    (*person)->pincode = pin;
    (*person)->sanctions = 0;
    return SUCCESS;
}

status_code find_user(Node* list, const char* log, int pin, User** user) {
    if (strlen(log) > 6) {
        return INVALID_PARAM;
    }
    Node* current = list;
    while (current != NULL) {
        User* check = current->data;
        if (!strcmp(check->login, log) && check->pincode != pin) {
            return INVALID_PARAM;
        } else if (!strcmp(check->login, log) && check->pincode == pin) {
            free(*user);
            *user = check;
            return REGISTER_AUTHORIZED;
        }
        current = current->next;
    }
    return REGISTER_NON_AUTHORIZED;
}

status_code make_sanctions(Node* list, const char* log, char* number, User* user) {
    if (strlen(log) > 6) {
        return INVALID_PARAM;
    }
    printf("%s %s %d\n", user->login, log, user->sanctions);
    if (!strcmp(user->login, log) && user->sanctions != 0) {
        return INVALID_PARAM;
    }
    int number_sanctions;
    sscanf(number, "%d", &number_sanctions);
    if (number_sanctions < 1) {
        return INVALID_PARAM;
    }
    Node* current = list;
    while (current != NULL) {
        User* user = current->data;
        if (!strcmp(user->login, log)) {
            user->sanctions = number_sanctions;
            return SUCCESS;
        }
        current = current->next;
    }
    return INVALID_PARAM;
}


void print_menu_non_authorized() {
    printf("\n|===============|Welcome to the programm!!!|===============|\n");
    printf("| Enter your login and password to continue                |\n");
    printf("|==========================================================|\n");
}

void print_menu_authorized() {
    printf("|==========================================================|\n");
    printf("| Enter \"Time\" to know whats time is now                 |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Date\" to know whats date is today               |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Howmuch <time> flag\" to know how much time left |\n");
    printf("| after specified date, flag = '-s', '-m', '-h', '-y'      |\n");
    printf("| so you can get answer in sec / min / hours / years       |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Logout\" to log out from account                 |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Sanctions username <number>\" to create sanctions|\n");
    printf("| on specified user with specified quantity                |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Exit\" to shut down programm                     |\n");
    printf("|==========================================================|\n");
}

void get_time() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current time: %02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

void get_date() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current date: %02d:%02d:%04d\n", time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
}

status_code get_elapsed_time(char *time_start, char *flag) {
    if (flag[0] != '-') {
        return INVALID_PARAM;
    }
    if(flag[1] != 's' && flag[1] != 'm' && flag[1] != 'h' && flag[1] != 'y') return INVALID_PARAM;

    struct tm tm_start;
    int day, month, year;
    if (sscanf(time_start, "%d:%d:%d", &day, &month, &year) != 3) {
        return INVALID_PARAM;
    }

    tm_start.tm_year = year - 1900;
    tm_start.tm_mon = month - 1;
    tm_start.tm_mday = day;
    tm_start.tm_hour = 0;
    tm_start.tm_min = 0;
    tm_start.tm_sec = 0;

    time_t start_time = mktime(&tm_start);
    time_t current_time = time(NULL);

    double elapsed_seconds = difftime(current_time, start_time);
    if (elapsed_seconds < 0) {
        return INVALID_PARAM;
    }
    if (flag[1] == 's')
        printf("Elapsed time in seconds: %.0f\n", elapsed_seconds);
    else if (flag[1] == 'm')
        printf("Elapsed time in minutes: %.2f\n", elapsed_seconds / 60.0);
    else if (flag[1] == 'h')
        printf("Elapsed time in hours: %.2f\n", elapsed_seconds / 3600.0);
    else if (flag[1] == 'y')
        printf("Elapsed time in years: %.4f\n", elapsed_seconds / (365.25 * 24 * 3600));

    return SUCCESS;
}

void free_all(void* pointer, ...)
{
    va_list args;
    va_start(args,pointer);
    void* tmp = va_arg(args, void*);
    while(tmp!= NULL)
    {
        free(tmp);
        tmp = va_arg(args, void*);
    }
    va_end(args);
}

char *get_string(int *len)
{
    *len = 0;
    int capacity = 1;
    char *s = (char*) malloc(sizeof(char));
    if(s == NULL) return NULL;

    char c = getchar();

    do{
        if(!isspace(c)) s[(*len)++] = c;
        if (*len >= capacity) {
            capacity *= 2;
            s = (char*) realloc(s, capacity * sizeof(char));
        }
        c = getchar();
    }while (!isspace(c));

    s[*len] = '\0';

    return s;
}

cur_command command(char** arg_one, char** arg_two)
{
    int len_of_cmd, len_of_one, len_of_two;
    char* tmp_cmd = get_string(&len_of_cmd);

    if (!strcmp(tmp_cmd, "Time"))
    {
        free(tmp_cmd);
        return TIME;
    }
    else if (!strcmp(tmp_cmd, "Date"))
    {
        free(tmp_cmd);
        return DATE;
    }
    else if (!strcmp(tmp_cmd, "Exit"))
    {
        free(tmp_cmd);
        return EXIT;
    }
    else if (!strcmp(tmp_cmd, "Howmuch"))
    {
        free(tmp_cmd);
        (*arg_one) = get_string(&len_of_one);
        if(!(*arg_one)) return MEMORY_ERROR;

        (*arg_two) = get_string(&len_of_two);
        if(!(*arg_two))
        {
            free(*arg_one);
            return MEMORY_ERROR;
        }

        return HOWMUCH;
    }
    else if (!strcmp(tmp_cmd, "Sanctions"))
    {
        free(tmp_cmd);
        (*arg_one) = get_string(&len_of_one);
        if (*arg_one == NULL) return MEMORY_ERROR;

        (*arg_two) = get_string(&len_of_two);
        if (*arg_two == NULL)
        {
            free(*arg_one);
            return MEMORY_ERROR;
        }
        return SANCTIONS;
    }
    else if (!strcmp(tmp_cmd, "Logout"))
    {
        free(tmp_cmd);
        return LOGOUT;
    }
    else
    {
        free(tmp_cmd);
        return NOT_PARAM;
    }
}