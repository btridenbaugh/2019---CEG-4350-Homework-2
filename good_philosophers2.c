#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


// Change the constant below to change the number of philosophers
// coming to lunch...

#define PHILOSOPHER_COUNT 5 //This is N

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
      int times_eaten; //This will keep track of philosophers eating
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
             // The Resource Heirarchy allocates the lowest value choptstick adjacent to the philosopher first
             // in order to prevent the system from deadlocking
             if (philosopher_number == (PHILOSOPHER_COUNT - 1)){ //For the philosopher at the end of the table case
                 pthread_mutex_lock(&chopstick[0]); //Takes lowest first for philosopher at the end of the table, which is right chopstick
                 pthread_mutex_lock(&chopstick[philosopher_number]); //Takes other adjacent chopstick, left chopstick
             }
             else{
                 pthread_mutex_lock(&chopstick[philosopher_number]); //Takes lowest value chopstick first, left chopstick
                 pthread_mutex_lock(&chopstick[philosopher_number + 1]); //Takes next highest, right chopstick
             }
           times_eaten++; //Philosopher has eaten so +1
           // Hurray, if I got this far I'm eating
           printf ("Philosopher %d is eating\n",philosopher_number);
           printf ("Philosopher %d has eaten %d times\n",philosopher_number, times_eaten); //Outputs # times eaten for that philosopher
           usleep(1); // I spend twice as much time eating as thinking...
                      // typical....
           
           // I'm done eating.  Now put the chopsticks back on the table
           //printf ("Philosopher %d finished eating\n",philosopher_number);

           if (philosopher_number == (PHILOSOPHER_COUNT - 1)){ //Philosopher at the end of the table case
               pthread_mutex_unlock(&chopstick[0]); //Release lowest first for philosopher at the end of the table, right chopstick
               pthread_mutex_unlock(&chopstick[philosopher_number]); //Release other adjacent chopstick, left chopstick
           }
           else{
               pthread_mutex_unlock(&chopstick[philosopher_number]); //Release lowest value chopstick first, left chopstick
               pthread_mutex_unlock(&chopstick[philosopher_number + 1]); //Release next highest, right chopstick
           }
           //printf("Philosopher %d has placed chopsticks on the table\n", philosopher_number);

           
         } 

      return(NULL);
     }

int main()
  {  int i;

   srand(time(NULL));

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_init(&chopstick[i],NULL);
      
   //Since chopstick is made first order should go 0C 0P 1C 1P ... (N-1)C (N-1)P

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_join(philosopher[i],NULL);


   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_destroy(&chopstick[i]);

   return 0;
   }

