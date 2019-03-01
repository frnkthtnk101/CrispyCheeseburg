#include <pch.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int const SIZE = 255;
int const TLBSIZE = 16;

class Frame {
public:
	int Title;
	bool DirtyBit;
	char Bits[SIZE];
	Frame() {};
	Frame(int title) {
		this->Title = title;
		this->DirtyBit = 0;
		for (int i = 0; i < 255; i += 15) {
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

		}
	}
};
class LRUSize {
public:
	int titles[SIZE];

	LRUSize() {
		for (int i = 254; i >= 0; i--)
			titles[i] = 254 - i;
	}

	int pop() {
		int temp = titles[SIZE - 1];
		for (int i = 254; i > 1;)
		{
			int t = titles[--i];
			titles[i+1] = t ;
		}
		titles[0] = temp;
		return temp;
	}

	void pick(int title) {
		//int index = 0, temp = 0;
		int j = 0;
		for (; j < SIZE; j++)
			if (titles[j] == title)
				break;
		while (j > 1)
		{
			int t = titles[--j];
			titles[j + 1] = t;
		}
		titles[0] = title;
	}
};

class Page {
public:
	int Title;
	bool Valid;
	Frame* frame;
	Page() {};
	Page(int title) {
		Title = title;
		Valid = 0;
		frame = nullptr;
	}
};

class Statistics {
public:
	int NumberOfCalls, NumberOFTLBHits, NumberOFPageFaults;

	Statistics() {
		this->NumberOfCalls = 0;
		this->NumberOFTLBHits = 0;
		this->NumberOFPageFaults = 0;
	}
};

bool TLBHit(Page *TLB, int pagenumber, Frame* Framenumber) {
	for (int i = 0; i < TLBSIZE; i++)
		if (TLB[i].Title == pagenumber)
		{
			Framenumber = TLB[i].frame;
			return true;
		}
	return false;
}

bool PageHit(Page *Page, int pagenumber, int& frameIndex) {
	if (Page[pagenumber].Valid)
	{
		frameIndex = pagenumber;
		return true;
	}
	return false;
}

int GetValue(char value) {
	bool LastBit = value & 128;
	if (LastBit)
		return value & 127 - 128;
	return value;
};

int main(int argc, char* args[]) {
	if (sizeof(args) > 0) {
		Frame PhysicalMemory[SIZE];
		//Page TLB[TLBSIZE];
		Page PageTable[SIZE];
		for (int i = 0; i < SIZE; i += 15) {
			PhysicalMemory[i] = Frame(i);
			PhysicalMemory[i+1] = Frame(i+1);
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

		}
		//need to intialize that stuff
		LRUSize PageLRU = LRUSize();
		Page* LRUTLBTable[TLBSIZE];
		LRUSize FrameLRU = LRUSize();
		Statistics Stats;
		ifstream Addresses;
		ifstream BackStore;
		string la = args[1];
		Addresses.open(args[1]);
		BackStore.open("C:\\Users\\fpettigrosso\\ws\\CrispyCheeseburg\\HW3\\BACKING_STORE.bin", ios::binary | ios::in);
		if (Addresses.is_open()) {
			string line;
			int upper;
			int lower;
			int address;
			char command;
			while (getline(Addresses, line)) {
				int frame = 0;
				PROCESS: Stats.NumberOfCalls++;
				address = atoi(line.substr(0, strlen(line.c_str()) - 2).c_str());
				command = line.substr(strlen(line.c_str()) - 1, 1)[0];
				upper = address >> 8;
				lower = 255 & address;
				//TLBHit(TLB,upper, frame)
				if (PageHit(PageTable, upper, frame)) {
					Stats.NumberOFTLBHits++;
					cout << "address: " << address 
						<< " command: " << command << " upper: " 
						<< upper << " lower: " << lower << " value: " 
						<< GetValue(PhysicalMemory[frame].Bits[lower]) << endl;
					if (command == 'W')
						PhysicalMemory[frame].DirtyBit = 1;
					PageLRU.pick(upper);
					FrameLRU.pick(PhysicalMemory[frame].Title);

				}
				else {
					//lru frame
					int VictimTitle = FrameLRU.pop();
					int i = 0, j = 0, k = 0;
					//page out victim frame
					for (;i < SIZE; i++)
						if (PhysicalMemory[i].Title == VictimTitle)
							break;
					if (PhysicalMemory[i].DirtyBit == true) {
						//read out info to binar
						PhysicalMemory[i].DirtyBit = false;
					}		
					//change page of victim invalid
					for (; j < 255; j++)
						if (PageTable[j].frame == &PhysicalMemory[i]) {
							PageTable[j].frame = nullptr;
							PageTable[j].Valid = false;
							break;
						}
						
					//page in desired page
					BackStore.seekg(streampos(upper),ios::beg);
					//might have to read 255 once to bits
					while (k < SIZE)
					{
						char* t = &PhysicalMemory[i].Bits[k];
						BackStore.read(t, 1);
						k++;
					}
					//set page back for new frame
					PageTable[upper].frame = &PhysicalMemory[i];
					PageTable[upper].Valid = true;
					//reset process
					goto PROCESS;
				}

					
				


			}

		}
		Addresses.close();
		return 0;
	}
	cout << "you need to give me an address.txt" << endl;
	return 0;
}