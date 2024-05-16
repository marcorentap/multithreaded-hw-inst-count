#include <err.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sched.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> basicPaths = {
    "./dobasic100000",
    "./dobasic10000",
    "./dobasic1000",
};

std::vector<std::string> kokkosPaths = {
    "./dokokkos100000",
    "./dokokkos10000",
    "./dokokkos1000",
};

std::vector<std::string> openmpPaths = {
    "./doopenmp100000",
    "./doopenmp10000",
    "./doopenmp1000",
};

const std::string sharefilepath = "file.txt";

void SetAffinity(int numThreads) {
  sched_param param;
  int policy = SCHED_FIFO;
  int pid = 0;
  cpu_set_t cpuSet;

  // Set child's scheduler to SCHED_FIFO with max priority
  if (sched_getparam(pid, &param) < 0) {
    err(EXIT_FAILURE, "Cannot get child scheduling parameters: ");
  }
  param.sched_priority = sched_get_priority_max(policy);
  if (sched_setscheduler(pid, policy, &param) < 0) {
    err(EXIT_FAILURE, "Cannot set child scheduler: ");
  }

  // Limit child to CPUs 0 to numThreads-1
  CPU_ZERO(&cpuSet);
  for (int i = 0; i < numThreads; i++) {
    CPU_SET(i, &cpuSet);
  }
  if (sched_setaffinity(pid, sizeof cpuSet, &cpuSet) < 0) {
    err(EXIT_FAILURE, "Cannot set child affinity: ");
  }
}

int ExecWithRet(const char *path, const char *arg, const char *env, int numThreads) {
  int fd, pid, wstatus;
  if (fd < 0) {
    errx(EXIT_FAILURE, "Cannot open file %s", sharefilepath.c_str());
  }

  pid = fork();
  if (pid == 0) {
    fd = open(sharefilepath.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0664);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    SetAffinity(numThreads);
    execl(path, arg, NULL);
  } else {
    pid = wait(&wstatus);
    if (pid < 0) {
      errx(EXIT_FAILURE, "Cannot wait child");
    }
  }

  // Read return value as int
  char buf[128];
  std::memset(buf, 0, sizeof(buf));
  fsync(fd);
  fd = open(sharefilepath.c_str(), O_RDONLY);
  if (read(fd, &buf, sizeof(buf)) < 0) {
    errx(EXIT_FAILURE, "Cannot read shared file.");
  }
  int val = std::stoi(buf);
  close(fd);
  return val;
}

int main(int argc, char *args[]) {
    long nproc = sysconf(_SC_NPROCESSORS_ONLN);
  for (int i = 0; i < basicPaths.size(); i++) {
    auto path = basicPaths.at(i);
    printf("exec path: %s %s %s\n", "", path.c_str(), "");
    printf("return: %d\n", ExecWithRet(path.c_str(), NULL, NULL, nproc));
  }
  for (int i = 0; i < kokkosPaths.size(); i++) {
    auto path = kokkosPaths.at(i);

    for (int n = 1; n <= nproc; n++) {
      char arg[128];
      std::sprintf(arg, "--kokkos-num-threads=%d", n);
      printf("exec path: %s %s %s\n", "", path.c_str(), arg);
      printf("return: %d\n", ExecWithRet(path.c_str(), arg, NULL, n));
    }
  }
  for (int i = 0; i < kokkosPaths.size(); i++) {
    auto path = openmpPaths.at(i);

    for (int n = 1; n <= nproc; n++) {
      char env[128];
      std::sprintf(env, "OMP_NUM_THREADS=%d", n);

      printf("exec path: %s %s %s\n", env, path.c_str(), "");
      printf("return: %d\n", ExecWithRet(path.c_str(), NULL, env, n));
    }
  }
  return 0;
}
