
/*
 *  Copyright 2020-2023 ARCOS.INF.UC3M.ES
 *
 *  This file is part of Operaring System Labs (OSL).
 *
 *  OSL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OSL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OSL.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include "request.h"
#include <sys/types.h>
#include <sys/wait.h>

const int MAX_PETICIONES = 5;

void  receiver ( void )
{
    request_t p;
    int pid, n_children=0;

    for (int i=0; i<MAX_PETICIONES; i++)
    {
       receive_request(&p);

       pid = fork();
       if (pid  < 0) { perror("Error in fork: ");   }
       if (pid == 0) { answer_request(&p); exit(0); }   /* CHILD */
       if (pid != 0) { n_children++; }                  /* FATHER */
    }

    fprintf(stderr, "Esperando fin de %d n_children\n", n_children);
    while (n_children > 0)
    {
         pid = waitpid(-1, NULL, WNOHANG);
         if (pid > 0) { n_children--;  }
    } ;
}

int main ( int argc, char *argv[] )
{
    struct timeval timenow;
    long t1, t2;

    // t1
    gettimeofday(&timenow, NULL) ;
    t1 = (long)timenow.tv_sec * 1000 + (long)timenow.tv_usec / 1000 ;

    // receiver...
    receiver() ;

    // t2
    gettimeofday(&timenow, NULL) ;
    t2 = (long)timenow.tv_sec * 1000 + (long)timenow.tv_usec / 1000 ;

    // imprimir t2-t1...
    printf("Total time: %lf\n", (t2-t1)/1000.0);

    return 0;
}

