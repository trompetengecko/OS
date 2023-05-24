#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TEXT_LENGTH 1000
#define MAX_WORDS 100

// Function for fork, execvp and errorhandling
void execCommand(char *arguments[]) {
  int pid, ret;
  pid = fork();

  if (pid < 0) {
    // errorhandling fork
    fprintf(stderr, "\033[0;31mError creating the childprocess.\033[0m\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Child process
    execvp(arguments[0], arguments);
    perror("execvp");
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    // Parent process
    // Wait for the child to exit
    wait(&ret);
    if (ret != 0) {
      printf("\033[0;31mChild exited with %d\033[0m\n\n", ret);
    }
  }
}

// Signal handler for Ctrl+C
void escSeqHandler(int signal) {
  // call execCommand to show the User the info text
  char *exitPrompt[] = {"echo",
                        "\033[0;31mPlease write '\033[0mexit\033[0;31m' to "
                        "quit the program\033[0m",
                        NULL};
  execCommand(exitPrompt);
}

int main(int argc, char *argv[]) {
  // register the handler for strg+c
  signal(SIGINT, escSeqHandler);

  bool programRunning = true;

  while (programRunning) {
    char text[MAX_TEXT_LENGTH];
    char *words[MAX_WORDS];
    int wordCount = 0;

    // show prompt at line beginning. Call always at the beginning of each
    // iteration
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("\033[0;33m%s>\033[0m ", cwd);
    } else {
      perror("getcwd");
      exit(EXIT_FAILURE);
    }

    // user input
    fgets(text, sizeof(text), stdin);
    // if text is empty go to next iteration for a new line
    if (strlen(text) == 1) {
      continue;
    }

    // Remove the newline character at the end of the text
    text[strcspn(text, "\n")] = '\0';

    // Split the text into words
    char *token = strtok(text, " ");
    while (token != NULL && wordCount < MAX_WORDS) {
      words[wordCount] = token;

      // catch the exit input command from user
      if (strcmp(words[wordCount], "exit") == 0) {
        programRunning = false;
        exit(EXIT_SUCCESS);
      }

      wordCount++;
      token = strtok(NULL, " ");
    }
    words[wordCount] = NULL;

    // Handle 'cd' command separately
    if (strcmp(words[0], "cd") == 0) {
      if (chdir(words[1]) == -1) {
        perror("cd");
      }
      // go to next iteration to prevent the execCommand(call). execvp and fork
      // are not needed.
      continue;
    }

    // call execCommand for fork and execvp to handle shell input / output
    execCommand(words);
  }
  return 0;
}