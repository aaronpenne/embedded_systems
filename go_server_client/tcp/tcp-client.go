// tcp-client.go
// Aaron Penne & Jacob Torres
//
// Acts as client for TCP connections over 6LoWPAN.
// Purpose is to send client's timestamp to server.
// Run with tcp-server.go

package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"time"
	"os"
)

func main() {

	// Checks that the command line paramaters are correct
	if len(os.Args) != 2 {
		fmt.Fprintf(os.Stderr, "Usage: %s host:port\n", os.Args[0])
		os.Exit(1)
	}

	address_raw := os.Args[1]

	// Parses address input and checks for valid IPv6 format
	address_remote, err := net.ResolveTCPAddr("tcp6", address_raw)
    catch(err)
	
	// Connects to the remote TCP socket
	connection, err := net.DialTCP("tcp6", nil, address_remote)
	catch(err)
	
	for { 
		// Creates timestamp
		tx := time.Now().Format("02:Jan:2006:15:04.000000")
		fmt.Printf("tx > %s\n", tx)
		
		// Sends client's current time out to server
		_, err := fmt.Fprintf(connection, tx + "\n")
		catch(err)
		
		// Listens for reply
		rx, err := bufio.NewReader(connection).ReadString('\n')
		catch(err)
		fmt.Print("rx < "+rx)
		
		// Waits for one second
		time.Sleep(1000 * time.Millisecond)
	}
}

func catch(err error) {
		// Checks if any errors exist and dies somewhat gracefully
		if err != nil {
			log.Fatal(err)
			os.Exit(1)
		}
}
