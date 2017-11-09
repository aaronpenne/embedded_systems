package main

import (
        "os"
        "fmt"
        "os/exec"
)

func main() {
        err := exec.Command("echo", "3 > /proc/sys/vm/drop_caches").Run()
        if err != nil {
                fmt.Fprintln(os.Stderr, err)
                os.Exit(1)
        }
        fmt.Println("Cache cleared")
}
