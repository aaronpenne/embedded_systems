// tcp-server.go
// Aaron Penne & Jacob Torres
//
// Acts as always on server for TCP connections over 6LoWPAN.
// Purpose is to send server's timestamp to client.
// Run with tcp-client.go

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

	fmt.Println("Server is starting up...")
	
	// Listens on all interfaces, but only specific port
	listening, err := net.Listen("tcp", ":1111")
	catch(err)
	
	// Accepts connection, first come first serve
	connection, err := listening.Accept()
	catch(err)

	for {
		// will listen for message to process ending in newline (\n)
		rx, err := bufio.NewReader(connection).ReadString('\n')
		catch(err)
		
		// Outputs message received from client
		fmt.Print("rx < ", string(rx))
	
		// Sends server's current time out to client
		tx := time.Now().Format("02:Jan:2006:15:04.000000")
		fmt.Printf("tx > %s\n", tx)
		_, err = connection.Write([]byte(tx + "\n"))
		catch(err)
	}
}

func catch(err error) {
		// Checks if any errors exist and dies somewhat gracefully
		if err != nil {
			log.Fatal(err)
			os.Exit(1)
		}
}
