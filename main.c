#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

// Struct definition
struct TerminalSize {
    int COLUMNS;
    int ROWS;
    int LEFT_PADDING;
    int TOP_PADDING;
};

const int FRAME_WIDTH = 60; // Equalivent to columns
const int FRAME_HEIGHT = 20; // Equalivent to rows


// Function declarations
struct TerminalSize terminal_size(void);
void create_frame(struct TerminalSize sizes);


int main(void) {
    struct TerminalSize size = terminal_size();
    if (size.COLUMNS == -1 && size.ROWS == -1) {
        return EXIT_FAILURE;
    }

    if (size.COLUMNS <= FRAME_WIDTH || size.ROWS <= FRAME_HEIGHT){
        printf("cannot fit frame into the screen\n");
        return EXIT_FAILURE;
    }
    create_frame(size);



    printf("Columns: %d\nRows: %d\n", size.COLUMNS, size.ROWS);
    return EXIT_SUCCESS;
}

// get terminal size
struct TerminalSize terminal_size(void) {
    struct winsize ws;
    struct TerminalSize termSize = { -1, -1 }; // Default values in case of error

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return termSize;
    }

    termSize.COLUMNS = ws.ws_col;
    termSize.ROWS = ws.ws_row;

    return termSize;
}

// todo finish this thing after reading file
void create_frame(struct TerminalSize sizes){
    system("clear");
    sizes.LEFT_PADDING = (sizes.COLUMNS - FRAME_WIDTH) / 2;

    // -1 this thing beacuse top and bottom line will put this thing off center
    sizes.TOP_PADDING = (sizes.ROWS - FRAME_HEIGHT-1) / 2;

    // lower top line
    for (int i = 0; i < sizes.TOP_PADDING; i++){
        printf("\n");
    }

    // print centered horizontal line (top line of the frame)
    for (int i = 0; i < sizes.LEFT_PADDING;i++) {
        printf(" ");
    }
    for (int j = 0; j < FRAME_WIDTH; j++){
        printf("-");
    }
    printf("\n");



    for (int i = 0; i < sizes.LEFT_PADDING;i++) {
        printf(" ");
    }
    for (int j = 0; j < FRAME_WIDTH; j++){
        printf("-");
    }
    // create space at the bottom of the screen
    for (int i = 0; i < sizes.TOP_PADDING; i++){
        printf("\n");
    }
}
