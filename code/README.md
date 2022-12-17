# _Assignment-3 Final Submssion_

### _Operating Systems and Networks_

---

Name : Mitansh Kayathwal

Roll No : 2021101026

OS : Linux

---

### _Executing the Shell_

---

Compile the Code and execute it using the following in the extracted directory _2021101026_ :-

$ make

$ ./shell

---

### _Specification-1_

---

- Handled in file commands.c itself(commands.c executes all built-in as well as foreground and background processes as well as some user defined commands such as fg and bg)

- This Specification is Executed in the Following Files:

- commands.c
- commands.h

---

### _Specification-2_

---

- Errors for piping have NOT been handled.

- A count for number of pipes is maintained in main.c. If pipes exist then we call the function piper() and execute the pipe.

- Piping cannot be run in background in this shell as it has not been handled.

- These Commands are Executed based on the Code Written in the Following Files:

- piping.c
- piping.h

---

### _Specification-3_

---

- No new files have been created for this specification. This specification is handled automatically in commands.c file.

- No extra code has been written for this specification

- This Specification is Executed in the Following Files:

- commands.c
- commands.h

---

### _Specification-4_

---

- Error handling has been done properly.

- Error has also been handled for invalid input type.

- This Specification is Executed in the Following Files:

- jobs.c
- jobs.h
- fg.c
- fg.h
- bg.c
- bg.h
- sig.c
- sig.h

---

### _Specification-5_

---

- Ctrl-D has been handled in the raw-mode code provided by the TAs

- Ctrl-C has been handled using SIG_DFL in the file foreground.c

- Ctrl-Z has been handled in foreground.c

- This Specification is Executed in the Following Files.

- foreground.c
- foreground.h
- main.c

---

### _Specification-6_

---

- Tab autocompletion has been done properly.

- Input files given as Absolute Path or Relative Path(wrt current directory or ~) have been handled:

- ~/file_path
- /absolute_path
- relative_path

- While doing autocomplete, I have not taken care if given file is a directory or not.

- Assumed that the last token(i.e. last command token after space) is the incomplete path given. No other I/O format has been handled for this specification.

- This Specification is Executed in the Following Files.

- tab_handler.c
- tab_handler.h
