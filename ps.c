 #include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
    int pid = -1; // Default PID, -1 means all processes
    
    // Check if a specific PID is provided as an argument
    if (argc == 2)
    {
        pid = atoi(argv[1]);
    }
    
    // Call the ps system call to display process information
    if (ps(pid) == -1)
    {
        printf(1, "Error: Process with PID %d not found\n", pid);
    }
    
    exit();
}
