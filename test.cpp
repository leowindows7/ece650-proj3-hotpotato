#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
 
#include <iostream>

#define LOGGING "Start Logging my task = %d\n"
static void skeleton_daemon() {
  pid_t pid;
  std::cout << (int)pid << std::endl;
  /* Fork off the parent process */
  pid = fork();
  std::cout << (int)pid << std::endl;
  /* An error occurred */
  if (pid < 0)
    exit(EXIT_FAILURE);

  /* Success: Let the parent terminate */
  if (pid > 0)
    exit(EXIT_SUCCESS);

  /* On success: The child process becomes session leader */
  if (setsid() < 0)
    exit(EXIT_FAILURE);

  /* Catch, ignore and handle signals */
  //TODO: Implement a working signal handler */                               
  signal(SIGCHLD, SIG_IGN);
  signal(SIGHUP, SIG_IGN);

  /* Fork off for the second time*/
  //pid = fork();
  std::cout << (int)pid << std::endl;
  /* An error occurred */
  if (pid < 0)
    exit(EXIT_FAILURE);

/* Set new file permissions */
  umask(0);

  /* Change the working directory to the root directory */
  /* or another appropriated directory */
  chdir("/");

  /* Close all open file descriptors */
  int x;
  for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
    close(x);
  }

  /* Open the log file */
  openlog("firstdaemon", LOG_PID, LOG_DAEMON);
}

int main() {
  skeleton_daemon();
  int count = 0;
  openlog("Logs", LOG_PID, LOG_USER);
while (1) {
    //TODO: Insert daemon code here.                                          
    sleep(2);
    syslog(LOG_INFO, LOGGING, count++);
  }
  closelog();
  return EXIT_SUCCESS;
}