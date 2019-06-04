// SÜHA TANRIVERDİ - 201611689
// CENG-328 Operating Systems
// Assignment 1 - Multiprocessing
/*

================== FORK ==================
In this assignment, we will have two child processes,
that's why we need to use fork() system call two times.
I am going to use following multiprocess structure by using fork() system call;

pid_t child_a, child_b; ---> pid_t variables
child_a = fork(); ---> First Fork()
if (child_a == 0) { 
     ---> CHILD A Code 
} else {
    child_b = fork(); ---> Second Fork() 
    if (child_b == 0) {
        ---> CHILD B Code 
    } else {
    	---> PARENT Code 
    }
}

================== PIPE ==================
I am also going to use Pipe() system call to get communicated two children processes 
Note that it is because we have two child processes, we'll need to have two pipes! 
Sample Pipe Usage;

	int pfd[2]; ---> Create fd means File Descripter
	pipe(pfd); ---> convert variable to "pipe", it must be done before fork()
	pfd[0]; ---> for using READ end
	pfd[1]; ---> for using WRITE end

*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// #include <fcntl.h> 
#include <sys/stat.h> 
#define DATA_SIZE 10000 // To wait parent process for children 
#define BUFFER_SIZE 2048 // When Reading Char from File, Buffer Size

// Common Print Function to list all letter counts
void listCount(int arr[]) {
	int i;
	for (i = 0; i < 26; i++) {
		printf("%c: %d\n", ('a' + i), arr[i]);
	}
}

// Calculates Total Frequency from Child Processes
int calculateTotalFrequency(int arr[]) {
	int sum = 0, i;
	for(i = 0; i < 26; i++) {
		sum += arr[i];
	}
	printf("Total Frequency Sum: %d\n", sum);
	return sum;
}

// Merges Total Frequency Tables into One Table from Both Child Processes
void mergeTables(int arr[], int arr2[]) {
	int i, totalFreq[26];
	memset(&totalFreq, 0, sizeof(totalFreq)); // We fill our array with zeros
	for(i = 0; i < 26; i++) {
		totalFreq[i] += arr[i] + arr2[i];
	}
	listCount(totalFreq);
	return totalFreq;
}

int main(void) {
	// We need two pipes because we have two child processes, we create them
	// Creates File Pointer to open text.txt file, To let children processes access this pointer!
    int pfd1[2];  // First Pipe 
    int pfd2[2];  // Second Pipe
    int pdfFreq1[2];  // First Pipe for Code
    int pdfFreq2[2];  // Second Pipe for Code
    // pfd1[0] ---> READ End of FIRST Pipe
    // pfd1[1] ---> WRITE End of FIRST Pipe
    // pfd2[0] ---> READ End of SECOND Pipe
    // pfd2[1] ---> WRITE End of SECOND Pipe
    pipe(pfd1);
    pipe(pfd2);
    pipe(pdfFreq1); // This pipe will be used in the side of Child_1 to send calculated freqs
    pipe(pdfFreq2); // This pipe will be used in the side of Child_2 to send calculated freqs
	pid_t child_1, child_2;
	child_1 = fork(); // First Fork
	child_2 = fork(); // Second Fork
	if(child_1 > 0 && child_2 > 0) { // PARENT PROCESS
		int status;
    	// We Test This Process If it is really Parent by using getpid();
    	// getpid() -> returns the pid of parent process
        printf("->	Parent Process, pid = %d, parent pid = %d\n", getpid());
        // We generate Text File
		char text[DATA_SIZE] = "CENG328OperatingSystems8.04.2019Assignment1DueDate:22.04.2019Subject:MultiprocessingProblemStatement:Allowthemainprocesstogenerateatextfilecontainingthetextofthisassignment.Themainprocesswillthencreatetwootherprocessesandpasstothemthenameofthefileandtwocodes(code1andcode2)usingapipe()systemcall.Thecodesaretwodifferentalphabeticletterssuchas“a”and“k”.Thechildprocessesshouldsearchthefileforthecharacterintheintervalreceived,computetheirfrequenciesandreturnthemthroughaseparatepipetotheparentprocess.Theparentprocessshouldcomputethetotalnumberofcharactersinthefile,andtherateoftheappearanceofthecharacterfrequenciesreceivedfromthetwochildprocess,throughseparatepipes.Theparentprocessshouldthenformatableofalphabeticcharactersandtheirfrequenciesandprintthetableinaproperformatonthescreen.Hint:Youareexpectedtousefork(),pipe(),read(),write(),close(),waitpid(),andotherappropriatesystemcall,ifneeded.Importantnotes:1.Yourprogramsshouldbewelldocumented.Eachprogrammustincludethesummaryoftheworkitwilldoattheverybeginning.Youmustalsoproperlycommentalltheprogrammingblocks.2.Anoverallsimilarityover50percentbetweendifferentsubmissionswillbeconsideredas“cheating”.3.Submissiondetails:Submitparentprocessprogramandchildprocessprograms,andscreenshotfortheprogramexecutionshellcommandsandtheoutput,intheformof<”yourid”-ParentProg>,<”yourid”-ChildProg>,<”yourid”-screenShots>4.PossibleGrading:*Documentation:20%Programforparentprocess:30%Programforchildprocesses:30%Output:20%*canchange";
		char ch;	
		int count[26], i = 0, total = 0; // English Alphabet
		memset(&count, 0, sizeof(count)); // We fill our array with zeros
		FILE *fout = fopen("text.txt", "w");
	    if(fout != NULL) {
	        fputs(text, fout);
	        fclose(fout);
	    }
		while(text[i] != '\0') {
			ch = text[i];
			if(isalpha(ch)) {
				count[tolower(ch) - 'a']++;
				total++;
			}
			i++;
		}
		// List Them for testing
		// for (i = 0; i < 26; i++) {
		// 	printf("%c: %d\n", ('a' + i), count[i]);
		// }
		// We close read end of both pipes because parent will not use read-ends!
		close(pfd1[0]); // Close Read-End Pipe_1
		close(pfd2[0]); // Close Read-End Pipe_2
		close(pdfFreq1[1]); // Close Write-End PipeFreq_1
		close(pdfFreq2[1]); // Close Write-End PipeFreq_2

		char c1[11] = "text.txt,ad";
		char c2[11] = "text.txt,kz";
		write(pfd1[1], c1, 12);
		write(pfd2[1], c2, 12);

		waitpid(child_1, &status, 0);
		waitpid(child_2, &status, 0);


		// =============== FREQUENCY RECEIVED FROM CHILD 1 ===============
		int countChild_1[26], totalCh1 = 0; // Child 1's Frequency
		memset(&countChild_1, 0, sizeof(countChild_1)); // We fill our array with zeros
		read(pdfFreq1[0], countChild_1, sizeof(countChild_1));
		//printf("Received From Child_1: \n");
		//listCount(countChild_1); // Parent test what it is received from child 1


		// =============== FREQUENCY RECEIVED FROM CHILD 1 ===============
		int countChild_2[26], totalCh2 = 0; // Child 2's Frequency
		memset(&countChild_2, 0, sizeof(countChild_2)); // We fill our array with zeros
		read(pdfFreq2[0], countChild_2, sizeof(countChild_2));
		//printf("Received From Child_2: \n");
		//listCount(countChild_2); // Parent test what it is received from child 2

		printf("Child_1 interval from '%c' to '%c' ", c1[9], c1[10]);
		totalCh1 = calculateTotalFrequency(countChild_1);
		printf("Child_2 interval from '%c' to '%c' ", c2[9], c2[10]);
		totalCh2 = calculateTotalFrequency(countChild_2);
		printf("Total Exactly Number of Letters in the Text file: %d\n", total);
		printf("Total Number of Letters Received From Two Child Process: %d\n", (totalCh1 + totalCh2));
		printf("Child_1: %d, Child_2: %d\n", totalCh1, totalCh2);
		//listCount(countChild_1); // Prints received from child 1
		//listCount(countChild_2); // Prints received from child 2 

		int merged[26]; // English Alphabet
		memset(&merged, 0, sizeof(merged)); // We fill our array with zeros	
		mergeTables(countChild_1, countChild_2);

		printf("Parent Process Finished!\n");
    } 

    else if(child_1 == 0 && child_2 > 0) { // CHILD_1 PROCESS
		printf("->	Child_1 Process, pid = %d, parent pid = %d\n", getpid(), getppid());
		close(pfd1[1]); // Close Write-End Pipe_1
		close(pfd2[1]); // Close Write-End Pipe_2
		close(pdfFreq1[0]); // Close Read-End PipeFreq_1
		close(pdfFreq2[0]); // Close Read-End PipeFreq_1
		char input[11], filename[9] = "", c;
		// Consume From Pipe
		read(pfd1[0], input, 11);
		char start = input[9], end = input[10], lower;
   		strncpy(filename, input, 8);
		printf("Child_1 File name is: %s | start: %c | end: %c \n", filename, start, end);
		//========== COUNTING ============
		FILE *fp1;
		int count1[26]; // English Alphabet
        memset(&count1, 0, sizeof(count1)); // We fill our array with zeros
        fp1 = fopen(filename, "r"); // Opens text.txt file
		while((c = fgetc (fp1)) != EOF) {
			lower = tolower(c);
			if(isalpha(lower) && lower >= start && lower <= end) {
        		//putchar(c);
        		count1[tolower(c) - 'a']++;
        	}
		}
		fclose(fp1); // Then Close Opened File.
		//printf("Count_1 List: \n");
		//listCount(count1);
		write(pdfFreq1[1], count1, sizeof(count1) + 1);
    } 

    else if(child_2 == 0 && child_1 > 0) { // CHILD_2 PROCESS
		printf("->	Child_2 Process, pid = %d, parent pid = %d\n", getpid(), getppid());
		close(pfd1[1]); // Close Write-End Pipe_1
		close(pfd2[1]); // Close Write-End Pipe_2
		close(pdfFreq1[0]); // Close Read-End PipeFreq_1
		close(pdfFreq2[0]); // Close Read-End PipeFreq_1
		char input2[11], filename2[9] = "", c2;
		// Consume From Pipe
		read(pfd2[0], input2, 11);
		//FILE *fp1;
		char start2 = input2[9], end2 = input2[10], lower2;
   		strncpy(filename2, input2, 8);
		printf("Child_2 File name is: %s | start: %c | end: %c \n", filename2, start2, end2);
		//========== COUNTING ============
		FILE *fp2;
		int count2[26]; // English Alphabet
        memset(&count2, 0, sizeof(count2)); // We fill our array with zeros
        fp2 = fopen(filename2, "r"); // Opens text.txt file
		while((c2 = fgetc (fp2)) != EOF) {
			lower2 = tolower(c2);
			if(isalpha(lower2) && lower2 >= start2 && lower2 <= end2) {
        		//putchar(c2);
        		count2[tolower(c2) - 'a']++;
        	}
		}
		fclose(fp2); // Then Close Opened File.
		//printf("Count_2 List: \n");
		//listCount(count2);
		write(pdfFreq2[1], count2, sizeof(count2) + 1);
    }

    return 0;
}
