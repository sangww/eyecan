#ifndef _FPTR_OBJ
#define _FPTR_OBJ

#include <vector>

typedef unsigned long       DWORD;
typedef void (*fptr)(DWORD c);

class fptrObject {

	public:
		fptrObject();
		~fptrObject();
		
		std::vector<fptr> f;
		std::vector<int> a;
		std::vector<bool> n; //automatically call next?
		std::vector<bool> r; //automatically repeat this command?

		void call();
		void callRepeater();
		void reset();
		void attach(fptr fp, DWORD ch, bool next, bool repeat);
		void attach(fptr fp, DWORD ch, bool next);
		void attach(fptr fp, DWORD ch);
		void attach(fptr fp, bool next);
		void attach(fptr fp);
		void attachNull();

		int curIndex;
		int repIndex;
};

#endif
