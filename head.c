// Include necessary headers
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

// Define a constant for the default number of lines to display
#define DEFAULT_LINES 14

// Function to display usage instructions and exit
void usage(void) {
    printf(1, "Usage: head [-n <number>] [<filename>]\n");
    exit();
}

// Function to determine the number of lines the user wants to print
int get_lines_to_print(int argc, char **argv, int *offset) {
    // Check if user passed the "-n" flag
    if (argc >= 2 && strcmp(argv[1], "-n") == 0) {
        // If not enough arguments are provided after "-n", display usage
        if (argc < 4) {
            usage();
        }
        // Set the offset to adjust argv for filename
        *offset = 2;
        // Return the user-specified number of lines to print
        return atoi(argv[2]);
    }
    // If no "-n" flag, return default number of lines
    return DEFAULT_LINES;
}

// Function to print the specified number of lines from the file or standard input
void print_head(int fd, int lines_to_print) {
    char buf[512]; // Buffer to store data read from file
    int n, line_count = 0, i;

    // Keep reading from the file or standard input
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        for (i = 0; i < n; i++) {
            printf(1, "%c", buf[i]); // Print each character
            // If a newline character is found
            if (buf[i] == '\n') {
                line_count++;
                // If the desired number of lines are printed, exit the loop
                if (line_count == lines_to_print) {
                    return;
                }
            }
        }
    }

    // Handle read errors
    if (n < 0) {
        printf(1, "head: read error\n");
    }
}

int main(int argc, char *argv[]) {
    int fd = 0, offset = 0; // Default file descriptor and offset values
    int lines_to_print;

    // Notify that command is executed in user mode
    printf(1, "Head command is getting executed in user mode,\n");

    // Get the number of lines to print
    lines_to_print = get_lines_to_print(argc, argv, &offset);
    // Adjust argc and argv based on offset
    argc -= offset;
    argv += offset;

    // If a filename is provided
    if (argc > 1) {
        fd = open(argv[1], O_RDONLY); // Open the file in read-only mode
        if (fd < 0) {
            printf(1, "head: cannot open %s\n", argv[1]); // Handle file opening errors
            exit();
        }
    }

    // Print the head of the file or standard input
    print_head(fd, lines_to_print);

    // If a file was opened, close it
    if (fd != 0) close(fd);
    exit();
}
