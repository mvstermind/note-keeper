#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

// Struct definition
struct TerminalSize {
    int COLUMNS;
    int ROWS;
    int LEFT_PADDING;
    int TOP_PADDING;
};

const int FRAME_WIDTH = 60; // Equivalent to columns
const int FRAME_HEIGHT = 20; // Equivalent to rows

// Function declarations
struct TerminalSize terminal_size(void);
void create_frame(struct TerminalSize sizes);
int check_if_display_help(char **argv);
int check_for_flags(char **argv);
int len_array(char **any);
void use_flags(char **any);

int main(int argc, char** argv) {
    struct TerminalSize size = terminal_size();
    if (size.COLUMNS == -1 && size.ROWS == -1) {
        return EXIT_FAILURE;
    }

    if (size.COLUMNS <= FRAME_WIDTH || size.ROWS <= FRAME_HEIGHT){
        printf("cannot fit frame into the screen\n");
        return EXIT_FAILURE;
    }

    // if this fails, fail whole f-ing program
    if (check_if_display_help(argv) != 0){
        return EXIT_FAILURE;
    }

    if (check_for_flags(argv) != 0){
        return EXIT_FAILURE;
    }

    use_flags(argv);

    create_frame(size);

    return EXIT_SUCCESS;
}

// get terminal size
struct TerminalSize terminal_size(void) {
    struct winsize ws;
    struct TerminalSize termSize = { -1, -1 }; // default values in case of error

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return termSize;
    }

    termSize.COLUMNS = ws.ws_col;
    termSize.ROWS = ws.ws_row;

    return termSize;
}

void create_frame(struct TerminalSize sizes) {
    system("clear");

    sizes.LEFT_PADDING = (sizes.COLUMNS - FRAME_WIDTH) / 2;

    // -1 because top and bottom lines will put this thing off-center
    sizes.TOP_PADDING = (sizes.ROWS - FRAME_HEIGHT - 1) / 2;

    for (int i = 0; i < sizes.TOP_PADDING; i++) {
        printf("\n");
    }

    for (int i = 0; i < sizes.LEFT_PADDING; i++) {
        printf(" ");
    }
    for (int j = 0; j < FRAME_WIDTH; j++) {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < FRAME_HEIGHT - 2; i++) {
        for (int j = 0; j < sizes.LEFT_PADDING; j++) {
            printf(" ");
        }
        printf("|");
        for (int j = 0; j < FRAME_WIDTH - 2; j++) {
            printf(" ");
        }
        printf("|\n");
    }

    for (int i = 0; i < sizes.LEFT_PADDING; i++) {
        printf(" ");
    }
    for (int j = 0; j < FRAME_WIDTH; j++) {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < sizes.TOP_PADDING; i++) {
        printf("\n");
    }
}


int check_if_display_help(char **argv){
    int argc = len_array(argv);

    if (argc == 1){
        printf("use -h or --help for help\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < argc; i++) {
        if ((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)) {
            printf("Usage:\n");
            printf("\t-a to add to a note\n");
            printf("\t-c to add to a note\n");
            printf("\t-v to view notes\n");
            printf("\n");
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}

int check_for_flags(char **argv) {
    int arg_a_appearance = 0;
    int arg_v_appearance = 0;
    int arg_c_appearance = 0;

    for (int i = 0; i < len_array(argv); i++) {
        if (strcmp(argv[i], "-a") == 0) {
            arg_a_appearance++;
        }
        if (strcmp(argv[i], "-v") == 0) {
            arg_v_appearance++;
        }
        if (strcmp(argv[i], "-c") == 0) {
            arg_v_appearance++;
        }
    }

    if (arg_a_appearance == 0 && arg_v_appearance == 0 && arg_c_appearance == 0) {
        printf("No valid flags found. Use -h or --help for help.\n");
        return EXIT_FAILURE;
    }

    if (arg_a_appearance > 1 || arg_v_appearance > 1 || arg_c_appearance > 1) {
        printf("No valid flags found. Use -h or --help for help.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int len_array(char **any){
    int length = 0;
    while (any[length] != NULL) {
        length++;
    }
    return length;
}

void use_flags(char **args){
    for (int i = 0; i < len_array(args); i++){

        if (strcmp(args[i], "-a") == 0) {
            FILE *file = fopen("notes.txt", "a"); 
            if (file != NULL){
                for (int j = i + 1; j < len_array(args); j++) {
                    if (strchr(args[j], '\n') != NULL || j == len_array(args) - 1) {
                        // If \n found or end of array, write and break
                        fprintf(file, "%s\n", args[j]);
                        break;
                    } else {
                        fprintf(file, "%s ", args[j]);
                    }
                }
                fclose(file);
            } else {
                perror("Error opening file");
            }
        }
        if (strcmp(args[i], "-c") == 0) {
            FILE *file = fopen("notes.txt", "w"); 
            if (file != NULL){
                fprintf(file, "");
                fclose(file);
            }
        }
    }
}
