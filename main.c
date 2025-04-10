#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#define READABLE_FILE "./file_to_read.txt" /* File to be read during read operations */
#define BYTES_TO_READ_WRITE 819200       /* 800 KB */
#define MAX_BUFFER 1048576               /* 1 MB*/
#define MAX_BUFFER 1048576
char write_buf[MAX_BUFFER]; // Write Buffer 
char *wp;                   // Write Pointer
int write_buf_size = 0;     // Write Buffer Size
char read_buf[MAX_BUFFER];  // Read Buffer
char *rp;                   // Read Pointer
int read_buf_size;          // Read Buffer Size
int read_count;             // Data Read from The Buffer
/* Function for write without buffer */
void mywritec(char);
/* Functions for write with buffer */
void mywritebufsetup(int);
void myputc(char);
void mywriteflush(void);
/* Function for read without buffer */
int myreadc(void);
/* Functions for read with buffer */
void myreadbufsetup(int);
int mygetc(void);
/* Declare global variables for write operations here */
int fd_write = 1;
/* Declare global variables for read operations here */
int fd_read;
/* Main function starts */
int main()
{
    clock_t begin, end;
    int option, n, temp;
    const char *a="Writing byte by byte\n";
    const char *b="Writing using buffers\n";
    unsigned long i, bytes_to_write = BYTES_TO_READ_WRITE, bytes_to_read = BYTES_TO_READ_WRITE;
    char ch;

    while(1)
    {
        printf("\n 1 - Write without buffering \n 2 - Write with buffering");
        printf("\n 3 - Read without buffering \n 4 - Read with buffering");
        printf(("\n 5 - Exit \n Enter the option: "));
        scanf("%d", &option);
        switch(option)
        {
            case 1: /* Write without buffer */
                begin = clock();
                for (i=0;i<bytes_to_write;i++)
                {
                    ch = a[i%strlen(a)];
                    mywritec(ch);
                }
                end = clock();
                printf("\n Time to write without buffering: %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
                break;

            case 2:  /* Write with buffer */
                printf("\n Enter the buffer size in bytes: ");
                scanf("%d", &n);
                mywritebufsetup(n);
                begin = clock();
                for (i=0;i<bytes_to_write;i++)
                {
                    ch = b[i%strlen(b)];
                    myputc(ch);
                }
                mywriteflush();
                end = clock();
                printf("\n Time to write with buffering: %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
                break;

            case 3:  /* Read without buffer */
                fd_read = open(READABLE_FILE, O_RDONLY);
                if(fd_read < 0)
                {
                    printf("\n Error opening the readable file \n");
                    return 1;
                }
                begin = clock();
                for (i=0;i<bytes_to_read;i++)
                {  /* you may need to modify this slightly to print the character received and also check for end of file */
                    int c; // Variable to store the read character
                    if(i%1000==0) printf("reading bytes %ld / %ld \n",i, bytes_to_read);
                    c = myreadc(); // Read character
                    if(c == -1)
                    {
                        printf("\n End of file \n");
                        break;
                    }
                    printf("%c", c); // Print the character to verify correctness
                }
                end = clock();
                if(close(fd_read))
                {
                    printf("\n Error while closing the file \n ");
                }
                printf("\n Time to read without buffering: %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
                break;

            case 4:  /* Read with buffer */
                printf("\n Enter the buffer size in bytes: ");
                scanf("%d", &n);
                myreadbufsetup(n);
                fd_read = open(READABLE_FILE, O_RDONLY);
                if(fd_read < 0)
                {
                    printf("\n Error opening the readable file \n");
                    return 1;
                }
                begin = clock();
                for (i=0;i<bytes_to_read;i++)
                { /* you may need to modify this slightly to print the character received and also check for end of file */
                    int c = mygetc(); // Capture the character
                    if (c == -1) {
                        printf("\n End of file \n");
                        break;
                    }
                    printf("%c", c); // Print character to verify correctness
                }
                end = clock();
                if(close(fd_read))
                {
                    printf("\n Error while closing the file \n ");
                }
                printf("\n Time to read with buffering: %f secs\n",(double)(end - begin)/CLOCKS_PER_SEC);
                break;

            default:
                return 0;
        }
    }
}
/**
 * @brief Writes a single character to the standard output without buffering.
 *
 * This function directly writes a single character to the standard output (file descriptor 1)
 * using the `write` system call.
 *
 * @param ch The character to be written.
 */
void mywritec(char ch) {
    write(1, &ch, 1);
}
/**
 * @brief Sets up the write buffer for buffered writing.
 *
 * This function initializes the write buffer size and sets the write pointer to the start of the buffer.
 * It ensures that the buffer size is within valid limits before proceeding.
 *
 * @param n The size of the buffer in bytes.
 */
void mywritebufsetup(int n) {
    if ((n < 0) || (n > MAX_BUFFER))
        return;
    else {
        write_buf_size = n;
        wp = write_buf;
    }
}
/**
 * @brief Writes a single character to the write buffer.
 *
 * This function adds a character to the write buffer. If the buffer becomes full,
 * it writes the contents of the buffer to the standard output and resets the buffer pointer.
 *
 * @param ch The character to be written to the buffer.
 */
void myputc(char ch) {
    *wp++ = ch;
    if ((wp - write_buf) == write_buf_size) {
        write(1, write_buf, write_buf_size);
        wp = write_buf;
    }
}
/**
 * @brief Flushes the write buffer by writing its contents to the standard output.
 *
 * This function checks if there is any data in the write buffer that needs to be written.
 * If the buffer contains data, it writes the data to the standard output (file descriptor 1).
 * 
 * Steps:
 * 1. Check if the difference between the current write pointer (`wp`) and the start of the write buffer (`write_buf`) is non-zero.
 *    This indicates that there is data in the buffer that needs to be written.
 * 2. If there is data in the buffer, call the `write` system call to write the contents of the buffer
 *    to the standard output. The size of the data to be written is determined by `write_buf_size`.
 */
void mywriteflush(void) {
    if ((wp - write_buf) != 0) {
        write(1, write_buf, wp - write_buf);
        wp = write_buf;
    }
}

/* Function to read a single character from the file without buffering */
/* This function reads a single character from the file descriptor fd_read and returns it as an integer.
   If the end of the file is reached, it returns -1. */
int myreadc(void) {
    char c;
    int bytes_read = read(fd_read, &c, 1);
    return (bytes_read == 1) ? (unsigned char)c : -1;
}
/* Function to set up the read buffer for buffered reading */
/* This function initializes the read buffer size and sets the read count to zero.
   It checks if the provided buffer size is valid (between 0 and MAX_BUFFER). */
void myreadbufsetup(int n) {
    if ((n < 0) || (n > MAX_BUFFER))
        return;
    else {
        read_buf_size = n;
        read_count = 0;
    }
}
/* Function to read a single character from the file using buffered reading */
/* This function reads a single character from the read buffer. If the buffer is empty, it refills it by reading from the file. */
int mygetc(void) {
    if (read_count <= 0) {
        read_count = read(fd_read, read_buf, read_buf_size);
        if (read_count <= 0) {
            return -1;
        }
        rp = read_buf;
    }
    char ch = *rp++;
    read_count--;
    return (unsigned char)ch;
}