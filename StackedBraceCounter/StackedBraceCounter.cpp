// SimpleCompiler.cpp : Defines the entry point for the console application.

bool read_cmd_line(int argc, char** argv, std::string& source_file)
{
    auto print_usage = []() { std::cout << "USAGE: The only parameter is the text source file path." << std::endl; };

    if (argc < 2)
        std::cout << "[ERROR] Not enough parameters!" << std::endl, print_usage(), exit(1);

    if (argc > 2)
        std::cout << "[ERROR] Too many parameters!" << std::endl, print_usage(), exit(1);

    std::ifstream src(argv[1]);
    if (!src.is_open())
        std::cout << "[ERROR] Check does the file exist." << std::endl, print_usage(), exit(1);
    else
        src.close();

    source_file = argv[1];

    return true;
}

const std::vector<std::string> test
{
    "{{{}}}",
    "{{{}}}}",
    "{{{{}}}",
    "{{{",
    "}}}",
    "{{}}{",
};

// "state_1"
// stack symbol:    |                 {                    |      (empty)
//------------------*--------------------------------------*-------------------------
// input symbol:    |                                      |
//       {          |       write {, keep "state_1"        |  write {, keep "state_1"
//       }          |  write nothing, swtich to "state_2"  |  invalid string (1)
//      EOF         |        invalid string (2)            |  correct string (3)
//
//
// "state_2"
// stack symbol:    |                 {                    |      (empty)
//------------------*--------------------------------------*-------------------------
// input symbol:    |                                      |
//       {          |        invalid string (4)            |  invalid string (4)
//       }          |     write nothing, keep "state_2"    |  invalid string (5)
//      EOF         |        invalid string (6)            |  correct string (7)

// 1. string starts with }
// 2. string has few { in the beginning, but no } after
// 3. string is empty
// 4. string has { after }
// 5. number of } is greater then number of { (stack is empty but input is not)
// 6. number of } less then number of { (input is empty but stack is not)
// 7. number of { equals to number of } (stack and the input emptied at the same tyme)

class state_machine
{
/*
    state_name
                incoming_symbol,stack_symbol
                            new_state, action
*/
    enum class state_t
    {
        state_1,
        state_2,
    };

    enum class error_t
    {
        failure,
        need_more,
        success,
    };

    enum class ascii
    {
        h00 = 0x00,
        h01 = 0x00,
        h02 = 0x00,
        h03 = 0x00,
        h04 = 0x00,
        h05 = 0x00,
        h06 = 0x00,
        h07 = 0x00,
        h08 = 0x00,
        h09 = 0x00,
        h0A = 0x00,
        h0B = 0x00,
        h0C = 0x00,
        h0D = 0x00,
        h0E = 0x00,
        h0F = 0x00,

        h10 = 0x00,
        h11 = 0x00,
        h12 = 0x00,
        h13 = 0x00,
        h14 = 0x00,
        h15 = 0x00,
        h16 = 0x00,
        h17 = 0x00,
        h18 = 0x00,
        h19 = 0x00,
        h1A = 0x00,
        h1B = 0x00,
        h1C = 0x00,
        h1D = 0x00,
        h1E = 0x00,
        h1F = 0x00,

        h20 = 0x00,
        h21 = 0x00,
        h22 = 0x00,
        h23 = 0x00,
        h24 = 0x00,
        h25 = 0x00,
        h26 = 0x00,
        h27 = 0x00,
        h28 = 0x00,
        h29 = 0x00,
        h2A = 0x00,
        h2B = 0x00,
        h2C = 0x00,
        h2D = 0x00,
        h2E = 0x00,
        h2F = 0x00,
        
        h30 = 0x00,
        h31 = 0x00,
        h32 = 0x00,
        h33 = 0x00,
        h34 = 0x00,
        h35 = 0x00,
        h36 = 0x00,
        h37 = 0x00,
        h38 = 0x00,
        h39 = 0x00,
        h3A = 0x00,
        h3B = 0x00,
        h3C = 0x00,
        h3D = 0x00,
        h3E = 0x00,
        h3F = 0x00,
        
        h40 = 0x00,
        h41 = 0x00,
        h42 = 0x00,
        h43 = 0x00,
        h44 = 0x00,
        h45 = 0x00,
        h46 = 0x00,
        h47 = 0x00,
        h48 = 0x00,
        h49 = 0x00,
        h4A = 0x00,
        h4B = 0x00,
        h4C = 0x00,
        h4D = 0x00,
        h4E = 0x00,
        h4F = 0x00,
        
        h50 = 0x00,
        h51 = 0x00,
        h52 = 0x00,
        h53 = 0x00,
        h54 = 0x00,
        h55 = 0x00,
        h56 = 0x00,
        h57 = 0x00,
        h58 = 0x00,
        h59 = 0x00,
        h5A = 0x00,
        h5B = 0x00,
        h5C = 0x00,
        h5D = 0x00,
        h5E = 0x00,
        h5F = 0x00,
        
        h60 = 0x00,
        h61 = 0x00,
        h62 = 0x00,
        h63 = 0x00,
        h64 = 0x00,
        h65 = 0x00,
        h66 = 0x00,
        h67 = 0x00,
        h68 = 0x00,
        h69 = 0x00,
        h6A = 0x00,
        h6B = 0x00,
        h6C = 0x00,
        h6D = 0x00,
        h6E = 0x00,
        h6F = 0x00,

        h70 = 0x00,
        h71 = 0x00,
        h72 = 0x00,
        h73 = 0x00,
        h74 = 0x00,
        h75 = 0x00,
        h76 = 0x00,
        h77 = 0x00,
        h78 = 0x00,
        h79 = 0x00,
        h7A = 0x00,
        h7B = 0x00,
        h7C = 0x00,
        h7D = 0x00,
        h7E = 0x00,
        h7F = 0x00,
        
        h80 = 0x00,
        h81 = 0x00,
        h82 = 0x00,
        h83 = 0x00,
        h84 = 0x00,
        h85 = 0x00,
        h86 = 0x00,
        h87 = 0x00,
        h88 = 0x00,
        h89 = 0x00,
        h8A = 0x00,
        h8B = 0x00,
        h8C = 0x00,
        h8D = 0x00,
        h8E = 0x00,
        h8F = 0x00,

        h90 = 0x00,
        h91 = 0x00,
        h92 = 0x00,
        h93 = 0x00,
        h94 = 0x00,
        h95 = 0x00,
        h96 = 0x00,
        h97 = 0x00,
        h98 = 0x00,
        h99 = 0x00,
        h9A = 0x00,
        h9B = 0x00,
        h9C = 0x00,
        h9D = 0x00,
        h9E = 0x00,
        h9F = 0x00,
        
        hA0 = 0x00,
        hA1 = 0x00,
        hA2 = 0x00,
        hA3 = 0x00,
        hA4 = 0x00,
        hA5 = 0x00,
        hA6 = 0x00,
        hA7 = 0x00,
        hA8 = 0x00,
        hA9 = 0x00,
        hAA = 0x00,
        hAB = 0x00,
        hAC = 0x00,
        hAD = 0x00,
        hAE = 0x00,
        hAF = 0x00,

        hB0 = 0x00,
        hB1 = 0x00,
        hB2 = 0x00,
        hB3 = 0x00,
        hB4 = 0x00,
        hB5 = 0x00,
        hB6 = 0x00,
        hB7 = 0x00,
        hB8 = 0x00,
        hB9 = 0x00,
        hBA = 0x00,
        hBB = 0x00,
        hBC = 0x00,
        hBD = 0x00,
        hBE = 0x00,
        hBF = 0x00,

        hC0 = 0x00,
        hC1 = 0x00,
        hC2 = 0x00,
        hC3 = 0x00,
        hC4 = 0x00,
        hC5 = 0x00,
        hC6 = 0x00,
        hC7 = 0x00,
        hC8 = 0x00,
        hC9 = 0x00,
        hCA = 0x00,
        hCB = 0x00,
        hCC = 0x00,
        hCD = 0x00,
        hCE = 0x00,
        hCF = 0x00,

        hD0 = 0x00,
        hD1 = 0x00,
        hD2 = 0x00,
        hD3 = 0x00,
        hD4 = 0x00,
        hD5 = 0x00,
        hD6 = 0x00,
        hD7 = 0x00,
        hD8 = 0x00,
        hD9 = 0x00,
        hDA = 0x00,
        hDB = 0x00,
        hDC = 0x00,
        hDD = 0x00,
        hDE = 0x00,
        hDF = 0x00,

        hE0 = 0x00,
        hE1 = 0x00,
        hE2 = 0x00,
        hE3 = 0x00,
        hE4 = 0x00,
        hE5 = 0x00,
        hE6 = 0x00,
        hE7 = 0x00,
        hE8 = 0x00,
        hE9 = 0x00,
        hEA = 0x00,
        hEB = 0x00,
        hEC = 0x00,
        hED = 0x00,
        hEE = 0x00,
        hEF = 0x00,

        hF0 = 0x00,
        hF1 = 0x00,
        hF2 = 0x00,
        hF3 = 0x00,
        hF4 = 0x00,
        hF5 = 0x00,
        hF6 = 0x00,
        hF7 = 0x00,
        hF8 = 0x00,
        hF9 = 0x00,
        hFA = 0x00,
        hFB = 0x00,
        hFC = 0x00,
        hFD = 0x00,
        hFE = 0x00,
        hFF = 0x00,
    };

    using action_t              = std::function<bool(const char&)>;
    using input_stack_state_t   = std::pair<char, char>;
    using state_action_t        = std::pair<state_t, action_t>;
    using state_table           = std::map<state_t, std::map<input_stack_state_t, state_action_t>>;

public:
    state_machine()
        : m_state_table
    { 
        { state_t::state_1,
            { 
                {},{} 
            } 
        },
        { state_t::state_2,
            { 
                {},{} 
            } 
        },
    }
    {};

protected:
    const state_table m_state_table;

    bool action_push(const char& c); // write symbol to stack
    bool action_pop(const char& c);  // read symbol from stack
    bool action_done(const char& c); // return result done
};

int main(int argc, char** argv)
{
    std::cout << "Hello beautiful world!" << std::endl;

    return 0;
}

