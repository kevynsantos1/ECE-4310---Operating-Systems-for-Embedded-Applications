This is a basic custom shell that utilizes tokenizing inputs to be able to use execvp. Execvp is important because it basically imports our shell commands into the custom shell. I also utilized forks so that when a command is called it is
forked off and then returns to the shell once the command is ran/done.
