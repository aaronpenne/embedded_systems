// pmuon.c
// Aaron Penne

#include <linux/module.h>
#include <linux/kernel.h>

// Defines various register values.
#define CNTR_1	0x00000000
#define CNTR_2	0x00000001
#define CNTR_3	0x00000002
#define CNTR_4	0x00000003

#define L1_HIT	0x00000004	// L1 data cache access
#define L1_MISS	0x00000003	// L1 data cache refill
#define L2_HIT	0x00000016	// L2 data cache access
#define L2_MISS	0x00000017	// L2 data cache refill

// Debug, or not to debug, that is the question
// #define MY_DEBUG

// Defines metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aaron Penne");
MODULE_DESCRIPTION("PMUON");

// For debugging, reads counter value
static inline void read_counter_value(unsigned int counter) {
	unsigned int value = 0x00000000;

	// PMSELR
	// bit[4:0] selects event counter
	asm volatile("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r" (counter));

	// PMXEVCNTR
	asm volatile("mcr p15, 0, %0, c9, c13, 2\n\t" : "=r" (value));

	printk("Counter:\t%u\tResult:\t%u\n", counter, value);

}

// Assigns event to counter, one at a time
static inline void assign_counter(unsigned int counter, unsigned int event) {
	// PMSELR
	// bit[4:0] selects event counter
	asm volatile("mcr p15, 0, %0, c9, c12, 5\n\t" :: "r" (counter));

	// PMXEVTYPER
	// bit[7:0] selects event type
	asm volatile("mcr p15, 0, %0, c9, c13, 1\n\t" :: "r" (event));
}

int init_module(void) {
	unsigned int v;

	printk("PMU is on.\n");

	// PMUSERENR "User Enable Register"
	// bit[0] enables user mode access
	asm volatile("mcr p15, 0, %0, c9, c14, 0\n\t" :: "r" (0x00000001));

	// PMCR "Performance Monitor Control Register"
	// bit[2] resets cycle counter
	// bit[1] resets event counter
	// bit[0] enables PMU
	asm volatile("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r" (0x00000017));

	// PMNCNTENSET "Count Enable Set Register"
	// bit[31] enables cycle counter
	// bit[3:0] enables counters 4:1
	asm volatile("mcr p15, 0, %0, c9, c12, 1\n\t" :: "r" (0x8000000f));

	// PMOVSR "Overflow Flag Status Register"
	// bit[31]
	// bit[3:0
	asm volatile("mcr p15, 0, %0, c9, c12, 3\n\t" :: "r" (0x8000000f));

	// PMINTENCLR "Interrupt Enable Clear Register"
	asm volatile("mcr p15, 0, %0, c9, c14, 2\n\t" :: "r" (~0));

	// PMCR "Performance Monitor Control Register"
	// bit[15:11] contain number of event counters
	// Reads number of enabled counters
	asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v));
	printk("pmuon_init(): have %d configurable event counters from PMCR.\n", (v >> 11) & 0x1f);

	// Assign events to counters
	assign_counter(CNTR_1, L1_HIT);
	assign_counter(CNTR_2, L1_MISS);
	assign_counter(CNTR_3, L2_HIT);
	assign_counter(CNTR_4, L2_MISS);
	
	// Reads counter values for debugging purposes
	#ifdef MY_DEBUG
		read_counter_value(CNTR_1);
		read_counter_value(CNTR_2);
		read_counter_value(CNTR_3);
		read_counter_value(CNTR_4);
	#endif

	printk("PMUON Complete.\n");	

	return 0;
}

void cleanup_module(void) {
	printk("PMU is off.\n");
}
