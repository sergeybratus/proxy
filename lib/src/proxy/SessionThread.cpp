

#include "SessionThread.h"

namespace proxy
{

SessionThread::SessionThread(const ProxyConfig& config, FileDesc& server_fd, IParserFactory& factory) :
    m_session(config, server_fd, factory),
    m_thread([this](){ this->Run(); })
{

}

void SessionThread::Run()
{


    //m_session.Run();

    // TODO - Add callbacks for the end of the thread
}

}


