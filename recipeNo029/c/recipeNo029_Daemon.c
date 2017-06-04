#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "jni.h"
#include "recipeNo029_Daemon.h"

/* How to demonize your code propely, google a little bit.
   In this sample, we simply fork and don't care about anything.
   /tmp must be available to the process
*/
int demonize() {
  int pid = fork();
  switch ( pid ) {
    case -1:
      return (-1);
    case 0:
      break;
    default:
      return pid;
  }
  return 0;
}

JNIEXPORT int JNICALL Java_recipeNo029_Daemon_demonize
  (JNIEnv *env, jclass obj) {

  /* JNI call will demonize the code and get back to Java */ 
  int pid = demonize();

  /* Based on pid value, Java will either become daemon
     or will quit execution
     0  - means we are the daemon
     !0 - means we are oryginal code

     if we are daemon, we can make all the mainteanance here
  */
  if( pid == 0 ) {
    /* Change the file mode mask */
    umask(0);

    /* Create a new SID for the child process */
    int sid = setsid();
    if (sid < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }

    /* Change the current working directory */
    if ((chdir("/tmp")) < 0) {
      /* Log the failure */
      exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  }
  return pid;
 
}