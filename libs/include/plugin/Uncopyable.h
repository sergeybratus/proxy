
#ifndef PROXY_UNCOPYABLE_H
#define PROXY_UNCOPYABLE_H

namespace proxy
{
  
class Uncopyable
{
  
protected:
	Uncopyable() {} //allow construction/destruction
	~Uncopyable() {}

private:
	// prevent these functions
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator=(const Uncopyable&) = delete;
};
      
}

#endif
