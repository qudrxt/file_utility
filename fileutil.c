#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

/*
	A file utility that provides the simplified features of the Linux utilities:
	head, tail, cat and cp.
	
	:author: Qudrat Razi
*/

int getNumberOfLines(int fd)
/*
	Determines the number of lines that a file denoted by fd - file descriptor, has.
	
	:param fd: the file descriptor of the file
	:return: number of lines that a file has as an integer
*/

{
	int lineCount = 0;
	char *byteBuf = (char *) malloc(sizeof(char));

	// Count the file's number of lines

	while (read(fd, byteBuf, 1) == 1)
	{
		if (*(byteBuf) == '\n')
		{
			lineCount++;
		}
	}

	free(byteBuf);
	return lineCount;
}

int *getNumberOfCharsPerLine(int fd, int lineCount)
/* 
	Determines the number of characters per line of a file.
	
	:param fd: the file descriptor of the file
	:param lineCount: the number of lines that the file has
	:return: an integer pointer that stores the number of characters of each line
*/

{
	char *byteBuf = (char *) malloc(sizeof(char));
	int foundNewLine, charCount, i, *lineAr = (int *) malloc(sizeof(int) * lineCount);

	for (i = 0; i < lineCount; i++)
	{
		charCount = 0;
		foundNewLine = 0;

		// Count the number of characters per line

		while (!foundNewLine && read(fd, byteBuf, 1) == 1)
		{
			charCount++;

			if (*(byteBuf) == '\n')
			{
				// If a new line is encountered - move to next line

				*(lineAr + i) = charCount;
				foundNewLine = 1;
			}
		}
	}

	free(byteBuf);
	return lineAr;
}

void offsetCursor(int fd, int *lineChars, int lineDiff)
/* 
	Offsets the file cursor of a file by the number of characters on a line and the number of lines required.
	
	:param fd: the file descriptor of the file
	:param lineChars: a pointer that stores the number of characters on each line of a file
	:param lineDiff: the number of lines that the cursor needs to be offset by
*/

{
	int i;

	for (i = 0; i < lineDiff; i++)
	{
		// Offset by number of characters on line(i)
	
		lseek(fd, *(lineChars + i), SEEK_CUR);
	}
}

int findArgument(int argCount, char *argVector[], char *targetArg)
/* 
	The linear search algorithm that determines if a target exists in a collection.
	
	:param argCount: the length of argVector
	:param argVector: the collection of arguments of a fileutil execution
	:param targetArg: the target argument searched for through argVector
	:return: the position of targetArg in argVector or 0 to denote its absence  
*/

{
	int i = 0, argPos = 0;

	while (i < argCount && !argPos)
	{
		if (strcmp(argVector[i], targetArg) == 0)
		{
			// 0 evaluates to false in C
		
			argPos = i;
		}

		i++;
	}

	return argPos;
}

int main(int argc, char *argv[])
/* 
	The orchestrating function for fileutil.c.
	
	:param argc: The number of arguments passed to fileutil on execution
	:param argv: The arguments passed to fileutil on execution
	:return: an integer that represents the success or failure of the program
*/

{
	char *openError = "error: sample.txt does not exist in the working directory\n";
	char *inpArgs[] = {"-n", "-L", "-d"}, *lineBuf, *filePath, *createdPath;
	int wd = 1, exitValue = 0, pathSpec = 0, fd, *lineAr, numberOfLines, lineCount, i, charCount, lineDiff;

	// Determine if a file path has been specified

	i = 0;

	while (i < 3 && !pathSpec)
	{
		// If file path has been specified - must be at index 1

		if (findArgument(argc, argv, inpArgs[i]) == 1)
		{
			// No file path has been specified

			pathSpec = 1;
		}

		i++;
	}

	filePath = pathSpec ? "sample.txt" : argc > 1 ? argv[1] : "sample.txt";

	// Open the file path according to the ternary

	fd = open(filePath, O_RDONLY);

	if (fd < 0)
	{
		// 1 denotes failure of opening source file
	
		exitValue = 1;
		write(2, openError, strlen(openError));
	}
	
	else
	{
		char *copyOutput = "copy successful\n"; 
		char *sourceError = "error: destination directory already contains specified file or does not exist\n";
		
		// Get the number of lines of the input file

		numberOfLines = getNumberOfLines(fd);

		lseek(fd, 0, SEEK_SET);

		// Determine if -n argument has been set

		if (findArgument(argc, argv, "-n"))
		{
			lineCount = strtol(argv[findArgument(argc, argv, "-n") + 1], NULL, 10);
		}

		else
		{
			lineCount = numberOfLines > 10 ? 10 : numberOfLines;
		}

		// Determine if -L argument has been set

		lineDiff = findArgument(argc, argv, "-L") ? numberOfLines - lineCount : 0;

		lineAr = getNumberOfCharsPerLine(fd, numberOfLines);

		lseek(fd, 0, SEEK_SET);

		// Offset by the number of line(s) being skipped if any

		offsetCursor(fd, lineAr, lineDiff);

		// Determine if -d argument has been set

		if (findArgument(argc, argv, "-d"))
		{
			createdPath = strcat(argv[findArgument(argc, argv, "-d") + 1], basename(filePath));
			wd = open(createdPath, O_WRONLY | O_CREAT | O_EXCL, S_IRWXU);
		}
		
		if (wd < 0)
		{
			// 2 denotes failure of opening destination file
		
			exitValue = 2;
			write(2, sourceError, strlen(sourceError));
		}
		
		else
		{
			// Reading and writing loop

			for (i = lineDiff; i < lineCount + lineDiff; i++)
			{
				charCount = *(lineAr + i);

				// Allocate memory into lineBuf

				lineBuf = (char *) malloc(sizeof(char) * charCount);

				// Read and write a line of content into lineBuf

				read(fd, lineBuf, charCount);
				
				write(wd, lineBuf, charCount);

				// Release allocated memory for next iteration

				free(lineBuf);
			}
			
			// Determine if copying occurred
			
			if (wd != 1)
			{
				write(1, copyOutput, strlen(copyOutput)); 
			}
		}
		
		free(lineAr);
		close(wd);
	}

	// All resources are freed regardless of program failure 

	close(fd);
	return exitValue;
}
