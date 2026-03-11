# Mini Unix Shell

This project implements a simplified Unix shell in C using an incremental development approach.

Instead of implementing the entire shell at once, the system is built through twelve small programs, where each stage introduces one fundamental concept of Unix systems programming such as process creation, command execution, pipes, and file descriptor redirection.

---

## Features

- Command execution using `fork()` and `exec`
- Command parsing with arguments
- Pipes between processes
- Input and output redirection
- Basic interactive shell loop

---

## Project Structure

```
mini-unix-shell
│
├── src
│   ├── 1gets.c
│   ├── 2execlp.c
│   ├── 3strtok.c
│   ├── 4strtok.c
│   ├── 5dup.c
│   ├── 6pipe.c
│   ├── 7pipe.c
│   ├── 8pipe.c
│   ├── 9pipe.c
│   ├── 10pipe.c
│   ├── 11pipe.c
│   └── 12shell.c
│
├── report
│   └── myshell.pdf
│
├── README.md
├── LICENSE
└── .gitignore
```

---

## Development Stages

| Program | Concept |
|-------|--------|
| 1gets.c | Basic shell loop |
| 2execlp.c | Command execution |
| 3strtok.c / 4strtok.c | Command parsing |
| 5dup.c | File descriptor redirection |
| 6pipe.c | Basic pipes |
| 7pipe.c | Two-process pipeline |
| 8pipe.c | User pipe commands |
| 9pipe.c / 10pipe.c | Flexible command execution |
| 11pipe.c | Output redirection |
| 12shell.c | Final shell implementation |

---

## Example Commands

```
who | wc -l
ls -l > output.txt
ls -l | sort > sorted_output.txt
sort < input.txt > sorted.txt
```

---

## Build

Compile the final shell:

```
gcc src/12shell.c -o myshell
```

Run:

```
./myshell
```

---

## Project Report

A detailed explanation of the design and implementation is available in:

```
report/myshell.pdf
```

---

## Author

Nabin Kumar Singh  
MS Electrical Engineering  
University of Alabama in Huntsville
