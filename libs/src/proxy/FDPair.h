

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
  
  FDInfo() : m_state(FDState::FDState::CLOSED), m_fid(-1)
  {}
    
  FDState m_state;
  int m_fid;
  
};
  
/**
 * Represents a pair of file descriptors for connecting and listening sockets
 */  
class FDPair
{
  
public:
  
  
  
  
};
  
  
}