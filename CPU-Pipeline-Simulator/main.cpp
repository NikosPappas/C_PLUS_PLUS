#include <iostream>
#include <vector>
#include <map>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <iomanip>

// --- I. Definitions: Opcodes and Registers ---

// Define the Opcode for our simplified Instruction Set Architecture (ISA)
enum Opcode : uint8_t {
    OP_HALT  = 0x0, // 0000: Stop execution
    OP_LOAD  = 0x1, // 0001: Load value from memory [Address] into Rd
    OP_STORE = 0x2, // 0010: Store register value Ra into memory [Address]
    OP_ADD   = 0x3, // 0011: Add Ra and Rb, store result in Rd
    OP_JUMP  = 0x4, // 0100: Unconditional Jump to Address (PC = Address)
    OP_MOVI  = 0x5, // 0101: Move Immediate (16-bit) value into Rd
    OP_SUB   = 0x6  // 0110: Subtract Ra and Rb, store result in Rd
};

// Define the 8 General Purpose Registers (GPRs) in x86 naming convention
enum class RegisterName : uint8_t {
    EAX = 0, // Accumulator
    EBX = 1, // Base
    ECX = 2, // Counter
    EDX = 3, // Data
    ESI = 4, // Source Index
    EDI = 5, // Destination Index
    EBP = 6, // Base Pointer
    ESP = 7  // Stack Pointer
};

// --- II. Data Structures ---

// Structure to hold the status flags (part of the EFLAGS register in real x86)
struct Flags {
    bool ZF = false; // Zero Flag (Set if result is zero)
    bool SF = false; // Sign Flag (Set if result is negative, MSB is 1)
    bool CF = false; // Carry Flag (Set if an unsigned overflow occurs)
    bool OF = false; // Overflow Flag (Set if a signed overflow occurs)
};

// Structure to hold the components of an instruction after decoding
struct DecodedInstruction {
    Opcode opcode;
    RegisterName dest_reg;
    RegisterName src1_reg;
    RegisterName src2_reg;
    uint32_t immediate_or_address; // 16-bit immediate value or memory address offset
};

// Class to manage the General Purpose Registers (GPRs)
class Registers {
public:
    // Map to hold 32-bit registers, accessible by RegisterName
    std::map<RegisterName, uint32_t> gpr;

    Registers() {
        // Initialize all GPRs to zero
        gpr[RegisterName::EAX] = 0;
        gpr[RegisterName::EBX] = 0;
        gpr[RegisterName::ECX] = 0;
        gpr[RegisterName::EDX] = 0;
        gpr[RegisterName::ESI] = 0;
        gpr[RegisterName::EDI] = 0;
        gpr[RegisterName::EBP] = 0;
        gpr[RegisterName::ESP] = 0;
    }

    // Reset all registers to zero for a new test run
    void reset() {
        for (auto& pair : gpr) {
            pair.second = 0;
        }
    }
};

// Class to simulate the main memory
class Memory {
private:
    // 2^24 words = 16M words = 64 MB of addressable memory
    static constexpr uint32_t MAX_MEMORY_WORDS = 1 << 24;
    // Memory stores 32-bit words
    std::vector<uint32_t> data;

public:
    Memory() {
        data.resize(MAX_MEMORY_WORDS, 0);
        std::cout << "Memory initialized: " << (MAX_MEMORY_WORDS * 4) / (1024 * 1024)
                  << " MB of memory.\n";
    }

    // Reset all memory words to zero
    void reset() {
        std::fill(data.begin(), data.end(), 0);
    }

    // Read a 32-bit word from a given word address
    uint32_t read_word(uint32_t address) const {
        if (address >= MAX_MEMORY_WORDS) {
            throw std::out_of_range("Memory read access violation: Address out of bounds.");
        }
        return data[address];
    }

    // Write a 32-bit word to a given word address
    void write_word(uint32_t address, uint32_t value) {
        if (address >= MAX_MEMORY_WORDS) {
            throw std::out_of_range("Memory write access violation: Address out of bounds.");
        }
        data[address] = value;
    }

    // Load a program (list of instructions) into memory
    void load_program(const std::vector<uint32_t>& program, uint32_t start_address) {
        if (start_address + program.size() >= MAX_MEMORY_WORDS) {
             throw std::out_of_range("Program too large to load at specified starting address.");
        }
        for (size_t i = 0; i < program.size(); ++i) {
            write_word(start_address + i, program[i]);
        }
        std::cout << "Program loaded successfully starting at word address 0x"
                  << std::hex << start_address << std::dec << ".\n";
    }
};

// --- III. Helper Functions ---

// Maps a 4-bit index (0-7) to a RegisterName enum
RegisterName index_to_reg(uint8_t index) {
    if (index > 7) return RegisterName::EAX; // Default to EAX for safety
    return static_cast<RegisterName>(index);
}

// Maps a RegisterName enum to its string representation
std::string reg_to_string(RegisterName reg) {
    switch (reg) {
        case RegisterName::EAX: return "EAX";
        case RegisterName::EBX: return "EBX";
        case RegisterName::ECX: return "ECX";
        case RegisterName::EDX: return "EDX";
        case RegisterName::ESI: return "ESI";
        case RegisterName::EDI: return "EDI";
        case RegisterName::EBP: return "EBP";
        case RegisterName::ESP: return "ESP";
    }
    return "Unknown";
}

// --- IV. CPU Class Implementation (The Core) ---

class CPU {
private:
    Registers gprs;       // General Purpose Registers
    Flags flags;          // Status Flags
    uint32_t pc;          // Program Counter (word address of next instruction)
    uint32_t ir;          // Instruction Register (current 32-bit instruction)
    DecodedInstruction current_instruction; // Decoded instruction parts
    bool halted;          // State flag for HALT instruction

public:
    CPU() : pc(0), ir(0), halted(false) {
        std::cout << "CPU initialized.\n";
    }

    // Reset CPU state for a new test
    void reset() {
        gprs.reset();
        pc = 0;
        ir = 0;
        halted = false;
        flags = Flags(); // Reset flags
        std::cout << "\nCPU State Reset for new test.\n";
    }

    // Public Getter for a register's value (for testing/verification)
    uint32_t get_register_value(RegisterName reg) const {
        // Use .at() for bounds checking, although all GPRs should exist.
        return gprs.gpr.at(reg);
    }

    // Public Getter for the flags structure (for testing/verification)
    Flags get_flags() const {
        return flags;
    }

    // The core execution loop
    void run(Memory& mem) {
        std::cout << "\n--- Starting CPU Execution (Fetch-Decode-Execute Cycle) ---\n";

        while (!halted) {
            // 1. FETCH: Read instruction from memory
            fetch(mem);
            if (halted) break;

            // 2. DECODE: Extract fields from the instruction
            decode();

            // 3. EXECUTE: Perform the operation
            execute(mem);

            std::cout << "--------------------------------------------------------\n";
        }

        std::cout << "\n--- CPU Halted ---\n";
        display_state();
    }

    // 1. FETCH Stage: Get instruction and increment PC
    void fetch(const Memory& mem) {
        if (halted) return;

        try {
            // Read the instruction from memory at the address stored in PC
            ir = mem.read_word(pc);

            // Increment PC to point to the next instruction (word address increment)
            pc++;

            std::cout << "[FETCH] PC: 0x" << std::hex << (pc - 1) * 4 // Byte address display
                      << " | IR (Instruction): 0x" << ir << std::dec << "\n";

        } catch (const std::out_of_range& e) {
            std::cerr << "FATAL ERROR during FETCH: " << e.what() << "\n";
            halted = true;
        }
    }

    // 2. DECODE Stage: Parse the instruction into fields
    // Instruction Format (32-bit): [Opcode: 4] [Rd: 4] [Ra: 4] [Rb: 4] [Immediate/Addr: 16]
    void decode() {
        uint32_t instruction = ir;

        // 1. Opcode (Bits 31-28)
        current_instruction.opcode = static_cast<Opcode>((instruction >> 28) & 0xF);

        // 2. Destination Register (Rd) (Bits 27-24)
        uint8_t dest_idx = (instruction >> 24) & 0xF;
        current_instruction.dest_reg = index_to_reg(dest_idx);

        // 3. Source Register 1 (Ra) (Bits 23-20)
        uint8_t src1_idx = (instruction >> 20) & 0xF;
        current_instruction.src1_reg = index_to_reg(src1_idx);

        // 4. Source Register 2 (Rb) (Bits 19-16)
        uint8_t src2_idx = (instruction >> 16) & 0xF;
        current_instruction.src2_reg = index_to_reg(src2_idx);

        // 5. Immediate/Address (Bits 15-0)
        current_instruction.immediate_or_address = instruction & 0xFFFF;

        std::cout << "[DECODE] Opcode: 0x" << std::hex << (int)current_instruction.opcode
                  << " | Rd: " << reg_to_string(current_instruction.dest_reg)
                  << " | Ra: " << reg_to_string(current_instruction.src1_reg)
                  << " | Rb: " << reg_to_string(current_instruction.src2_reg)
                  << " | Imm/Addr: 0x" << current_instruction.immediate_or_address
                  << std::dec << "\n";
    }

    // 3. EXECUTE Stage: Perform the operation
    void execute(Memory& mem) {
        const DecodedInstruction& instr = current_instruction;
        std::cout << "[EXECUTE] ";

        // Macro to update simple flags after an arithmetic operation (ZF and SF)
        auto update_flags = [&](uint32_t result) {
            flags.ZF = (result == 0);
            // Sign Flag is based on the Most Significant Bit (MSB)
            flags.SF = (result & 0x80000000) != 0;
        };

        try {
            switch (instr.opcode) {
                case OP_HALT: {
                    halted = true;
                    std::cout << "HALT instruction executed. CPU stopped.\n";
                    break;
                }

                case OP_MOVI: {
                    // MOVI Rd, Immediate: Rd = Immediate_or_Address
                    uint32_t immediate = instr.immediate_or_address;
                    gprs.gpr[instr.dest_reg] = immediate;
                    update_flags(immediate);

                    std::cout << "MOVI " << reg_to_string(instr.dest_reg)
                              << ", 0x" << std::hex << immediate << std::dec << "\n";
                    break;
                }

                case OP_ADD: {
                    // ADD Rd, Ra, Rb: Rd = Ra + Rb
                    uint32_t val_a = gprs.gpr.at(instr.src1_reg);
                    uint32_t val_b = gprs.gpr.at(instr.src2_reg);
                    uint32_t result = val_a + val_b;

                    // Simplified flag updates (CF/OF logic is omitted for basic simulation)
                    flags.CF = (result < val_a); // Carry for unsigned overflow
                    update_flags(result);

                    gprs.gpr[instr.dest_reg] = result;
                    std::cout << "ADD " << reg_to_string(instr.dest_reg)
                              << ", " << reg_to_string(instr.src1_reg)
                              << ", " << reg_to_string(instr.src2_reg)
                              << " | Result: " << result << "\n";
                    break;
                }

                case OP_SUB: {
                    // SUB Rd, Ra, Rb: Rd = Ra - Rb
                    uint32_t val_a = gprs.gpr.at(instr.src1_reg);
                    uint32_t val_b = gprs.gpr.at(instr.src2_reg);
                    uint32_t result = val_a - val_b;

                    // Simplified flag updates (CF/OF logic is omitted for basic simulation)
                    flags.CF = (val_b > val_a); // Borrow flag / reverse carry
                    update_flags(result);

                    gprs.gpr[instr.dest_reg] = result;
                    std::cout << "SUB " << reg_to_string(instr.dest_reg)
                              << ", " << reg_to_string(instr.src1_reg)
                              << ", " << reg_to_string(instr.src2_reg)
                              << " | Result: " << result << "\n";
                    break;
                }

                case OP_LOAD: {
                    // LOAD Rd, [Address]: Rd = Mem[Address]
                    uint32_t address = instr.immediate_or_address;
                    uint32_t value = mem.read_word(address);
                    gprs.gpr[instr.dest_reg] = value;

                    std::cout << "LOAD " << reg_to_string(instr.dest_reg)
                              << ", [0x" << std::hex << address << std::dec
                              << "] | Value: " << value << "\n";
                    break;
                }

                case OP_STORE: {
                    // STORE [Address], Ra: Mem[Address] = Ra
                    uint32_t address = instr.immediate_or_address;
                    // The value to store comes from the Src1 register (Ra) field in the instruction
                    uint32_t value_to_store = gprs.gpr.at(instr.src1_reg);
                    mem.write_word(address, value_to_store);

                    std::cout << "STORE [0x" << std::hex << address << std::dec
                              << "], " << reg_to_string(instr.src1_reg)
                              << " | Stored Value: " << value_to_store << "\n";
                    break;
                }

                case OP_JUMP: {
                    // JUMP Address: PC = Address (16-bit field used as word address)
                    uint32_t target_address = instr.immediate_or_address;
                    pc = target_address;

                    std::cout << "JUMP 0x" << std::hex << target_address << std::dec
                              << " | PC set to word address 0x" << std::hex << target_address << std::dec << "\n";
                    break;
                }

                default: {
                    std::cerr << "Execution Error: Unknown Opcode 0x"
                              << std::hex << (int)instr.opcode << std::dec << ". Halting.\n";
                    halted = true;
                    break;
                }
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Execution Error: Memory/Register access violation. " << e.what() << "\n";
            halted = true;
        }
    }

    // Display the final state of the registers and flags
    void display_state() const {
        std::cout << "\n--- Final CPU State ---\n";
        std::cout << "PC: 0x" << std::hex << pc * 4 << " (Byte Address)\n";

        std::cout << "Registers (32-bit Hex):\n";
        std::vector<RegisterName> display_order = {
            RegisterName::EAX, RegisterName::EBX, RegisterName::ECX, RegisterName::EDX,
            RegisterName::ESI, RegisterName::EDI, RegisterName::EBP, RegisterName::ESP
        };

        for (RegisterName reg : display_order) {
            std::cout << "  " << reg_to_string(reg) << ": 0x"
                      << std::hex << std::setw(8) << std::setfill('0') << gprs.gpr.at(reg) << "\n";
        }
        std::cout << std::dec; // Reset to decimal

        std::cout << "\nFlags:\n";
        std::cout << "  ZF (Zero): " << (flags.ZF ? "1" : "0") << "\n";
        std::cout << "  SF (Sign): " << (flags.SF ? "1" : "0") << "\n";
        std::cout << "  CF (Carry): " << (flags.CF ? "1" : "0") << "\n";
        std::cout << "  OF (Overflow): " << (flags.OF ? "1" : "0") << "\n";
        std::cout << "--------------------------------\n";
    }
};

// --- V. Test Functions ---

void run_test_case(CPU& cpu, Memory& mem, const std::string& test_name,
                   const std::vector<uint32_t>& program, uint32_t start_address = 0) {
    std::cout << "\n\n=======================================================\n";
    std::cout << "         RUNNING TEST: " << test_name << "\n";
    std::cout << "=======================================================\n";

    cpu.reset();
    mem.load_program(program, start_address);
    cpu.run(mem);
}


// --- VI. Main Execution Block ---

int main() {
    // Instantiate the memory and the CPU
    Memory mem;
    CPU cpu;

    // =========================================================================
    // TEST 1: Arithmetic, Register-to-Register Operation, and Memory Store
    // (10 + 5) stored in Memory[10], then (15 - 5) stored in EDX.
    // =========================================================================

    // Instruction Encodings (Hexadecimal): [Opcode][Rd][Ra][Rb][Immediate/Addr]
    std::vector<uint32_t> program_1 = {
        // [0] MOVI EAX, 10
        0x5000000A,

        // [1] MOVI EBX, 5
        0x51000005,

        // [2] ADD ECX, EAX, EBX (ECX = 15)
        0x32010000,

        // [3] STORE [10], ECX
        0x2020000A,

        // [4] SUB EDX, ECX, EBX (EDX = 15 - 5 = 10)
        0x63210000,

        // [5] HALT
        0x00000000
    };

    run_test_case(cpu, mem, "1: Basic Arithmetic & Store", program_1, 0);

    // Verification for Test 1
    try {
        std::cout << "\n--- TEST 1 VERIFICATION ---\n";
        uint32_t val_in_mem = mem.read_word(10);

        // FIX: Use the new public getter method to access the register value
        uint32_t val_in_edx = cpu.get_register_value(RegisterName::EDX);

        std::cout << "Value in Memory[10] (Expected 15): " << val_in_mem << (val_in_mem == 15 ? " (PASS)" : " (FAIL)") << "\n";
        std::cout << "Value in EDX (Expected 10): " << val_in_edx << (val_in_edx == 10 ? " (PASS)" : " (FAIL)") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Verification failed: " << e.what() << "\n";
    }

    // =========================================================================
    // TEST 2: JUMP and LOAD Operations
    // Jumps over some NO-OPs and loads the value 15 (stored in Test 1) into EAX.
    // Memory Address 10 still holds 15 from Test 1.
    // =========================================================================

    std::vector<uint32_t> program_2 = {
        // [0] MOVI EAX, 99 (Dummy value to be overwritten)
        0x50000063,

        // [1] JUMP 5 (Jumps over instructions 2, 3, 4)
        0x40000005,

        // [2] MOVI EBX, 0xFFFF (Should be skipped)
        0x5100FFFF,

        // [3] MOVI ECX, 0xFFFF (Should be skipped)
        0x5200FFFF,

        // [4] JUMP 0 (Error state - Should be skipped)
        0x40000000,

        // [5] LOAD EAX, [10] (EAX = Mem[10] = 15)
        0x1000000A,

        // [6] HALT
        0x00000000
    };

    run_test_case(cpu, mem, "2: JUMP and LOAD", program_2, 0);

    // Verification for Test 2
    try {
        std::cout << "\n--- TEST 2 VERIFICATION ---\n";
        // FIX: Use the new public getter method to access the register value
        uint32_t val_in_eax = cpu.get_register_value(RegisterName::EAX);
        // FIX: Use the new public getter method to access the register value
        uint32_t val_in_ebx = cpu.get_register_value(RegisterName::EBX);

        std::cout << "Value in EAX (Expected 15): " << val_in_eax << (val_in_eax == 15 ? " (PASS)" : " (FAIL)") << "\n";
        // EBX should be 0 because the instruction that set it (at PC=2) was skipped
        std::cout << "Value in EBX (Expected 0): " << val_in_ebx << (val_in_ebx == 0 ? " (PASS)" : " (FAIL)") << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Verification failed: " << e.what() << "\n";
    }

    // =========================================================================
    // TEST 3: Zero Flag (ZF) Check
    // Set EAX = 10, EBX = 10, then EAX = EAX - EBX. ZF must be set.
    // =========================================================================

    std::vector<uint32_t> program_3 = {
        // [0] MOVI EAX, 10
        0x5000000A,

        // [1] MOVI EBX, 10
        0x5100000A,

        // [2] SUB EAX, EAX, EBX (EAX = 10 - 10 = 0)
        0x60010000,

        // [3] HALT
        0x00000000
    };

    run_test_case(cpu, mem, "3: Zero Flag (ZF) Check", program_3, 0);

    // Verification for Test 3
    try {
        std::cout << "\n--- TEST 3 VERIFICATION ---\n";
        // FIX: Use the new public getter method to access the register value
        uint32_t val_in_eax = cpu.get_register_value(RegisterName::EAX);

        // FIX: Use the new public getter method to access the flag state
        bool is_zf_set = cpu.get_flags().ZF;

        std::cout << "Value in EAX (Expected 0): " << val_in_eax << (val_in_eax == 0 ? " (PASS)" : " (FAIL)") << "\n";
        std::cout << "ZF Flag State (Expected 1): " << (is_zf_set ? "1 (PASS)" : "0 (FAIL)") << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Verification failed: " << e.what() << "\n";
    }

    return 0;
}
