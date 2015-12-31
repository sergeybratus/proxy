#include "dnp3cpp/DNP3Factory.h"

#include "dnp3cpp/DNP3Parser.h"
#include <easylogging++.h>

namespace proxy { namespace  dnp3 {

DNP3Factory::DNP3Factory(HAllocator *mm_input_,
                         HAllocator *mm_parse_,
                         HAllocator *mm_context_,
                         HAllocator *mm_results_) :
                                mm_input(mm_input_),
                                mm_parse(mm_parse_),
                                mm_context(mm_context_),
                                mm_results(mm_results_)
{

}

std::unique_ptr<IParser> DNP3Factory::Create(SessionDir dir, IParserCallbacks& callbacks)
{
    return std::unique_ptr<IParser>(new DNP3Parser(mm_input, mm_parse, mm_context, mm_results, dir, callbacks));
}

}}
