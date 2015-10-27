#include <iostream>

extern "C"
{
    #include <dnp3.h>
}

int main (int argc, char *argv[])
{
    const size_t MAX_BYTES = 4096;

    // TODO - can the malloc's here every be reclaimed?
    dnp3_p_init();

    // read input from std-in until EOF
    uint8_t buffer[MAX_BYTES];

    //try to read from stdin
    const size_t NUM_READ = fread(buffer, 1, MAX_BYTES, stdin);

    std::cout << "Processing " << NUM_READ << " bytes" << std::endl;

    auto parser = dnp3_p_app_request;

    HParseResult *res = h_parse(parser, buffer, NUM_READ);
    if (!res) {
      return -1;
    }

    h_delete_arena(res->arena);

    return 0;
}

