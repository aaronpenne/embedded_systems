package main

import (
	"log"
	"math/rand"
	"time"
)

const (
	LOOP       = 100     // iterations before printing stats
	NUM_SEATS  = 4       // number of philosophers and forks
	MIN_THINK  = 5000    // 5 seconds
	MAX_THINK  = 10000   // 10 seconds
	MIN_EAT    = 1000    // 1 second
	MAX_EAT    = 2000    // 2 seconds
	STATISTICS = 1       // number of times to loop before showing statistics
)

type fork int

type philosopher struct {
	name          int
	primary       chan fork
	secondary     chan fork
	time_thinking time.Duration
	time_waiting  time.Duration
	time_eating   time.Duration
}

func wait(min int, max int) time.Duration {
	rand.Seed(time.Now().Unix())
	
	// Calculates milliseconds to sleep within given range
	return time.Duration((rand.Intn(max - min)) + min) * time.Millisecond
}

// Rounds the time duration to manageable accuracy
func round(d time.Duration) time.Duration {
	r := time.Millisecond
	m := d % r
	d  = d - m
	return d
}

func (guy *philosopher) one_loop(loop chan bool) {
	start   := time.Now()
	counter := 0
	
	for {
		// Thinking - random amount of time within given range
		start_thinking := time.Now()
		log.Println(guy.name, "thinking")
		time.Sleep(wait(MIN_THINK, MAX_THINK))
		guy.time_thinking = time.Since(start_thinking) + guy.time_thinking
		
		// Get forks - don't need to wait until both are ready to grab one
		start_waiting := time.Now()
		<-guy.primary
		log.Println(guy.name, "has one fork")
		<-guy.secondary
		log.Println(guy.name, "has both forks")
		guy.time_waiting = time.Since(start_waiting) + guy.time_waiting

		// Eating - random amount of time within given range
		start_eating := time.Now()
		log.Println(guy.name, "eating")
		time.Sleep(wait(MIN_EAT, MAX_EAT))
		guy.time_eating = time.Since(start_eating) + guy.time_eating
		
		// Release forks - puts forks back into the channels
		log.Println(guy.name, "done eating")
		guy.primary   <- 1
		guy.secondary <- 1
		
		// Comment out "loop <- true" to loop forever
		// loop <- true
		
		if counter > STATISTICS {
			log.Printf("-------------------------------\n")
			log.Printf("| Statistics for Philosopher %v\n", guy.name)
			log.Printf("|   Thinking %v\n", round(guy.time_thinking))
			log.Printf("|   Eating   %v\n", round(guy.time_eating))
			log.Printf("|   Waiting  %v\n", round(guy.time_waiting))
			log.Printf("| Total      %v\n", round(time.Since(start)))
			log.Printf("-------------------------------\n")
			counter = 0
		}
		counter++
	}
}

func setup_philosopher(name int, primary chan fork, secondary chan fork) *philosopher {
	guy := &philosopher{name, primary, secondary, 0, 0, 0}
	guy.secondary <- 1
	return guy
}

func main() {
	philosophers := make([]*philosopher, NUM_SEATS)
	var guy *philosopher
	
	// Control for the number of loops (forever is default)
    loop := make(chan bool)
	
	// Set the table up with seating arrangment and forks
    table_head  := make(chan fork, 1)  // Only allowing buffer size of one
    table_head  <- 1
	primary_tmp := table_head
	for i := 1; i < NUM_SEATS; i++ {
		guy = setup_philosopher(i, primary_tmp, make(chan fork, 1))
		philosophers[i] = guy
		primary_tmp = philosophers[i].secondary
	}
	
	// Flipping the head of the table's forks prevents deadlock
	guy = setup_philosopher(0, table_head, make(chan fork, 1))
	philosophers[0] = guy

	// Run each of the philosophers concurrently
	for i := 0; i < NUM_SEATS; i++ {
		go philosophers[i].one_loop(loop)
	}
	
	// Waits for philosphers to finish
	for i := 0; i < NUM_SEATS; i++ {
		<-loop 
	}
}