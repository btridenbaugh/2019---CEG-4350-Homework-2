#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


// Change the constant below to change the number of philosophers
// coming to lunch...

#define PHILOSOPHER_COUNT 4

// Each philosopher is represented by one thread.  Each thread independenly
// runs the same "think/start eating/finish eating" program.
pthread_t philosopher[PHILOSOPHER_COUNT];

// Each chopstick gets one mutex.  If there are N philosophers, there are
// N chopsticks.  That's the whole problem.  There's not enough chopsticks 
// for all of them to be eating at the same time.  If they all cooperate, 
// everyone can eat.  If they don't... or don't know how.... well.... 
// philosophers are going to starve.

pthread_mutex_t chopstick[PHILOSOPHER_COUNT];


void *philosopher_program(int philosopher_number)
   {  // In this version of the "philosopher program", the philosopher
      // will think and eat forever.
      
      while (1)
         { // Philosophers always think before they eat.  They need to 
           // build up a bit of hunger....
         
           //printf ("Philosopher %d is thinking\n", philosopher_number);
           usleep(1);  
           
           // That was a lot of thinking.... now hungry... this 
           // philosopher (who knows his own number) grabs the chopsticks
           // to her/his right and left.  The chopstick to the left of 
           // philosopher N is chopstick N.  The chopstick to the right
           // of philosopher N is chopstick N+1
           
           //printf ("Philosopher %d wants chopsticks\n",philosopher_number);

           pthread_mutex_lock(&chopstick[philosopher_number]);
           pthread_mutex_lock(&chopstick[(philosopher_number+1)%PHILOSOPHER_COUNT]);
           
           // Hurray, if I got this far I'm eating
           printf ("Philosopher %d is eating\n",philosopher_number);

           //usleep(1); // I spend twice as much time eating as thinking...
                      // typical....
           
           // I'm done eating.  Now put the chopsticks back on the table
           //printf ("Philosopher %d finished eating\n",philosopher_number);

           pthread_mutex_unlock(&chopstick[philosopher_number]);
           pthread_mutex_unlock(&chopstick[(philosopher_number+1)%PHILOSOPHER_COUNT]);
           //printf("Philosopher %d has placed chopsticks on the table\n", philosopher_number);

           
         } 

      return(NULL);
     }

int main()
  {  int i;

   srand(time(NULL));

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_init(&chopstick[i],NULL);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_join(philosopher[i],NULL);


   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_destroy(&chopstick[i]);

   return 0;
   }

