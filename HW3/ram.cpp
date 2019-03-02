
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>
using namespace std;

int const SIZE = 256;
int const TLBSIZE = 16;

class Frame {
public:
	int Title;
	bool DirtyBit;
	char Bits[SIZE];
	int physical[SIZE];
	Frame() {};
	Frame(int title) {
		this->Title = title;
		this->DirtyBit = 0;
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
class LRUSize {
public:
	int titles[SIZE];

	LRUSize() {
		for (int i = 255; i >= 0; i--)
			titles[i] = 255 - i;
	}

	int pop() {
		int temp = titles[SIZE - 1];
		for (int i = 255; i > 0;)
		{
			int t = titles[--i];
			titles[i + 1] = t;
		}
		titles[0] = temp;
		return temp;
	}

	void pick(int title) {
		int j = 0;
		for (; j < SIZE; j++)
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
class LRUTLB {
public:
	int titles[TLBSIZE];

	LRUTLB() {
		titles[0] = -1;
		titles[1] = -1;
		titles[2] = -1;
		titles[3] = -1;
		titles[4] = -1;
		titles[5] = -1;
		titles[6] = -1;
		titles[7] = -1;
		titles[8] = -1;
		titles[9] = -1;
		titles[10] = -1;
		titles[11] = -1;
		titles[12] = -1;
		titles[13] = -1;
		titles[14] = -1;
		titles[15] = -1;
	}

	int pop() {
		int temp = titles[TLBSIZE - 1];
		for (int i = 15; i > 0;)
		{
			int t = titles[--i];
			titles[i + 1] = t;
		}
		titles[0] = temp;
		return temp;
	}

	void pick(int title) {
		int j = 0;
		for (; j < TLBSIZE; j++)
			if (titles[j] == title)
				break;
		while (j > 0)
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
	double NumberOfCalls, NumberOFTLBHits, NumberOFPageFaults;

	Statistics() {
		this->NumberOfCalls = 0;
		this->NumberOFTLBHits = 0;
		this->NumberOFPageFaults = 0;
	}
};

bool TLBHit(Page *TLB, int pagenumber, Frame*& Framenumber) {
	for (int i = 0; i < TLBSIZE; i++)
		if (TLB[i].Title == pagenumber)
		{
			Framenumber = TLB[i].frame;
			return true;
		}
	return false;
}

bool PageHit(Page *Page, int pagenumber, Frame*& frameIndex) {
	if (Page[pagenumber].Valid)
	{
		frameIndex = Page[pagenumber].frame;
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
		Page TLB[TLBSIZE];
		Page PageTable[SIZE];
		for (int i = 0; i < TLBSIZE; i += 2)
		{
			TLB[i] = Page(-1);
			TLB[i + 1] = Page(-1);
		}
		for (int i = 0; i < SIZE; i += 16) {
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
		fstream BackStore("/home/oscreader/Desktop/osc9e-src/ch9/BACKING_STORE.bin", ios::out | ios::in | ios::binary);
		Addresses.open(args[1]);
		if (Addresses.is_open()) {
			string line;
			int upper;
			int lower;
			int address;
			char command;
			bool redo = 0;
			while (redo || getline(Addresses, line)) {
				Frame* frame = nullptr;
				Stats.NumberOfCalls++;
				address = atoi(line.substr(0, strlen(line.c_str()) - 2).c_str());
				command = line.substr(strlen(line.c_str()) - 1, 1)[0];
				upper = (address >> 8);
				lower = 255 & address;
				if (TLBHit(TLB, upper, frame)) {
					Stats.NumberOFTLBHits++;
					cout << "Virtual Address: " << address
						<< " command: " << command
						<< " Physical Address: " << frame->physical[lower]
						<< " value: " << GetValue(frame->Bits[lower]) << " tlb hit" << endl;
					if (command == 'W')
						frame->DirtyBit = 1;
					FrameLRU.pick(frame->Title);
					LRUTLBTable.pick(frame->Title);
					redo = 0;
				}
				else if (PageHit(PageTable, upper, frame)) {

					cout << "Virtual Address: " << address
						<< " command: " << command
						<< " Physical Address: " << frame->physical[lower]
						<< " value: " << GetValue(frame->Bits[lower]) << endl;
					if (command == 'W')
						frame->DirtyBit = 1;
					FrameLRU.pick(frame->Title);
					//add the page to the tlb sinc it wasnt there
					int TLBTablePage = LRUTLBTable.pop();
					for (int i = 0; i < TLBSIZE; i++)
						if (TLB[i].Title == TLBTablePage)
						{
							TLB[i].Title = PageTable[upper].Title;
							TLB[i].frame = PageTable[upper].frame;
							LRUTLBTable.titles[0] = PageTable[upper].Title;
							break;
						}


					redo = 0;

				}
				else {
					Stats.NumberOFPageFaults++;
					//lru frame
					int VictimTitle = FrameLRU.pop();
					int i = 0, j = 0, k = 0;
					//page out victim frame
					for (; i < SIZE; i++)
						if (PhysicalMemory[i].Title == VictimTitle)
							break;
					if (PhysicalMemory[i].DirtyBit == true) {
						BackStore.seekg(streampos(upper << 8), ios::beg);
						while (k < 255) {
							BackStore.write(&PhysicalMemory[i].Bits[k], 1);
							k++;
						}
						k = 0;
						PhysicalMemory[i].DirtyBit = false;
					}
					//change page of victim invalid
					for (; j < SIZE; j++)
						if (PageTable[j].frame == &PhysicalMemory[i]) {
							PageTable[j].frame = nullptr;
							PageTable[j].Valid = false;
							break;
						}

					//page in desired page
					int t = upper << 8;
				BackStore.seekg(streampos(t), ios::beg);
					//might have to read 255 once to bits
					while (k < 255)
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
