#include <unistd.h>
#include <fcntl.h>
#include <csptr/smart_ptr.h>

struct log_file {
  int fd;
  // ...
};

void cleanup_log_file(void *ptr)
{
  close(((struct log_file *)ptr)->fd);
}

int main(void)
{
  struct log_file *log = unique_ptr(sizeof(struct log_file), cleanup_log_file);
  log->fd = open("/dev/null", O_WRONLY | O_APPEND);
  write(log->fd, "Hello", 5);
  
  // cleanup_log_file is called by sfree, then log is freed
  sfree(log);
  return 0;
}
