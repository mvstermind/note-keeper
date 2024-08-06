#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

// Struct definition
struct TerminalSize {
    int columns;
    int rows;
};

// Function declarations
struct TerminalSize terminal_size(void);


int main(void) {
    struct TerminalSize size = terminal_size();
    if (size.columns == -1 && size.rows == -1) {
        return EXIT_FAILURE;
    }

    printf("Columns: %d\nRows: %d\n", size.columns, size.rows);
    return EXIT_SUCCESS;
}

struct TerminalSize terminal_size(void) {
    struct winsize ws;
    struct TerminalSize termSize = { -1, -1 }; // Default values in case of error

    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return termSize;
    }

    termSize.columns = ws.ws_col;
    termSize.rows = ws.ws_row;

    return termSize;
}

