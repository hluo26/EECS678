#include "deque.h"
#include "limits.h"
#include <string.h>

#define MAX_CAPACITY 32

typedef struct job_info {

  int job_id;
  pid_t pid[2];
  char* cmd;

} job_info;

IMPLEMENT_DEQUE_STRUCT(job_deque, job_info);
PROTOTYPE_DEQUE(job_deque, job_info);
IMPLEMENT_DEQUE(job_deque, job_info);

job_deque* background_jobs = NULL;

job_deque* get_background_jobs_deque() // to maintain only one instance of deque at a time
{
  if(!background_jobs) //if background_jobs is unitialized
  {
    background_jobs = (job_deque*) malloc(sizeof(job_deque));
    *background_jobs = new_job_deque(MAX_CAPACITY);
  }

  return background_jobs;
}

void add_job_to_deque(job_info info)
{
  push_back_job_deque(get_background_jobs_deque(), info);
}

void print_one_job(job_info info)
{

      printf("[%i]\t%i\t%s\n", info.job_id, info.pid[0], info.cmd);

}

void print_jobs()
{
  apply_job_deque(get_background_jobs_deque(), (void*)print_one_job);
}

int check_no_jobs()
{
  return is_empty_job_deque(get_background_jobs_deque()) ? 1 : 0;
}

int get_job_id()
{
  if(check_no_jobs())
  {
    return 0;
  }
  else
  {
    return peek_back_job_deque(get_background_jobs_deque()).job_id + 1;
  }
}

job_info get_back_job()
{
  return pop_back_job_deque(get_background_jobs_deque());
}

job_info get_front_job()
{
  return pop_front_job_deque(get_background_jobs_deque());
}

int get_length()
{
  if(check_no_jobs())
  {
    return 0;
  }
  else
  {
    return length_job_deque(get_background_jobs_deque());
  }
}
