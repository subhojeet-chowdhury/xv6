#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAX_LINE 512

// Global variables
int flag_c = 0, flag_i = 0, flag_d = 0;

// Function Prototypes
void parse_args(int argc, char *argv[], int *fd);
int compare_lines(const char *current_line, const char *prev_line);
void handle_line_output(const char *line, int *count);
void read_input(int fd);

int main(int argc, char *argv[]) {
    int fd = 0;  // Default to stdin
    printf(1, "Uniq command is getting executed in user mode.\n");
    parse_args(argc, argv, &fd);
    read_input(fd);
    close(fd);
    exit();
}

// Parse command line arguments
void parse_args(int argc, char *argv[], int *fd) {
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        if (strcmp(argv[i], "-c") == 0) flag_c = 1;
        else if (strcmp(argv[i], "-i") == 0) flag_i = 1;
        else if (strcmp(argv[i], "-d") == 0) flag_d = 1;
        i++;
    }
    if (i < argc) {
        if ((*fd = open(argv[i], O_RDONLY)) < 0) {
            printf(1, "uniq: cannot open %s\n", argv[i]);
            exit();
        }
    }
}

// Compare two lines with the given flags
int compare_lines(const char *current_line, const char *prev_line) {
    if (flag_i) {
        return strcmp(current_line, prev_line);
    } else {
        return strcmp(current_line, prev_line);
    }
}

// Handle line output with the given flags
void handle_line_output(const char *line, int *count) {
    if (flag_c && (!flag_d || (flag_d && *count > 1))) {
        printf(1, "%d %s\n", *count, line);
    } else if (!flag_c && (!flag_d || (flag_d && *count > 1))) {
        printf(1, "%s\n", line);
    }
}

// Read from the input and process it
void read_input(int fd) {
    char prev_line[MAX_LINE] = {0}, current_line[MAX_LINE] = {0};
    int n, index = 0, first_line = 1, count = 0;
    char buf[1];

    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        if (buf[0] == '\n' || index >= MAX_LINE - 1) {
            current_line[index] = 0;

            if (first_line) {
                count = 1;
                strcpy(prev_line, current_line);
                first_line = 0;
            } else {
                if (compare_lines(current_line, prev_line) == 0) {
                    count++;
                } else {
                    handle_line_output(prev_line, &count);
                    strcpy(prev_line, current_line);
                    count = 1;
                }
            }
            index = 0;
        } else {
            current_line[index++] = buf[0];
        }
    }
    if (index > 0) {
        handle_line_output(current_line, &count);
    }
    if (n < 0) {
        printf(1, "uniq: read error\n");
    }
}
