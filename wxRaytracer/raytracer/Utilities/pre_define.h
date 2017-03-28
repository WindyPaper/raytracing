#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#define SAFE_DELETE(p) \
if (p) \
{ \
	delete p; \
	p = 0; \
} 

#endif