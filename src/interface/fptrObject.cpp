#include "fptrObject.h"


fptrObject::fptrObject(){
	curIndex=0;
	repIndex=-1;
}
fptrObject::~fptrObject(){	
}

void fptrObject::call()
{
	if(r.size()>0)
	{
		if(r[curIndex])
		{
			repIndex=curIndex;
		}
		else
		{
			repIndex=-1;
		}
	}
	//printf("fptrCall %d %d\n",curIndex,f.size());	
	for(int i=curIndex;i<f.size();i++)
	{
		if(f[i]!=0)
			f[i](a[i]);
		curIndex=i+1;
		if(curIndex>=f.size())
		{
			curIndex=0;
		}
		else
		{
			if(!n[i]) break;
		}
	}
}
void fptrObject::callRepeater() //how about multi action?
{
	if(repIndex<0) return;
	if(f[repIndex]!=0)
		f[repIndex](a[repIndex]);
}

void fptrObject::reset()
{
	f.clear();
	a.clear();
	n.clear();
	r.clear();
	curIndex=0;
	repIndex=-1;
}

void fptrObject::attach(fptr fp, DWORD ch, bool next, bool repeat)
{
	f.push_back(fp);
	a.push_back(ch);
	n.push_back(next);
	r.push_back(repeat);
}
void fptrObject::attach(fptr fp, DWORD ch, bool next)
{
	attach(fp,ch,next,false);
}
void fptrObject::attach(fptr fp, DWORD ch)
{
	attach(fp,ch,true);
}
void fptrObject::attach(fptr fp, bool next)
{
	attach(fp,0,next);
}
void fptrObject::attach(fptr fp)
{
	attach(fp,0,true);
}
void fptrObject::attachNull()
{
	attach(0,0,true);
}



