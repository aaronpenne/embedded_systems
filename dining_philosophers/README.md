# Dining Philosophers

The classic CS problem, in go.

This algorithm avoids deadlock by switching the primary fork for one of the philosophers. It tracks wait/think/eat times and outputs them for each philosopher for each loop. The algorithm has not deadlocked in testing, and no philosophers starve. Busy waiting was avoided to increase power savings. There are some inefficiences, in that one of the philosophers tends to eat less than the others, but he still lives so it is acceptable. 