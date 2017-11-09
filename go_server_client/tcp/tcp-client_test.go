package main

import "testing"

func BenchmarkClient(b *testing.B) {  
    for n := 0; n < b.N; n++ {
        main()
    }
}
