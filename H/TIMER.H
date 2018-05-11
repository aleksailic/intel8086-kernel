/*
 * TIMER.H
 *
 *  Created on: May 11, 2018
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "PCB.H"
#include "THREAD.H"

//pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta) brojac--;
	if (brojac == 0 || zahtevana_promena_konteksta) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
		}

		running->sp = tsp;
		running->ss = tss;

		running= getNextPCBToExecute();	// Scheduler

		tsp = running->sp;
		tss = running->ss;

		brojac = running->kvant;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
if(!zahtevana_promena_konteksta) asm int 60h;

	zahtevana_promena_konteksta = 0;
}



#endif /* TIMER_H_ */
