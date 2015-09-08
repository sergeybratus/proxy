
#include <cstdint>

namespace proxy
{
  
enum class FDState : uint8_t
{
  CLOSED,
  OPEN  
};
  
class FDInfo
{

public:
  
  FDInfo();
  
    
  FDState state;
  int fid;  
};
  
/**
 * Represents a pair of file descriptors for connecting and listening sockets
 */  
class FDPair
{
  
public:
  
  
  
  FDInfo listening;
  FDInfo connecting;
    
};
  
  
}