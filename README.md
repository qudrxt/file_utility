## File-Utility ##

This utlity program allows for the viewing of a file or copying of a file to a directory employing low-level operations - *system calls*, with the C programming langauge.

This project was initially developed without the usage of version control.

## Context ##

If no source path has been provided, utility will read the contents of *sample.txt* from the directory that the program resides in. 

If a source path has been provided, it must be the **first** argument when executing the program and provided as an **absolute path**. 

On default, the utility reads the **first 10 lines** - entire file if it has less than 10 lines, and writes the content of these lines to standard output.

The number of lines read can be modified by employing the *-n* argument followed by the number of lines desired, will read the desired number of lines from the **start** of the file.

The position of reading can be changed by employing the *-L* argument which modifies to reading to read the **last** m number of lines. 

Instead of writing to standard output, the utility is able to copy the content it has read and write it to a file. This functionality can be executed by employing the *-d* argument followed by the absolute path of the directory where this new file should now exist. 

## Usage ##

* Clone this repository
* Compile the program - `fileutil.c`
* Execute the compiled program by employing zero or more options 

## Options ##

* *sourcefile* - allows the user to specify a path of the file to be displayed or copied and must be directly specified after the invocation of the program 

* *-n* (--numlines) - allows the user to specify the m number of lines that should be displayed or copied

* *-d* (--destdir) - allows the user to input a path for the destination directory for copying the source file. It must be an absolute path that begins and ends with a '/' character

* *-L* - switches the utility to 'tail' mode where the display or copying of the file is performed on the last 10 or m number of lines

## Execution ##

* The successful execution of the program requires the following **exact ordering** in its invocation - `./(executable program name) [sourcefile] [-n numlines] [-d destdir] [-L]`

## Status codes ##

* *0* - successful program execution
* *1* - opening file existance exception
* *2* - directory path or creating file existence exception