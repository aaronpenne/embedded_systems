package main

import (
	"log"
	"time"
)

func main() {
	for i := 0; i < 30; i++ {
		log.SetFlags(log.LstdFlags | log.Lmicroseconds)
		log.Println("start")
		time.Sleep(30 * time.Second)
		log.Println("stop")
	}
}
