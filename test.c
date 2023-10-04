#include "types.h"
#include "stat.h"
#include "user.h"
#include <stddef.h>

char *uniq_args[] = { "uniq", "file.txt", NULL };
char *head_args[] = { "head", "-n", "10", "file.txt", NULL };

void getAndPrintProcessInfo(char* processName) {
  struct time_info t;

  if (waitx(&t) < 0) {
    printf(1, "Error: Could not retrieve process information for %s\n", processName);
  } else {
    printf(1, "Process %s\n", processName);
    printf(1, "Creation Time: %d ms\n", t.creation_time);
    printf(1, "End Time: %d ms\n", t.end_time);
    printf(1, "Total Time: %d ms\n", t.total_time);
  }
}

int main(int argc, char *argv[])
{
  int pid1, pid2;

  // Create pipes for capturing the output of uniq and head
  int pipe_uniq[2];
  int pipe_head[2];

  if (pipe(pipe_uniq) < 0 || pipe(pipe_head) < 0) {
    printf(1, "Error: Pipe creation failed\n");
    exit();
  }

  // Launch the 'uniq' command and redirect its output to the pipe
  pid1 = fork();
  if (pid1 == 0) {
    close(pipe_uniq[0]); // Close read end of pipe_uniq
    dup(pipe_uniq[1]);   // Redirect stdout to the write end of pipe_uniq
    close(pipe_uniq[1]); // Close the write end of pipe_uniq
    exec("uniq", uniq_args);
    printf(1, "Error: exec failed for uniq\n");
    exit();
  } else if (pid1 < 0) {
    printf(1, "Error: fork failed for uniq\n");
    exit();
  }

  sleep(100);

  // Launch the 'head' command and redirect its output to the pipe
  pid2 = fork();
  if (pid2 == 0) {
    close(pipe_head[0]); // Close read end of pipe_head
    dup(pipe_head[1]);   // Redirect stdout to the write end of pipe_head
    close(pipe_head[1]); // Close the write end of pipe_head
    exec("head", head_args);
    printf(1, "Error: exec failed for head\n");
    exit();
  } else if (pid2 < 0) {
    printf(1, "Error: fork failed for head\n");
    exit();
  }

  // Close write ends of pipes in the parent process
  close(pipe_uniq[1]);
  close(pipe_head[1]);

  // Read and print the output of uniq
  char buffer[512];
  int n;

  printf(1, "Output of uniq:\n");
  while ((n = read(pipe_uniq[0], buffer, sizeof(buffer))) > 0) {
    write(1, buffer, n);
  }

  // Read and print the output of head
  printf(1, "\nOutput of head:\n");
  while ((n = read(pipe_head[0], buffer, sizeof(buffer))) > 0) {
    write(1, buffer, n);
  }

  // Wait for the children to finish and retrieve their information
  getAndPrintProcessInfo("uniq");
  getAndPrintProcessInfo("head");

  // Wait for the children to finish
  wait();
  wait();

  exit();
}
