/* 
 * ===========================================================================
 * 
 * datalogger.c --
 * test program for sthread lib
 * spawns two threads which read stdin and /dev/random
 * 
 * Ralf Moeller
 * 
 *    Copyright (C) 2005
 *    Computer Engineering Group
 *    Faculty of Technology
 *    University of Bielefeld
 *    www.ti.uni-bielefeld.de
 * 
 * 1.0 / 29. Jul 05 (rm)
 * - from scratch
 * 1.1 / 13. Feb 07 (rm)
 * - changed INTERVAL* parameter (tests run faster)
 * 1.2 / 28. Jun 22 (rm)
 * - ACHTUNG: INTERVAL_LENGTH geÃ¤ndert und malloc statt auto-var.! 
 *   (100->1 Mio.)
 *
 * ===========================================================================
 */

#include "scheduler/sthread.h"
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TERMINAL_BUFLEN 100

// this thread reads from the terminal and writes separate words to stdout
void
serveTerminal()
{
  int res, index, wordno;
  char buf[TERMINAL_BUFLEN];
  
  printf("serverTerminal: running\n"); fflush(stdout);
  // start writing buffer at the beginning
  index = 0;
  // start counting words
  wordno = 0;
  while(1) {
    // read from stdin
    res = sthr_read(STDIN_FILENO, buf + index, 1);
    if (res > 0) {
      // we read a character
      // either the word ended or the buffer is full
      if (isspace(buf[index]) || (index == TERMINAL_BUFLEN-2)) {
	if (isspace(buf[index])) {
	  // add string end char (at the point where the space occured)
	  buf[index] = '\0';
	}
	else {
	  // add string end char (after last character)
	  buf[index+1] = '\0';
	}
	// buffer starts from beginning
	index = 0;
	// print word to stdout
	printf("[word #%d: \"%s\"]\n", wordno++, buf); fflush(stdout);
	// check if the word was "quit"
	if (!strcmp(buf, "quit")) {
	  printf("serveTerminal: terminal thread ends\n"); fflush(stdout);
	  sthr_exit(0);
	}
      }
      else 
	/* no word, buffer not full */
	index++;
    }
    else {
      // we did not read anything, that should not happen
      fprintf(stderr, "serveTerminal: read error\n"); fflush(stderr);
      sthr_exit(-1);
    }
  }
}

// #define INTERVALS_TO_READ 20
// #define INTERVAL_LENGTH 1000

// somewhat faster: 13. Feb 07 (rm)
// changed:  5. Jul 22 (rm)
//
// Achtung: diese Werte mÃ¼ssen ggf. angepasst werden, wenn die Daten
// zu schnell oder zu langsam geliefert werden!
#define INTERVALS_TO_READ 20
#define INTERVAL_LENGTH 2000000


// this thread reads from /dev/random and prints statistics
void
observeDataChannel()
{
  int fd, sum, n, res, i, got;
  // unsigned char buf[INTERVAL_LENGTH];
  unsigned char *buf = malloc(INTERVAL_LENGTH * sizeof(unsigned char));
  
  printf("observeDataChannel: running\n"); fflush(stdout);
  // our "data" channel is /dev/random
  fd = open("/dev/random", O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "observeDataChannel: could not open data channel\n");
    fflush(stderr);
    free(buf);
    sthr_exit(-1);
  }
  for (n = 0; n < INTERVALS_TO_READ; n++) {
    // read interval
    got = 0;
    while (got < INTERVAL_LENGTH) {
      res = sthr_read(fd, buf+got, INTERVAL_LENGTH-got);
      // printf("res = %d\n", res);
      if (res <= 0) {
	fprintf(stderr, "observeDataChannel: read error\n");
	fflush(stderr);
	free(buf);
	sthr_exit(-1);
      }
      else
	got += res;
    }
    // sum all data
    sum = 0;
    for (i = 0; i < INTERVAL_LENGTH; i++)
      sum += buf[i];
    sum /= INTERVAL_LENGTH;
    // print data information
    printf("(interval %d: average %d)\n", n, sum); fflush(stdout);
  }
  close(fd);
  printf("observeDataChannel: observation thread ends\n"); fflush(stdout);
  free(buf);
  sthr_exit(0);
}

// startup threadh
void
startup()
{
  int cpid[2], i, status;

  printf("startup: spawning serveTerminal\n"); fflush(stdout);
  cpid[0] = sthr_spawn(serveTerminal);
  printf("startup: spawning observeDataChannel\n"); fflush(stdout);
  cpid[1] = sthr_spawn(observeDataChannel);
  for (i = 0; i < 2; i++) {
    printf("startup: waiting for cpid[%d] = %d\n", i, cpid[i]); fflush(stdout);
    if (sthr_waitpid(cpid[i], &status) == -1) {
      fprintf(stderr, "waitpid for %d failed\n", cpid[i]);
      _exit(-1);
    }
    printf("(status = %d)\n", status);
  }
  printf("startup: finished\n");
  /* this process should call _exit and not sthr_exit, since there's
     nobody who would destroy it */
  _exit(0);
}

int
main()
{
  printf("starting procInit\n"); fflush(stdout);
  sthr_init(startup);
  exit(0);
}