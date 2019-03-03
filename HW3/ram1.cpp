/*
	Ram1.cpp
	original
	Homework 3
	Franco Pettigrosso
*/
#include<pch.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;
/*
 Physical size is used for the size of the physical frame size
 Size is how many words are in frame
 TLB size is to tell how many tuples the tlb has
*/
int const PHYICALSIZE = 256;
int const SIZE = 256;
int const TLBSIZE = 16;
/*
 Frame
 A class that hold the basic information about a part of physical memory
 In this program, a Frame is inside an Array of Frames. This makes up the
 Physical memory
*/
class Frame {
public:
	int Title; //used to identify what section is the frame
	char Bits[SIZE]; //used to store what is in the bin
	//used to give what would be the physical address if the bytes where
	//put in one long array
	int physical[SIZE];
	/*
		Frame
		a constructor to set up a frame array
	*/
	Frame() {};
	/*
		Frame
		a constructor to set up an individual frame
		sets up the physical address and sets the bits
		array to all zeros.
	*/
	Frame(int title) {
		this->Title = title;
		int temp = title * 256;
		for (int i = 0; i < 256; i += 16) {
			physical[i] = i + temp;
			physical[i + 1] = i + 1 + temp;
			physical[i + 2] = i + 2 + temp;
			physical[i + 3] = i + 3 + temp;
			physical[i + 4] = i + 4 + temp;
			physical[i + 5] = i + 5 + temp;
			physical[i + 6] = i + 6 + temp;
			physical[i + 7] = i + 7 + temp;
			physical[i + 8] = i + 8 + temp;
			physical[i + 9] = i + 9 + temp;
			physical[i + 10] = i + 10 + temp;
			physical[i + 11] = i + 11 + temp;
			physical[i + 12] = i + 12 + temp;
			physical[i + 13] = i + 13 + temp;
			physical[i + 14] = i + 14 + temp;
			physical[i + 15] = i + 15 + temp;
			Bits[i] = 0b00000000;
			Bits[i + 1] = 0b00000000;
			Bits[i + 2] = 0b00000000;
			Bits[i + 3] = 0b00000000;
			Bits[i + 4] = 0b00000000;
			Bits[i + 5] = 0b00000000;
			Bits[i + 6] = 0b00000000;
			Bits[i + 7] = 0b00000000;
			Bits[i + 8] = 0b00000000;
			Bits[i + 9] = 0b00000000;
			Bits[i + 10] = 0b00000000;
			Bits[i + 11] = 0b00000000;
			Bits[i + 12] = 0b00000000;
			Bits[i + 13] = 0b00000000;
			Bits[i + 14] = 0b00000000;
			Bits[i + 15] = 0b00000000;

		}
	}
};
/*
	LRUSize
	class that handles the Frame policy.
	I tried the stack method with a twist.
*/
class LRUSize {
public:
	//used to keep a list of frame titles
	int titles[PHYICALSIZE];
	/*
		LRUSize
		constructor to intialize titles. the titles are done in reverse
		so the algorithm takes the lower sections first. Also this ensures
		that the algorithm takes all the free ones first.
	*/
	LRUSize() {

		for (int i = PHYICALSIZE - 1; i >= 0; i--)
			titles[i] = (PHYICALSIZE - 1) - i;
		/*
		titles[0] = 127;
		titles[1] = 126;
		titles[2] = 125;
		titles[127] = 0;
		*/
	}

	/*
		pop
		a function that takes the last index in titles.
		pushes everything down, then puts that last index
		and puts in at the top. returns that last index also.
	*/
	int pop() {
		int temp = titles[PHYICALSIZE - 1];
		for (int i = PHYICALSIZE - 1; i > 0;)
		{
			int t = titles[--i];
			titles[i + 1] = t;
		}
		titles[0] = temp;
		return temp;
	}
	/*
		Pick
		like, pop but looks for the title given, pushes
		everything up to that title in the array down one
		then puts that title at the top.
	*/
	void pick(int title) {
		int j = 0;
		for (; j < PHYICALSIZE; j++)
			if (titles[j] == title)
				break;
		while (j >= 1)
		{
			int t = titles[--j];
			titles[j + 1] = t;
		}
		titles[0] = title;
	}
};
/*
	LRUTLB
	like LRUSize but designed for tlbsize.
	POP and Pick are exactly the same
*/
class LRUTLB {
public:
	int titles[TLBSIZE];
	/*
		LRUTLB
		a constructor the makes up the LRUTLB object
		makes the tlb titles all -1.
	*/
	LRUTLB() {
		for (int i = 0; i < TLBSIZE; i++)
			titles[i] = -1;
	}
	/*
		pop
		a function that takes the last index in titles.
		pushes everything down, then puts that last index
		and puts in at the top. returns that last index also.
	*/
	int pop() {
		int temp = titles[TLBSIZE - 1];
		for (int i = TLBSIZE - 1; i > 0;)
		{
			int t = titles[--i];
			titles[i + 1] = t;
		}
		titles[0] = temp;
		return temp;
	}
	/*
	Pick
	like, pop but looks for the title given, pushes
	everything up to that title in the array down one
	then puts that title at the top.
	*/
	void pick(int title) {
		int j = 0;
		for (; j < TLBSIZE; j++)
			if (titles[j] == title)
				break;
		while (j > 0) {
			int t = titles[--j];
			titles[j + 1] = t;
		}
		titles[0] = title;
	}
};
/*
	Page
	used to construct a page table
*/
class Page {
public:
	int Title;
	bool Valid;
	bool DirtyBit;
	Frame* frame;
	/*
		Page
		constructor so we can make an array
	*/
	Page() {};
	/*
		Page
		Construcor so we can intialize a signal page
	*/
	Page(int title) {
		Title = title;
		Valid = 0;
		frame = nullptr;
		DirtyBit = 0;
	}
};
/*
	Statistics
	A class to keep a record of required information
*/
class Statistics {
public:
	double NumberOfCalls, NumberOFTLBHits, NumberOFPageFaults, NumberOfWrites;
	/*
		Statistics
		a constructor to intialize a statistics object
	*/
	Statistics() {
		this->NumberOfCalls = 0;
		this->NumberOFTLBHits = 0;
		this->NumberOFPageFaults = 0;
		this->NumberOfWrites = 0;
	}
};
/*
	TLBHIT
	used to see if a pagenumber is in the tlb. Is so,
	return true and points the double pointer to the frame
	in physical memory
*/
bool TLBHit(Page *TLB, int pagenumber, Frame*& Framenumber) {
	for (int i = 0; i < TLBSIZE; i++)
		if (TLB[i].Title == pagenumber)
		{
			Framenumber = TLB[i].frame;
			return true;
		}
	return false;
}
/*
	PageHit
	See if a page is in memory. Is so,
	return true and points the double pointer to the frame
	in physical memory
*/
bool PageHit(Page *Page, int pagenumber, Frame*& frameIndex) {
	if (Page[pagenumber].Valid)
	{
		frameIndex = Page[pagenumber].frame;
		return true;
	}
	return false;
}
/*
	IsDirty
	Checks to see if the command given is a write command. if so
	the tlb and pagetable are updated accordingly
*/
void IsDirty(Page *TLBTable, Page *PageTable, char command, Frame*& frame) {
	if (command == 'R')
		return;
	int i, j;
	for (i = 0; i < TLBSIZE; i++)
		if (TLBTable[i].frame == frame) {
			TLBTable[i].DirtyBit = 1;
			break;
		}
	for (j = 0; j < SIZE; j++)
		if (PageTable[j].frame == frame) {
			PageTable[j].DirtyBit = 1;
			break;
		}
}
/*
	GetValue
	gives the value of given value in 2's compliment
*/
int GetValue(char value) {
	bool LastBit = value & 128;
	if (LastBit)
		return value & 127 - 128;
	return value;
};
/*
	Main
	the main program
*/
int main(int argc, char* args[]) {
	//check to see if addresses are given
	if (sizeof(args) > 3) {
		Frame PhysicalMemory[PHYICALSIZE];
		Page TLB[TLBSIZE];
		Page PageTable[SIZE];
		for (int i = 0; i < TLBSIZE; i += 2)
		{
			TLB[i] = Page(-1);
			TLB[i + 1] = Page(-1);
		}
		for (int i = 0; i < PHYICALSIZE; i += 16) {
			PhysicalMemory[i] = Frame(i);
			PhysicalMemory[i + 1] = Frame(i + 1);
			PhysicalMemory[i + 2] = Frame(i + 2);
			PhysicalMemory[i + 3] = Frame(i + 3);
			PhysicalMemory[i + 4] = Frame(i + 4);
			PhysicalMemory[i + 5] = Frame(i + 5);
			PhysicalMemory[i + 6] = Frame(i + 6);
			PhysicalMemory[i + 7] = Frame(i + 7);
			PhysicalMemory[i + 8] = Frame(i + 8);
			PhysicalMemory[i + 9] = Frame(i + 9);
			PhysicalMemory[i + 10] = Frame(i + 10);
			PhysicalMemory[i + 11] = Frame(i + 11);
			PhysicalMemory[i + 12] = Frame(i + 12);
			PhysicalMemory[i + 13] = Frame(i + 13);
			PhysicalMemory[i + 14] = Frame(i + 14);
			PhysicalMemory[i + 15] = Frame(i + 15);
		}
		for (int i = 0; i < SIZE; i += 16) {
			PageTable[i] = Page(i);
			PageTable[i + 1] = Page(i + 1);
			PageTable[i + 2] = Page(i + 2);
			PageTable[i + 3] = Page(i + 3);
			PageTable[i + 4] = Page(i + 4);
			PageTable[i + 5] = Page(i + 5);
			PageTable[i + 6] = Page(i + 6);
			PageTable[i + 7] = Page(i + 7);
			PageTable[i + 8] = Page(i + 8);
			PageTable[i + 9] = Page(i + 9);
			PageTable[i + 10] = Page(i + 10);
			PageTable[i + 11] = Page(i + 11);
			PageTable[i + 12] = Page(i + 12);
			PageTable[i + 13] = Page(i + 13);
			PageTable[i + 14] = Page(i + 14);
			PageTable[i + 15] = Page(i + 15);

		}
		//need to intialize that stuff
		LRUTLB LRUTLBTable = LRUTLB();
		LRUSize FrameLRU = LRUSize();
		Statistics Stats;
		ifstream Addresses;
		fstream BackStore(args[2], ios::out | ios::in | ios::binary);
		Addresses.open(args[1]);
		if (Addresses.is_open()) {
			string line;
			int upper;
			int lower;
			int address; //virtual address
			char command;//command
			bool redo = 0; //used if there is a page fault
			//if redo is true getline with be bypassed until it is false
			while (redo || getline(Addresses, line)) {
				Frame* frame = nullptr;
				Stats.NumberOfCalls++;
				address = atoi(line.substr(0, strlen(line.c_str()) - 2).c_str());
				command = line.substr(strlen(line.c_str()) - 1, 1)[0];
				upper = (address >> 8);
				lower = 255 & address;
				if (TLBHit(TLB, upper, frame)) {
					Stats.NumberOFTLBHits++;
					cout << "Virtual address: " << address
						<< " Physical address: " << frame->physical[lower]
						<< " Value: " << GetValue(frame->Bits[lower]) << endl;
					IsDirty(TLB, PageTable, command, frame);
					//v--- makes sure that a frame in the tlb doesnt get paged out
					FrameLRU.pick(frame->Title);
					LRUTLBTable.pick(frame->Title);
					redo = 0;
				}
				else if (PageHit(PageTable, upper, frame)) {

					cout << "Virtual address: " << address
						<< " Physical address: " << frame->physical[lower]
						<< " Value: " << GetValue(frame->Bits[lower]) << endl;
					FrameLRU.pick(frame->Title);
					//add the page to the tlb since it wasnt there
					int TLBTablePage = LRUTLBTable.pop();
					for (int i = 0; i < TLBSIZE; i++)
						if (TLB[i].Title == TLBTablePage)
						{
							TLB[i].Title = PageTable[upper].Title;
							TLB[i].frame = PageTable[upper].frame;
							LRUTLBTable.titles[0] = PageTable[upper].Title;
							break;
						}
					//-----------------------------------------------
					redo = 0;
				}
				else {
					//if the program reaches this point then the program has faulted
					Stats.NumberOFPageFaults++;
					//lru frame
					int VictimTitle = FrameLRU.pop();
					int i = 0, j = 0, k = 0, l = 0;
					//page out victim frame
					for (; i < PHYICALSIZE; i++)
						if (PhysicalMemory[i].Title == VictimTitle)
							break;

					//change page of victim invalid
					for (; j < SIZE; j++)
						if (PageTable[j].frame == &PhysicalMemory[i]) {
							PageTable[j].frame = nullptr;
							PageTable[j].Valid = false;
							break;
						}


					//page in desired page
					BackStore.seekg(streampos(upper << 8), ios::beg);
					while (k <= 255)
					{
						BackStore.read(&PhysicalMemory[i].Bits[k], 1);
						k++;
					}
					//set page back for new frame
					PageTable[upper].frame = &PhysicalMemory[i];
					PageTable[upper].Valid = true;
					//reset process
					redo = 1;
				}
			}

		}
		cout << "Statistics\r\n" <<
			"Page Fault Rate - " << setprecision(4) << (Stats.NumberOFPageFaults / Stats.NumberOfCalls) * 100 << "%\r\n" <<
			"TLB Hit RAte    - " << setprecision(4) << (Stats.NumberOFTLBHits / Stats.NumberOfCalls) * 100 << "%\r\n";
		Addresses.close();
		BackStore.close();
		return 0;
	}
	cout << "you need to give me an address.txt" << endl;
	return 0;
}