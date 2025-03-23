#include <iostream>
#include <pwd.h>
#include <ucontext.h>
#include <unistd.h>
#include <stdio.h>
#include <csignal>
#include <cstring>

void signal_handler(int signum, siginfo_t *info, void *context) {
	pid_t sender_pid = info->si_pid;
	uid_t sender_uid = info->si_uid;
	struct passwd *pwd = getpwuid(sender_uid);
	char *username = pwd->pw_name;
	ucontext_t *uc = (ucontext_t *)context;
	uintptr_t rip = (uintptr_t)uc->uc_mcontext.gregs[REG_RIP];
    	uintptr_t rax = (uintptr_t)uc->uc_mcontext.gregs[REG_RAX];
    	uintptr_t rbx = (uintptr_t)uc->uc_mcontext.gregs[REG_RBX];
	std::cout << "received a signal SIGUSR1 from process " << sender_pid << " by " << sender_uid << " (" << username << ")." << std::endl;
	std::cout << "state of: RIP = " << rip << ", RAX = " << rax << ", RBX = " << rbx << std::endl;
}

int main() {
	pid_t pid = getpid();
	std::cout << "pid of process " << pid << std::endl;
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, nullptr);
	while(true) sleep(10);
}

