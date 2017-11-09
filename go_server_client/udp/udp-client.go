package main

import "fmt"
import "net"
import "bufio"
import "time"
import "unsafe"
/*
TODO:
Write gpio code
figure out how to push to conn buffer directly

*/

/*

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

double get_system_time (double * ret, double * ret1)
{
    double time = 0;
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    *ret = t.tv_sec;
    *ret1 = t.tv_nsec;

    time = t.tv_sec + (t.tv_nsec * 1e-9);

    return time;
}

int get_switch(void)
{

    return 1;

}

void set_gpio( int hilo )
{

    return;

}


*/
import "C"

func init_connection(resync_period *int, restart_gpio *bool) {


    fmt.Printf("initinig connection \n\r")

    conn, err := net.Dial("udp6", "[2001:db8::8]:1111")

    if err != nil {
        fmt.Printf("Some error %v \n\r", err)
        defer init_connection(resync_period, restart_gpio)
        fmt.Printf("reattempting connection in 2 seconds \n\r")
        time.Sleep(time.Second * 2)
    } else {

        defer connection_up(conn, err, resync_period, restart_gpio)
    }

    return

}

func connection_up(conn net.Conn, err error, resync_period *int, restart_gpio *bool) {

    p := make([]byte, 100)

    var send [129]byte

    var t1s C.double
    var t1n C.double
    var t2s C.double
    var t2n C.double
    var t3s C.double
    var t3n C.double
    var nts C.double
    var ntn C.double

    for {

        error_count := 0

        for i := 0; i < 129; i++ {
            send[i] = 0
        }

        send[0] = 1

        //get system time from C
        C.get_system_time(&t1s, &t2n)

        //TODO FILL conn buffer with send fmt.Fprintf(conn, "%1f", send)
        _, err = bufio.NewReader(conn).Read(p)

        C.get_system_time(&t2s, &t2n)

        if err == nil {
            fmt.Printf("%s\n", p)
        } else {
            fmt.Printf("Resync error %v\n\r", err)
            error_count += 1
        }

        latency_sec := (t2s - t1s) / 2
        latency_nsec := (t2n - t1n) / 2

        //server will set it's time using C
        C.get_system_time(&t3s, &t3n)

        nts = t3s + latency_sec
        ntn = t3n + latency_nsec

        send[0] = 2
        C.memcpy(unsafe.Pointer(&(send[1])), unsafe.Pointer(&nts), 64)
        C.memcpy(unsafe.Pointer(&(send[65])), unsafe.Pointer(&ntn), 64)
        /*for i := 1; i < 65; i++ {
            //fmt.Fprint(conn, send[i])
            send[i] = *((&ntn)+i);
        }

        for i := 65; i < 129; i++ {
            //fmt.Fprint(conn, send[i]
            send[i] = *((&ntn)+i);
        }*/



        //TODO not sure if this will work...
        for i := 0; i < 129; i++ {
            fmt.Fprint(conn, send[i])
        }

        _, err = bufio.NewReader(conn).Read(p)

        if err == nil {
            fmt.Printf("%s\n", p)
        } else {
            fmt.Printf("Resync error %v\n\r", err)
            error_count += 1
        }

        if error_count > 10 {
            defer connection_down(conn, resync_period, restart_gpio)
            error_count = 0
            return
        }

        *restart_gpio = true

        time.Sleep(time.Millisecond * time.Duration(*resync_period))

    }

}

func connection_down(conn net.Conn, resync_period *int, restart_gpio *bool) {

    conn.Close()

    fmt.Printf("connection is down...reconnecting \n\r")

    init_connection(resync_period, restart_gpio)

}

/*write this one in C*/
func monitor_switch(resync_period *int) {

    for {

        gpio := C.get_switch()

        if gpio == 1 {
            *resync_period = 1000
        } else {
            *resync_period = 10000
        }

    }

}

/*let's write this one in c*/
func toggle_gpio(restart_gpio *bool) {

    var gpio_state C.int

    for {

        if *restart_gpio == true {
            //waait for next 10ms slot
          //  for int(C.get_system_time(nil, nil)*1000)%10 != 0 {
          //  }
            *restart_gpio = false
            gpio_state = 0
        }

        C.set_gpio(gpio_state)

        gpio_state ^= 1

        time.Sleep(time.Millisecond * 10)

    }

return
}

func main() {


    resync_period := 100
    restart_gpio := true


		fmt.Println("reattempting connection in 2 seconds \n\r")


    time.Sleep(time.Millisecond * 1000)
    go monitor_switch(&resync_period)
    
    time.Sleep(time.Millisecond * 1000)
    go toggle_gpio(&restart_gpio)

    init_connection(&resync_period, &restart_gpio)
    for {
    }
return
}
