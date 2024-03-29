//ARBITER SOLUTION

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


// Change the constant below to change the number of philosophers
// coming to lunch...

#define PHILOSOPHER_COUNT 5

// Each philosopher is represented by one thread.  Each thread independenly
// runs the same "think/start eating/finish eating" program.
pthread_t philosopher[PHILOSOPHER_COUNT];

// Each chopstick gets one mutex.  If there are N philosophers, there are
// N chopsticks.  That's the whole problem.  There's not enough chopsticks 
// for all of them to be eating at the same time.  If they all cooperate, 
// everyone can eat.  If they don't... or don't know how.... well.... 
// philosophers are going to starve.

pthread_mutex_t chopstick[PHILOSOPHER_COUNT];

// The arbitrator solution adds a "waiter" that ensures that only pairs of
// chopsticks are grabbed.  Here is the mutex for the waiter ;)

pthread_mutex_t waiter;


int Random_Int(int max)
   {  return((rand() % (max+1))); 
   }

void *philosopher_program(int philosopher_number)
   {  // In this version of the "philosopher program", the philosopher
      // will think and eat forever.
      
      int i_want_this;
      int i_want_this_too;
      
      while (1)
         { // Philosophers always think before they eat.  They need to 
           // build up a bit of hunger....
         
           usleep(1);  
           
           // That was a lot of thinking.... now hungry... this 
           // philosopher is a jerk.  He may not grap the chopsticks 
           // closest to him.  In fact, he may grab any two chopsticks at
           // the table.... because he is a jerk.
           
           i_want_this = Random_Int(PHILOSOPHER_COUNT);// Need to change this so it takes the lower value
          
           while ((i_want_this_too = Random_Int(PHILOSOPHER_COUNT)) == i_want_this);

           printf ("Jerk Philosopher %d wants chopsticks %d and %d\n",
                    philosopher_number,
                    i_want_this,
                    i_want_this_too);
            
           //The Arbiter solutions makes all other philsophers wait while someone is grabbing their chopsticks
             
           pthread_mutex_lock(&waiter); //I'm making everyone else wait while I grab chopsticks!
             
           pthread_mutex_lock(&chopstick[i_want_this]);
           pthread_mutex_lock(&chopstick[i_want_this_too]);
           
           pthread_mutex_unlock(&waiter); //I have my chopsticks now
           // Hurray, if I got this far I'm eating
           printf ("Jerk Philosopher %d is eating with chopsticks %d and %d\n",
                   philosopher_number,
                   i_want_this,
                   i_want_this_too);
           
           // I'm done eating.  Now put the chopsticks back on the table
           pthread_mutex_unlock(&chopstick[i_want_this]);
           pthread_mutex_unlock(&chopstick[i_want_this_too]);           
         } 

      return(NULL);
     }

int main()
  {  int i;

   srand(time(NULL));//Starts all children processes at random times

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_init(&chopstick[i],NULL);
      
   pthread_mutex_init(&waiter,NULL); //Adds in the waiter

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_join(philosopher[i],NULL);


   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_destroy(&chopstick[i]);

   return 0;
   }

