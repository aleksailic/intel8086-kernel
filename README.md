# Multi-Threaded kernel for intel8086

**goal:** implementation of a functional kernel for a multi-threaded operating system with time sharing. Kernel provides the user with concepts of Threads, Semaphores and Events through syscalls while itself being run in a seperate kernel thread which internally is loaded through system interrupt on overloaded IVT entry (60h). That way user and kernel implementation are seperated and thus independent. 

**motive:** this is my semestral work for a course in Operating Systems. PCB Scheduling is done through Scheduler abstraction that I've been given beforehand as it is not the focal point of this course.

**dependencies**: scheduler (declared in SCHEDULE.H and deployed in lib as APPLICAT.LIB)
