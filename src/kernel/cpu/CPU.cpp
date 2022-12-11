#include "CPU.h"

#include "../../common/monarch.h"
#include "../../common/stdlib.h"

#include "../memory/memory.h"
#include "../drivers/display.h"

#define cpuid(in, a, b, c, d) __asm__("cpuid": "=a" (a), "=b" (b), "=c" (c), "=d" (d) : "a" (in));

uint_8 YELLOW = (BACKGROUND_BLACK | FOREGROUND_YELLOW);
uint_8 LIGHTGRAY = (BACKGROUND_BLACK | FOREGROUND_LIGHTGRAY);
uint_8 LIGHTRED = (BACKGROUND_BLACK | FOREGROUND_LIGHTRED);

/*
* Get and print the CPU info (vendor, model, stepping, features)
*/
int detect_cpu(void) {
	DWORD ebx, unused;
	cpuid(0, unused, ebx, unused, unused);
	switch (ebx) {
		case 0x756e6547: do_intel(); break; /* Intel Magic Code */
		case 0x68747541: do_amd(); break; /* AMD Magic Code */
		default: display::print("Unknown x86_64 CPU Detected\n\r", LIGHTRED); break;
	}
	return 0;
}

/* Intel Specific brand list */
char *Intel[] = {
	"Brand ID Not Supported.",
	"Intel(R) Celeron(R) processor",
	"Intel(R) Pentium(R) III processor",
	"Intel(R) Pentium(R) III Xeon(R) processor",
	"Intel(R) Pentium(R) III processor",
	"Reserved",
	"Mobile Intel(R) Pentium(R) III processor-M",
	"Mobile Intel(R) Celeron(R) processor",
	"Intel(R) Pentium(R) 4 processor",
	"Intel(R) Pentium(R) 4 processor",
	"Intel(R) Celeron(R) processor",
	"Intel(R) Xeon(R) Processor",
	"Intel(R) Xeon(R) processor MP",
	"Reserved",
	"Mobile Intel(R) Pentium(R) 4 processor-M",
	"Mobile Intel(R) Pentium(R) Celeron(R) processor",
	"Reserved",
	"Mobile Genuine Intel(R) processor",
	"Intel(R) Celeron(R) M processor",
	"Mobile Intel(R) Celeron(R) processor",
	"Intel(R) Celeron(R) processor",
	"Mobile Geniune Intel(R) processor",
	"Intel(R) Pentium(R) M processor",
	"Mobile Intel(R) Celeron(R) processor"
};

/* This table is for those brand strings that have two values depending on the processor signature. It should have the same number of entries as the above table. */
char *Intel_Other[] = {
	"Reserved",
	"Reserved",
	"Reserved",
	"Intel(R) Celeron(R) processor",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Intel(R) Xeon(R) processor MP",
	"Reserved",
	"Reserved",
	"Intel(R) Xeon(R) processor",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void regs2string(int eax, int ebx, int ecx, int edx, char *buf) {
	char string[17];
	string[16] = '\0';
	for (int j = 0; j < 4; j++) {
		string[j] = eax >> (8 * j);
		string[j + 4] = ebx >> (8 * j);
		string[j + 8] = ecx >> (8 * j);
		string[j + 12] = edx >> (8 * j);
	}
	strncpy(buf, string, 17);
}

char brandstring[50] = {0};

char *get_brandstring(){
	DWORD eax, ebx, ecx, edx, max_eax, signature, unused;
	int model, family, type, brand, stepping, reserved;

	cpuid(0x80000000, max_eax, unused, unused, unused);
	cpuid(1, eax, ebx, unused, unused);

	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	type = (eax >> 12) & 0x3;
	brand = ebx & 0xff;
	stepping = eax & 0xf;
	reserved = eax >> 14;
	signature = eax;

	/// If the max extended eax value is high enough to support the processor brand string
	/// (values 0x80000002 to 0x80000004), then we'll use that information to return the brand information.
	/// Otherwise, we'll refer back to the brand tables above for backwards compatibility with older processors.
	/// According to the Sept. 2006 Intel Arch Software Developer's Guide, if extended eax values are supported,
	/// then all 3 values for the processor brand const char *are supported, but we'll test just to make sure and be safe.
	if (max_eax >= 0x80000004) {
		if (max_eax >= 0x80000002) {
			cpuid(0x80000002, eax, ebx, ecx, edx);
			regs2string(eax, ebx, ecx, edx, brandstring);
		}
		if (max_eax >= 0x80000003) {
			cpuid(0x80000003, eax, ebx, ecx, edx);
			regs2string(eax, ebx, ecx, edx, &brandstring[16]);
		}
		if (max_eax >= 0x80000004) {
			cpuid(0x80000004, eax, ebx, ecx, edx);
			regs2string(eax, ebx, ecx, edx, &brandstring[32]);
		}
	} else if (brand > 0) {
		if (brand < 0x18) {
			if (signature == 0x000006B1 || signature == 0x00000F13) {
				strncpy(brandstring, Intel_Other[brand], 48);
			} else {
				strncpy(brandstring, Intel[brand], 48);
			}
		} else {
			strncpy(brandstring, "Reserved\n", 48);
		}
	}
	return brandstring;
}

/* Intel-specific information */
int do_intel(void) {
	display::print("Intel Specific Features:\n\r", YELLOW);

	DWORD eax, ebx, ecx, edx, max_eax, signature, unused;
	int model, family, type, brand, stepping, reserved;
	int extended_family = -1;

	cpuid(1, eax, ebx, unused, unused);

	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	type = (eax >> 12) & 0x3;
	brand = ebx & 0xff;
	stepping = eax & 0xf;
	reserved = eax >> 14;
	signature = eax;

	display::print("Type "); display::print(int_to_string(type)); display::print(" - ");
	switch (type) {
		case 0: display::print("Original OEM\n\r"); break;
		case 1: display::print("Overdrive\n\r"); break;
		case 2: display::print("Dual-capable\n\r"); break;
		case 3: display::print("Reserved\n\r"); break;
	}

	display::print("Family "); display::print(int_to_string(family)); display::print(" - ");
	switch (family) {
		case 3: display::print("i386\n\r"); break;
		case 4: display::print("i486\n\r"); break;
		case 5: display::print("Pentium\n\r"); break;
		case 6: display::print("Pentium Pro\n\r"); break;
		case 15: display::print("Pentium 4\n\r"); break;
	}

	if (family == 15) {
		extended_family = (eax >> 20) & 0xff;
        display::print("Extended family "); display::print(int_to_string(extended_family)); display::print("\n\r");
	}

    display::print("Model "); display::print(int_to_string(model)); display::print(" - ");
	switch (family) {
		case 3: break;
		case 4:
			switch(model) {
				case 0: case 1: display::print("DX\n\r"); break;
				case 2: display::print("SX\n\r"); break;
				case 3: display::print("487/DX2\n\r"); break;
				case 4: display::print("SL\n\r"); break;
				case 5: display::print("SX2\n\r"); break;
				case 7: display::print("Write-back enhanced DX2\n\r"); break;
				case 8: display::print("DX4\n\r"); break;
			}
		break;
		case 5:
			switch(model) {
				case 1: display::print("60/66\n\r"); break;
				case 2: display::print("75-200\n\r"); break;
				case 3: display::print("for 486 system\n\r"); break;
				case 4: display::print("MMX\n\r"); break;
			}
		break;
		case 6:
			switch(model) {
				case 1: display::print("Pentium Pro\n\r"); break;
				case 3: display::print("Pentium II Model 3\n\r"); break;
				case 5: display::print("Pentium II Model 5/Xeon/Celeron\n\r"); break;
				case 6: display::print("Celeron\n\r"); break;
				case 7: display::print("Pentium III/Pentium III Xeon - external L2 cache\n\r"); break;
				case 8: display::print("Pentium III/Pentium III Xeon - internal L2 cache\n\r"); break;
			}
		break;
		case 15:
		break;
	}

	cpuid(0x80000000, max_eax, unused, unused, unused);
	if (max_eax >= 0x80000004) {
		display::print("Brand: ");
		if (max_eax >= 0x80000002) {
			cpuid(0x80000002, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if (max_eax >= 0x80000003) {
			cpuid(0x80000003, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
		if (max_eax >= 0x80000004) {
			cpuid(0x80000004, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
	} else if (brand > 0) {
		display::print("Brand "); display::print(int_to_string(brand)); display::print(" - ");
		if (brand < 0x18) {
			if (signature == 0x000006B1 || signature == 0x00000F13) {
				display::print(Intel_Other[brand]); display::print("\n\r");
			} else {
				display::print(Intel[brand]); display::print("\n\r");
			}
		} else {
			display::print("Reserved\n\r");
		}
	}
    display::print("Stepping: "); display::print(int_to_string(stepping)); display::print(" Reserved: "); display::print(int_to_string(reserved)); display::print("\n\r");
	return 0;
}

/* Print Registers */
void printregs(int eax, int ebx, int ecx, int edx) {
    char string[17];
	string[16] = '\0';
	for (int j = 0; j < 4; j++) {
		string[j] = eax >> (8 * j);
		string[j + 4] = ebx >> (8 * j);
		string[j + 8] = ecx >> (8 * j);
		string[j + 12] = edx >> (8 * j);
	}
	display::print(string, LIGHTGRAY);
}

/* AMD-specific information */
int do_amd(void) {
	display::print("AMD Specific Features:\n\r", YELLOW);

	DWORD extended, eax, ebx, ecx, edx, unused;
	int family, model, stepping, reserved;

	cpuid(1, eax, unused, unused, unused);

	model = (eax >> 4) & 0xf;
	family = (eax >> 8) & 0xf;
	stepping = eax & 0xf;
	reserved = eax >> 12;

    display::print(" Family: ", LIGHTGRAY); display::print(int_to_string(family), LIGHTGRAY);
	display::print("\n\r");
	display::print(" Model: ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY);
	display::print(" [", LIGHTGRAY);
	switch (family) {
		case 4: display::print("486 Model ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY); break;
		case 5:
		switch(model) {
			case 0: case 1: case 2: case 3: case 6: case 7:
            display::print("K6 Model ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY); break;
			case 8: display::print("K6-2 Model 8", LIGHTGRAY); break;
			case 9: display::print("K6-III Model 9", LIGHTGRAY); break;
			default: display::print("K5/K6 Model ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY); break;
		}
		break;
		case 6:
		switch(model) {
			case 1: case 2: case 4:
            display::print("Athlon Model ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY); break;
			case 3: display::print("Duron Model 3", LIGHTGRAY); break;
			case 6: display::print("Athlon MP/Mobile Athlon Model 6", LIGHTGRAY); break;
			case 7: display::print("Mobile Duron Model 7", LIGHTGRAY); break;
			default: display::print("Duron/Athlon Model ", LIGHTGRAY); display::print(int_to_string(model), LIGHTGRAY); break;
		}
		break;
	}
	display::print("]\n\r", LIGHTGRAY);

	cpuid(0x80000000, extended, unused, unused, unused);
	if (extended == 0) {
		return 0;
	}

	if (extended >= 0x80000002) {
		unsigned int j;
		display::print(" Processor Name: ", LIGHTGRAY);
		for (j = 0x80000002; j <= 0x80000004; j++) {
			cpuid(j, eax, ebx, ecx, edx);
			printregs(eax, ebx, ecx, edx);
		}
        display::print("\n\r");
	}

	if (extended >= 0x80000007) {
		cpuid(0x80000007, unused, unused, unused, edx);
		if (edx & 1) {
			display::print(" Temperature Sensing Diode Detected!\n\r", LIGHTGRAY);
		}
	}

	display::print(" Stepping: ", LIGHTGRAY);
    display::print(int_to_string(stepping), LIGHTGRAY);
	display::print("\n\r");
    display::print(" Reserved: ", LIGHTGRAY);
    display::print(int_to_string(reserved), LIGHTGRAY);
    display::print("\n\r");

	// Get CPU frequency
	uint_32 freq = 0;
	if (extended >= 0x80000006) {
		cpuid(0x80000006, unused, unused, unused, edx);
		freq = edx >> 16 & 0xFF;
		freq *= 100;
		freq += (edx >> 8 & 0xFF) * 10;
		freq += edx & 0xFF;
		display::print(" Frequency: ", LIGHTGRAY);
		display::print(int_to_string(freq), LIGHTGRAY);
		display::print(" (", LIGHTGRAY);
		display::print(int_to_string(freq / 1000), LIGHTGRAY);
		display::print(" MHz)\n\r", LIGHTGRAY);
	}

	return 0;
}
