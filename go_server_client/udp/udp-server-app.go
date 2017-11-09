package main

import "fmt"
import "net"

//import "fmt"
//import "net"
//import "bufio"
import "time"

/*

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

double get_system_time (void)
{

	double ret = 0;

	struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);

    ret = t.tv_sec + (t.tv_nsec*1e-9);

	return ret;

}

void set_system_time (double sec, double nsec)
{
	struct timespec t;
	t.tv_sec = sec;
	t.tv_nsec = nsec; //( ( time - (double)((int)time) ) * 1e9);

	clock_settime(CLOCK_MONOTONIC, t );

	return;
}

void set_gpio( int hilo )
{

return;

}


*/
import "C"

var error_count int

func error_check(err error) bool {

	ret := false

	if err != nil {
		fmt.Printf("Some error %v\n", err)
		error_count += 1
	}

	if error_count > 10 {

		ret = true
		error_count = 0

	}

	return ret

}

func init_connection(restart_gpio *bool) {

	addr := net.UDPAddr{
		Port: 1111,
		IP:   net.ParseIP("2001:db8::7"),
	}

	fmt.Printf("Waiting for connection...\n\r")

	conn, err := net.ListenUDP("udp6", &addr)

	if err != nil {
		fmt.Printf("Some error %v\n\r", err)
		fmt.Printf("Attempting to reconnect in 2 seconds... \n\r")
		defer init_connection(restart_gpio)
		time.Sleep(time.Second * 2)
		return
	}

	defer connection_up(conn, restart_gpio)

	return
}

func sendResponse(conn *net.UDPConn, addr *net.UDPAddr, restart_gpio *bool) {

	_, err := conn.WriteToUDP([]byte("PING BACK"), addr)

	if error_check(err) == true {
		defer connection_down(conn, restart_gpio)
		return
	}

}
func connection_up(conn *net.UDPConn, restart_gpio *bool) {

	var queue *byte
	var are *byte

	p := make([]byte, 129)

	for {

		_, remoteaddr, err := conn.ReadFromUDP(p)

		if error_check(err) == true {
			defer connection_down(conn, restart_gpio)
			return
		}

		/*
			two types of messages from client:
			(1) ping back as fast as possible for sync and
			(2) set local systemm time with next two values
		*/
		switch p[0] {

		case 1:
			sendResponse(conn, remoteaddr, restart_gpio)
			fmt.Printf("Client at %v sent command: %s \n", remoteaddr, p)

		case 2:

			queue = &(p[1])
			are = &(p[65])

			C.set_system_time(C.double(*queue), C.double(*are))

			*restart_gpio = true

			fmt.Printf("Client at %v sent command: %s \n", remoteaddr, p)

		}

		//TODO dump p,q,r buffers

	}

}

func connection_down(conn *net.UDPConn, restart_gpio *bool) {
	conn.Close()
	fmt.Printf("Connection is down. Attmepting to reconnect...\n\r")
	defer init_connection(restart_gpio)
	return
}

func toggle_gpio(restart_gpio *bool) {

	var gpio_state C.int

	for {

		if *restart_gpio == true {
			//waait for next 10ms slot
			for int(C.get_system_time()*1000)%10 != 0 {
			}
			*restart_gpio = false
			gpio_state = 0
		}

		C.set_gpio(gpio_state)

		gpio_state ^= 1

		time.Sleep(time.Millisecond * 10)

	}

}

func main() {

	var restart_gpio bool

	error_count = 0

	go init_connection(&restart_gpio)
	go toggle_gpio(&restart_gpio)

	for {
	}

}
