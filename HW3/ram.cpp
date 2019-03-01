
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

class Page {
public:
	int Title;
	bool Valid;
	Frame* frame;
	Page() {};
	Page(int title) {
		Title = title;
		Valid = 0;
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

bool PageHit(Page *Page, int pagenumber, Frame *frame) {
	if (Page[pagenumber].Valid)
	{
		frame = Page[pagenumber].frame;
		return true;
	}
	return false;
}



int main(int argc, char* args[]){
	if (sizeof(args)> 0) {
		Frame PhysicalMemory[SIZE];
		Page TLB[TLBSIZE];
		Page PageTable[SIZE];
		//need to intialize that stuff
		Page* LRUPageTable[SIZE];
		Page* LRUTLBTable[TLBSIZE];
		Frame* LRUFrame[SIZE];
		Statistics Stats;
		ifstream Addresses;
		ifstream BackStore;
		string la = args[1];
		Addresses.open(args[1]);
		BackStore.open("some addres", ios::binary | ios::in);
		if (Addresses.is_open()) {
			string line;
			int upper;
			int lower;
			int address;
			char command;
			while(getline(Addresses, line)) {
				Stats.NumberOfCalls++;
				address = atoi(line.substr(0, strlen(line.c_str()) - 2).c_str());
				command = line.substr(strlen(line.c_str()) - 1,1)[0];
				upper = address >> 8;
				lower = 255 & address;
				Frame* frame;
				cout <<"address: "<<address<<" command: " << command <<" upper: "<<upper<<" lower: "<<lower;
				if (TLBHit(TLB,upper, frame) || PageHit(PageTable, upper, frame)){
					Stats.NumberOFTLBHits++;
					/*
					cout << " value: " << GetValue(frame->Bits[lower]);
					if(command == "W")
						frame->DirtyBit = 1;
						PutFrameAtTop(LRUFrame,frame,SIZE)

					*/
				}
				else {

					// do somethine
				}


			}
					
		}
		Addresses.close();
		return 0;
	}
	cout << "you need to give me an address.txt" << endl;
	return 0;
}
