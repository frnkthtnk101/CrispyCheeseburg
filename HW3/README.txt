HW3 - memory management
Franco Pettigrosso

TOB
-what everything does
-building
-how to run
-francos checklist

What everything does
address2.txt - just a copy of what you gave us for the homework, used for testing.
BACKING_STORE.bin - just a copy of the backing store, used for testing.
correct.txt - just a copy of the correct from the book, used for testing.
fpettigrosso_hw3_questions.docx - my answered question and stats. the stats are on the last page.
Makefile - used to make ram1 ram2 ram3.
ram1.cpp - what was orginally asked to make.
ram2.cpp - what was originally asked to make plus modifications.
ram3.cpp - ram2.cpp + r/w capabilites.
Readme.txt - what you are reading right now.
resultsTesting.test.ps1 - powershell script used for testing.
fpettigrosso_hw3_puedo.pdf - my scraps and psuedo

-building
the final product is ram3. just do make ram3. the ps1 will no run on the debian image unless you install powershell6.
that ps1 file is not used for anything other than tesing purposes.

-how to run
./ram3 <path to address2.txt> <path to BACKING_STORE.bin>
this will print out the addresses values and stats. matter of fact all the rams do that.
francos checklist
well commented code - commented to the best of my abilities
Readme done
the question and answers and stats are in fpettigrosso_hw3_questions.docx
all my scraps and psuedo are there. The read write algorithm in on the second pafe
