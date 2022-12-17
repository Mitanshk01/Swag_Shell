# Swag_Shell
Custom Shell made in C programming language

### _Executing the Shell_

---

Compile the Code and execute it using the following in the extracted directory _2021101026_ :-

$ make

$ ./shell

### _Specification-1_

---

- Printing Shell Prompt in every Iteration of the Shell int the format mentioned in the Assignment pdf.

- This Specification is Executed in the Following Files.

- prompt.c
- prompt.h

- The Current Directory is represented as _"~"_ when the Shell is in the Directory in which it was executed. This represents the Home Directory.

### _Specification-2_

---

- The Following Command are Executed in this Specification.

  cd

  echo

  pwd

- These Commands are Executed based on the Code Written in the Following Files.

- cd.c
- cd.h
- pwd.c
- pwd.h
- echo.c
- echo.h

#### _echo_

- echo Command Prints a Message on the Terminal.

- Brackets of any sort have not been handled and this command prints the string as it is given as input.

#### _cd_

- cd changes the Present Working Directory of the Shell.
- It can be Executed using the Following Command.
  bash
  cd <target_directory>

- We can give both Absolute and Relative Path as the Target Directory

- The Following Flags can also be executed in the cd Command
  bash
  cd .
  cd ..
  cd -
  cd ~

#### _pwd_

- This Command is Used to Print the Absolute Path of the Current Working Directory of the Shell.
- The Following Command can be used to Execute pwd.

  pwd

### _Specification-3_

---

#### _ls_

- List all the Directories and Files of the Given or the Current Working Directory.
- The Code for this Command is written in the Following Files:

- ls.c
- ls.h

- The Following Flags and Methods can be Used to Execute the ls Command.
  bash
  ls <directory>
  ls -a/-l/-al/-la
  ls -a/-l/-al/-la <directory>
  ls <directory_path>
  ls <filename>

- ls in this shell handles at most 1000 files in a given directory.

- Color Codiing is done in the Output of this Command.

Blue --> For Directories
Green --> For Executables
White --> All other files

### _Specification-4_

---

- This Specification Executes the System Commands as both Foreground and Background Processes.

- Incase of Multiple Foreground Processes the time taken by the last foreground process is printed.

- The command above executes multiple background processes with seperate PIDs for each one of them.

- At most 1000 background processes can be handled at once.

- The code for this specification can be found in the Following Files.

  foreground.c
  foreground.h
  background.c
  background.h

### _Specification-5_

---

- When PID is provided, the command outputs the data for the process with the given PID.

- When the PID is not provided, the command gives data regarding the process running the shell.

- The Code for this Command can be found in the Following Files.

  pinfo.c
  pinfo.h

### _Specification-6_

---

- Prints the Exit Message after the Background Process Ends.

  background.c
  background.h

- There is a formatting issue when the background process exits and prints its message and we have to press enter to continue in the shell.

### _Specification-7_

---

- The discover Command searches for files in a Directory Hierarchy.
- Depth First Search traversal is Used for searching in the Directory Hierarchy.
- The Discover Command does not Search inside Hidden Files but will search for hidden files listed in the main directory.
- The Following Commands can be executed to run the Discover Command.
  discover
  discover <target_dir> <type_flags> <file_name>
  discover <target_dir> -d/-f/-d -f
  discover -d/-f/-d -f/-f -d <file_name>
  discover <file_name>
  discover <target_dir>

- The Code for the Specification can be found in the Following Files.

  discover.c
  discover.h

### _Specification-8_

---

- The history Command prints the last ten Commands given by the User to the Shell.
- The Commands are stored across sessions of the Shell in a text file history.txt.
- While the shell is running a deque data structure named history takes care of maintaing the history dynamically.

- The file history.txt already exists in the shell directory.

- If the shell exits with a segmentation fault then the history is not updated for that session.

- The history is only updated in the text file when the user wants to exit the current shell session.

- Error is thrown if more than 1 input is given.

- The Code for this Specification can be Found in the Following Files.

  history.c
  history.h

### _Exiting the Shell_

---

- The Shell can be Terminated by typing `EXIT` on the shell command-line.

---

### _Specification-9_

---

- Handled in file commands.c itself(commands.c executes all built-in as well as foreground and background processes as well as some user defined commands such as fg and bg)

- This Specification is Executed in the Following Files:

- commands.c
- commands.h

---

### _Specification-10_

---

- Errors for piping have NOT been handled.

- A count for number of pipes is maintained in main.c. If pipes exist then we call the function piper() and execute the pipe.

- Piping cannot be run in background in this shell as it has not been handled.

- These Commands are Executed based on the Code Written in the Following Files:

- piping.c
- piping.h

---

### _Specification-11_

---

- No new files have been created for this specification. This specification is handled automatically in commands.c file.

- No extra code has been written for this specification

- This Specification is Executed in the Following Files:

- commands.c
- commands.h

---

### _Specification-12_

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

### _Specification-13_

---

- Ctrl-D has been handled in the raw-mode code provided by the TAs

- Ctrl-C has been handled using SIG_DFL in the file foreground.c

- Ctrl-Z has been handled in foreground.c

- This Specification is Executed in the Following Files.

- foreground.c
- foreground.h
- main.c

---

### _Specification-14_

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

