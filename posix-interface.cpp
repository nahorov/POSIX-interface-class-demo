#ifndef __POSIX_PROCESS_H
#define __POSIX_PROCESS_H

// #include "posix_process.h"
#include<spawn.h> // define posix_spawnattr_t and posix_spawn_file_actions_t
#include<errno.h> // system error numbers
#include<iostream>
#include<string>
#include<sys/wait.h> // to define the status of the child process
		     
// interface class
class posix_process
{
	protected:
		pid_t Pid;
		posix_spawnattr_t SpawnAttr;
		posix_spawn_file_actions_t FileActions;
		char **argv;
		char **envp;
		std::string ProgramPath;

	public:
		posix_process(std::string Path, char **av, char **env);
		posix_process(std::string Path, char **av, char **env, posix_spawnattr_t X, posix_spawn_file_actions_t Y);
		int run(void);
		void pwait(int &X);
};

// definitions

posix_process :: posix_process(std::string Path, char **av, char **env)
{
	argv = av;
	envp = env;
	ProgramPath = Path;
	posix_spawnattr_init(&SpawnAttr);
	posix_spawn_file_actions_init(&FileActions);
}

posix_process::posix_process(std::string Path, char **av, char **env, posix_spawnattr_t X, posix_spawn_file_actions_t Y)
{
	argv = av;
	envp = env;
	ProgramPath = Path;
	SpawnAttr = X;
	FileActions = Y;
	posix_spawnattr_init(&SpawnAttr);
	posix_spawn_file_actions_init(&FileActions);
}

int posix_process::run(void)
{
	int val=posix_spawn(&Pid, ProgramPath.c_str(), &FileActions, &SpawnAttr, argv, envp);
	std::cout<<"\n Process   "<<val<< " running";
	return val;
}

void posix_process::pwait(int &X)
{
	wait(&X);
	std::cout<<"\n Process   " << X << " waiting";
}

int main(int argc, char *argv[], char *envp[])
{
	int Value;
	posix_process Child1("pwd", argv, envp);
	posix_process Child2("ls", argv, envp);
	std::cout<<"\n Trying to run Child Process 1";
	int Val1=Child1.run();
	std::cout<<"\n Trying to run Child Process 2";
	int Val2=Child2.run();
	Child1.pwait(Val1);
	Child2.pwait(Val2);
	return 0;
}

#endif
