/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */


#include "execute.h"

#include <stdio.h>

#include <pwd.h>

#include <fcntl.h>

#include <sys/wait.h>

#include "quash.h"

#include <deque.h>

#define MAX_PID_NUM 64
#define MAX_COMMAND_LENGTH (1024)
#define MAX_CAPACITY 32
#define WRITE 1
#define READ 0

int p[2][2];
int pipe_num = 0;
pid_t pid_list[MAX_PID_NUM];

struct passwd *pwd;

typedef struct Job {
  int j_id;
  pid_t p_list[MAX_PID_NUM];
  char* cmd;
} Job;

IMPLEMENT_DEQUE_STRUCT(JobDeq, Job);
PROTOTYPE_DEQUE(JobDeq, Job);
IMPLEMENT_DEQUE(JobDeq, Job);
JobDeq* bgJobs = NULL;
Job j_info;

// Remove this and all expansion calls to it
/**
 * @brief Note calls to any function that requires implementation
 */

#define IMPLEMENT_ME()                                                  \
printf("IMPLEMENT ME: %s(line %d): %s()\n", __FILE__, __LINE__, __FUNCTION__)

/***************************************************************************
 * Interface Functions
 ***************************************************************************/

// Return a string containing the current working directory.
char* get_current_directory(bool* should_free) {
  // TODO: Get the current working directory. This will fix the prompt path.
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();

  // Change this to true if necessary
  *should_free = true;
  //char pathname[MATH_RL_LEN]; //

  char* pathname = (char*) malloc(MAX_COMMAND_LENGTH * sizeof(char));
  getcwd(pathname, MAX_COMMAND_LENGTH);
  return pathname;
}

// Returns the value of an environment variable env_var
const char* lookup_env(const char* env_var) {
  // TODO: Lookup environment variables. This is required for parser to be able
  // to interpret variables from the command line and display the prompt
  // correctly
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();

  // TODO: Remove warning silencers
  //(void) env_var; // Silence unused variable warning

  return getenv(env_var);
}

// Sets the environment variable env_var to the value val
void write_env(const char* env_var, const char* val) {
  // TODO: Write environment variables
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();
  setenv(env_var,val,1);
  // TODO: Remove warning silencers
  //(void) env_var; // Silence unused variable warning
  (void) val;     // Silence unused variable warning
}


/*void removecmd(j_info)
{
  free(j_info.cmd);
}*/

// Check the status of background jobs
void check_jobs_bg_status() {
  // TODO: Check on the statuses of all of the background jobs
  //IMPLEMENT_ME();
  //in p_list, 0 means the end of the list, -1 means the process has been finished
  //printf("Check Background jobs\n");
  if(!is_empty_JobDeq(bgJobs)) {
  	int qLength = length_JobDeq(bgJobs);
		for(int i=0; i<qLength; i++){
			Job j_info = pop_front_JobDeq(bgJobs);
			bool completed = true;
			for(int j=0; j_info.p_list[j]!=-1; j++) {
				//printf("j_info.p_list[j]: %d\n", j_info.p_list[j]);
				int status;
				//printf("What status: %d\n", waitpid(j_info.p_list[j], status, WNOHANG));
				if(j_info.p_list[j]!=0 && waitpid(j_info.p_list[j], &status, WNOHANG)>0 && (WIFEXITED(status)||WIFSIGNALED(status))) {
						//printf("Never been here\n");
						j_info.p_list[j] = 0;

				}
				else {
					//printf("Change completed to false\n");
					completed = false;
				}
			}
			if(completed) {
				//printf("Really completed?\n");

				print_job_bg_complete(j_info.j_id, j_info.p_list[0], j_info.cmd);
        free(j_info.cmd);
			}
			else {
				push_back_JobDeq(bgJobs, j_info);
			}
		}
	  // TODO: Once jobs are implemented, uncomment and fill the following line
	}
}

// Prints the job id number, the process id of the first process belonging to
// the Job, and the command string associated with this job
void print_job(int job_id, pid_t pid, const char* cmd) {
  printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
  fflush(stdout);
}

// Prints a start up message for background processes
void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {
  printf("Background job started: ");
  print_job(job_id, pid, cmd);
}

// Prints a completion message followed by the print job
void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {
  printf("Completed: \t");
  print_job(job_id, pid, cmd);
}

/***************************************************************************
 * Functions to process commands
 ***************************************************************************/
// Run a program reachable by the path environment variable, relative path, or
// absolute path
void run_generic(GenericCommand cmd) {
  // Print an array of strings. The `str` array is a NULL terminated (last
  // character pointer is always NULL) list of strings. The first element in the
  // array is the executable
  char** str = cmd.args;
  //printf("cmd.str: %s\n", str);

  // TODO: Remove warning silencers
  //(void) str; // Silence unused variable warning

  // TODO: Implement run generic
  //IMPLEMENT_ME();

  execvp(str[0], str);
  //Check whether the input is a right command
  fprintf(stderr, "Error: The input is not a right command. \n");
  exit(1);
}

// Print strings
void run_echo(EchoCommand cmd) {
  // Print an array of strings. The args array is a NULL terminated (last
  // character pointer is always NULL) list of strings.
  char** str = cmd.args;

  // TODO: Remove warning silencers
  //(void) str; // Silence unused variable warning

  // TODO: Implement echo
  //IMPLEMENT_ME();
  for (; *str != NULL; ++str)
    printf("%s ", *str);
  printf("\n");
  fflush(stdout);
}

// Sets an environment variable
void run_export(ExportCommand cmd) {
  // Write an environment variable
  const char* env_var = cmd.env_var;
  const char* val = cmd.val;
  //printf("%s, %s\n", env_var, val);

  // TODO: Remove warning silencers
  //(void) env_var; // Silence unused variable warning
  //(void) val;     // Silence unused variable warning

  // TODO: Implement export.
  // HINT: This should be quite simple.
  //if (env_var == "HOME" || "PATH" || "PWD") {
  //  chdir(val);
  //}
  write_env(env_var, val);
  //IMPLEMENT_ME();
}

// Changes the current working directory
void run_cd(CDCommand cmd) {
  //Get the OLDPWD
  char* pathname = (char*) malloc(MAX_COMMAND_LENGTH * sizeof(char));
  getcwd(pathname, MAX_COMMAND_LENGTH);
  setenv("OLDPWD",pathname,1);
  // TODO: Change directory
  //IMPLEMENT_ME();
  chdir(cmd.dir);
  //TODO: Update PWD and optionally update OLD_PWD
  getcwd(pathname, MAX_COMMAND_LENGTH);
  setenv("PWD",pathname,1);
  free(pathname);
}

// Sends a signal to all processes contained in a job
void run_kill(KillCommand cmd) {
  int signal = cmd.sig;
  int job_id = cmd.job;

  // TODO: Remove warning silencers
  //(void) signal; // Silence unused variable warning
  //(void) job_id; // Silence unused variable warning

  // TODO: Kill a background job
  //IMPLEMENT_ME();
  int jobNum = length_JobDeq(bgJobs);
  for (int i=0; i<jobNum; i++){
    Job j_info = pop_front_JobDeq(bgJobs);
    if (j_info.j_id == job_id) {
      for (int s=0; j_info.p_list[s]!=-1; s++){
	if (j_info.p_list[s] != 0) {
	  fprintf(stderr, "%d\n", j_info.p_list[s]);
	  if (kill(j_info.p_list[s], signal) == -1) {
	    perror("Kill failed");
	  }
	}
      }
      //free(j_info.cmd);
      //printf("Killed all of them");
    }
    push_back_JobDeq(bgJobs, j_info);
  }
}


// Prints the current working directory to stdout
void run_pwd() {
  // TODO: Print the current working directory
  //IMPLEMENT_ME();
  char absDir[MAX_COMMAND_LENGTH];
  getcwd(absDir, sizeof(absDir));
  puts(absDir);
  // Flush the buffer before returning
  fflush(stdout);
}

// Prints all background jobs currently in the job list to stdout
void run_jobs() {
  // TODO: Print background jobs
  // IMPLEMENT_ME();
  int jobNum = length_JobDeq(bgJobs);
  for (int i=0; i<jobNum; i++){
    Job j_info = pop_front_JobDeq(bgJobs);
    print_job(j_info.j_id, j_info.p_list[0], j_info.cmd);   //Should I print out all the pid?
    push_back_JobDeq(bgJobs, j_info);
  }
}

/***************************************************************************
 * Functions for command resolution and process setup
 ***************************************************************************/

/**
 * @brief An example of how to resolve a Command type from a @a Command union.
 *
 * You will need some variant(s) of this switch structure to direct the command
 * statements to their correct caller. Probably you won't want all commands
 * accessible from a single function like this.
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void child_run_command(Command cmd) {
  CommandType type = get_command_type(cmd);
  switch (type) {
    case GENERIC:
      run_generic(cmd.generic);

    case ECHO:
      run_echo(cmd.echo);
      break;

    case PWD:
      run_pwd();
      break;

    case JOBS:
      run_jobs();
      break;

    default:
      fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

void parent_run_command(Command cmd) {
  CommandType type = get_command_type(cmd);
  switch (type) {
    case EXPORT:
      run_export(cmd.export);
      break;

    case CD:
      run_cd(cmd.cd);
      break;

    case KILL:
      run_kill(cmd.kill);
      break;

    case EXIT:
    case EOC:
      break;

    default:
      fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

/**
 * @brief Create a process centered around the @a Command in the @a
 * CommandHolder setting up redirects and pipes where needed
 *
 * @note Not all commands should be run in the child process. A few need to
 * change the quash process in some way
 *
 * @param holder The CommandHolder to try to run
 *
 * @sa Command CommandHolder
 */
void create_process(CommandHolder holder, int count) {
  // Read the flags field from the parser
  bool p_in  = holder.flags & PIPE_IN;
  bool p_out = holder.flags & PIPE_OUT;
  bool r_in  = holder.flags & REDIRECT_IN;
  bool r_out = holder.flags & REDIRECT_OUT;
  bool r_app = holder.flags & REDIRECT_APPEND;

  // TODO: Remove warning silencers
  (void) p_in;  // Silence unused variable warning
  (void) p_out; // Silence unused variable warning
  (void) r_in;  // Silence unused variable warning
  (void) r_out; // Silence unused variable warning
  (void) r_app; // Silence unused variable warning

  // TODO: Setup pipes and redirects
  //IMPLEMENT_ME();
  Command cmd = holder.cmd;
  CommandType type = get_command_type(cmd);

  //nothing in it
  if((p_in || p_out || r_in || r_out || r_app) == false) {
    if(type == CD || type == EXPORT || type == KILL) {
      parent_run_command(cmd);
    }
    else {
      pid_t pid = fork();
      if (pid == 0) {
        child_run_command(cmd);
	exit(0);
      }
      //exit(0);
      pid_list[count] = pid; //child id
    }
  }
  //at least one flag is true
  else{
    // If there is pipe out in command, it means we need new pipe
    if (p_out == true && p_in == false){
      pipe(p[pipe_num%2]);
    }
    if (p_out == true && p_in == true) {
      pipe(p[(pipe_num+1)%2]);
    }
    pid_t pid = fork();
    if (pid == 0){
      // "<"
      if (r_in == true) {
        int in_fd = open(holder.redirect_in, O_RDONLY, 0666);
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
      }
      // ">"
      if (r_out == true){
        int out_fd;
        if (r_app == true){
          out_fd = open(holder.redirect_out, O_APPEND|O_WRONLY, 0666);
        }
        else if(r_app == false){
          out_fd = open(holder.redirect_out, O_WRONLY|O_CREAT|O_TRUNC, 0666);
        }
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
      }
      // "pipe_in: | cmd3"
      if(p_in == true && p_out == false){
        dup2(p[pipe_num%2][READ], STDIN_FILENO);
        close(p[pipe_num%2][READ]);
      }
      // "| cmd2 |"
      if(p_in == true && p_out == true){
        dup2(p[pipe_num][READ], STDIN_FILENO);
        close(p[pipe_num][READ]);
        dup2(p[(pipe_num+1)%2][WRITE], STDOUT_FILENO);
        close(p[(pipe_num+1)%2][WRITE]);
      }
      // "pipe_out: cmd1 |"
      if (p_in == false && p_out == true) {
        dup2(p[pipe_num%2][WRITE], STDOUT_FILENO);
        close(p[pipe_num%2][WRITE]);
      }
      child_run_command(cmd);
	exit(0);
    }
    pid_list[count] = pid; //child id
    if (p_out == true && p_in == false){
      close(p[pipe_num%2][WRITE]);
    }
    if (p_in == true && p_out == true) {
      close(p[pipe_num%2][READ]);
      close(p[(pipe_num+1)%2][WRITE]);
      pipe_num++;
    }
    if (p_in == true && p_out == false){
      close(p[pipe_num%2][READ]);
    }
  }
}

void free_jobs(Job job) {
  free(job.cmd);
}

void deletebgjobs()
{
  apply_JobDeq(bgJobs, free_jobs);
  destroy_JobDeq(bgJobs);
  free(bgJobs);
}

void run_script(CommandHolder* holders) {
  if (holders == NULL)
    return;

	//If deque is not initialized
	if(!bgJobs){
    bgJobs = (JobDeq*) malloc(sizeof(JobDeq));
    *bgJobs = new_JobDeq(MAX_CAPACITY);
    empty_JobDeq(bgJobs);
    //destroy_JobDeq(bgJobs);
    atexit(deletebgjobs);
  }

  check_jobs_bg_status();

  if (get_command_holder_type(holders[0]) == EXIT &&
      get_command_holder_type(holders[1]) == EOC) {
    end_main_loop();
    return;
  }

  CommandType type;

  // Run all commands in the `holder` array
  int i;
  pipe_num = 0;
  int status;
  for (i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i){
    create_process(holders[i], i);
  }
  if (!(holders[0].flags & BACKGROUND)) {
    // Not a background Job
    // TODO: Wait for all processes under the job to complete
    //IMPLEMENT_ME();
    for (int j = 0; j < i; ++j){
      int status;
      if(get_command_holder_type(holders[0]) != KILL)
      {
        waitpid(pid_list[j], &status, 0);
      }
      else
      {
        //printf("Actually killlllll\n");
      }
    }
  }
  else {
    // A background job.j_info.j_id, j_info.p_list[0], j_info.cmd
    // TODO: Push the new job to the job queue
    //IMPLEMENT_ME();
    //Check whether bgJobs is initialized and it should be initialized just once
    //Decide the j_id
    if (is_empty_JobDeq(bgJobs)) {
      j_info.j_id = 1;
    }
    else{
      j_info.j_id = (peek_back_JobDeq(bgJobs).j_id+1);
      //printf("Second; jid : %d\n", peek_back_JobDeq(bgJobs).j_id);
      //printf("peek_back_JobDeq(bgJobs).p_list[0]: %d\n", peek_back_JobDeq(bgJobs).p_list[0]);
    }
    j_info.cmd = get_command_string();
    //free(get_command_string());

    for (int l=0; l < i; l++) {
      j_info.p_list[l] = pid_list[l];
      //printf("l: %d; state.pid : %d, j.pid : %d\n", l, pid_list[l], j_info.p_list[l]);
    }
    j_info.p_list[i] = -1;  //used for marking the length of job.

    push_back_JobDeq(bgJobs, j_info);

    // TODO: Once jobs are implemented, uncomment and fill the following line
    print_job_bg_start(j_info.j_id, j_info.p_list[0], j_info.cmd);
  }
  //destroy_JobDeq(bgJobs);
}
