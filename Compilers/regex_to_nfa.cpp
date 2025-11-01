#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>

// --- Global NFA Components ---
// Global counter for generating unique state IDs (0, 1, 2, ...)
static int stateCounter = 0; 
// Convention: '#' is used for the epsilon (ε) transition symbol

// 1. Structure to define a single transition (edge)
struct Transition {
    int fromState;      // Start state ID
    int toState;        // End state ID
    char symbol;        // Input symbol: a, b, etc., or '#' for epsilon (ε)
};

// 2. Core NFA Fragment structure (the element stored on the stack)
struct NFA {
    int startState;                     // The designated entry point for this fragment
    int finalState;                     // The designated exit/accepting point for this fragment
    std::vector<Transition> transitions; // All transitions within this fragment
};

// --- Helper Functions (Base Cases) ---

NFA CreateSingleSymbolNFA(char symbol) {
    NFA newNFA;

    // Create unique start and final states
    newNFA.startState = stateCounter++;
    newNFA.finalState = stateCounter++;

    // Add the transition: Start -> (symbol) -> Final
    Transition t = {newNFA.startState, newNFA.finalState, symbol};
    newNFA.transitions.push_back(t);

    return newNFA;
}

NFA CreateEpsilonNFA() {
    NFA newNFA;

    // Create unique start and final states
    newNFA.startState = stateCounter++;
    newNFA.finalState = stateCounter++;

    // Add the transition: Start -> (epsilon) -> Final
    Transition t = {newNFA.startState, newNFA.finalState, '#'};
    newNFA.transitions.push_back(t);

    return newNFA;
}

// --- Inductive Case: Concatenation (R1 . R2) ---
NFA ConcatenateNFAs(const NFA& n1, const NFA& n2) {
    NFA n_new;

    // 1. Define the start and final states
    n_new.startState = n1.startState;
    n_new.finalState = n2.finalState; 
    
    // 2. Aggregate all transitions from both sub-NFAs
    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());
    n_new.transitions.insert(n_new.transitions.end(), n2.transitions.begin(), n2.transitions.end());

    // 3. Add the connecting epsilon transition: N1.final -> N2.start
    Transition connector = {n1.finalState, n2.startState, '#'};
    n_new.transitions.push_back(connector);

    return n_new;
}

// --- Inductive Case: Union (R1 | R2) ---
NFA UnionNFAs(const NFA& n1, const NFA& n2) {
    NFA n_new;

    // 1. Create the new global start and final states
    n_new.startState = stateCounter++;
    n_new.finalState = stateCounter++;
    
    // 2. Aggregate all transitions from both sub-NFAs
    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());
    n_new.transitions.insert(n_new.transitions.end(), n2.transitions.begin(), n2.transitions.end());

    // 3. Add the four new epsilon transitions for the Union
    n_new.transitions.push_back({n_new.startState, n1.startState, '#'});  // NewStart -> N1.Start
    n_new.transitions.push_back({n_new.startState, n2.startState, '#'});  // NewStart -> N2.Start
    n_new.transitions.push_back({n1.finalState, n_new.finalState, '#'});  // N1.Final -> NewFinal
    n_new.transitions.push_back({n2.finalState, n_new.finalState, '#'});  // N2.Final -> NewFinal

    return n_new;
}

// --- Inductive Case: Kleene Star (R1*) ---
NFA KleeneStarNFA(const NFA& n1) {
    NFA n_new;

    // 1. Create the new global start and final states
    n_new.startState = stateCounter++;
    n_new.finalState = stateCounter++;

    // 2. Aggregate all transitions from the sub-NFA
    n_new.transitions.insert(n_new.transitions.end(), n1.transitions.begin(), n1.transitions.end());

    // 3. Add the four new epsilon transitions for the Kleene Star
    n_new.transitions.push_back({n_new.startState, n_new.finalState, '#'}); // Zero repetitions
    n_new.transitions.push_back({n_new.startState, n1.startState, '#'});    // Entry
    n_new.transitions.push_back({n1.finalState, n_new.finalState, '#'});    // Exit
    n_new.transitions.push_back({n1.finalState, n1.startState, '#'});      // Loop

    return n_new;
}

// --- Preprocessing: Shunting-Yard Algorithm to Convert Infix RE to Postfix ---

// Define precedence for operators
int precedence(char op) {
    if (op == '|') return 1;
    if (op == '.') return 2;
    if (op == '*') return 3; // Highest
    return 0; // For parentheses and literals
}

// A simplified Shunting-Yard implementation. 
// NOTE: It requires the input RE to ALREADY have explicit '.' concatenation markers.
std::string RegExToPostfix(const std::string& infix_re) {
    std::string postfix = "";
    std::stack<char> op_stack;

    for (char token : infix_re) {
        if (std::isalpha(token) || std::isdigit(token) || token == '#') {
            // Operands (symbols and epsilon)
            postfix += token;
        } else if (token == '(') {
            op_stack.push(token);
        } else if (token == ')') {
            // Pop operators until '(' is found
            while (!op_stack.empty() && op_stack.top() != '(') {
                postfix += op_stack.top();
                op_stack.pop();
            }
            if (!op_stack.empty() && op_stack.top() == '(') {
                op_stack.pop(); // Discard '('
            }
        } else if (token == '*' || token == '.' || token == '|') {
            // Operators
            while (!op_stack.empty() && precedence(op_stack.top()) >= precedence(token)) {
                if (op_stack.top() != '(') { // Only pop operators
                    postfix += op_stack.top();
                    op_stack.pop();
                } else {
                    break;
                }
            }
            op_stack.push(token);
        }
    }

    // Pop remaining operators
    while (!op_stack.empty()) {
        postfix += op_stack.top();
        op_stack.pop();
    }

    return postfix;
}

// --- Main Thompson's Construction Algorithm ---
NFA ThompsonConstruction(const std::string& postfix_re) {
    std::stack<NFA> nfa_stack;
    
    // IMPORTANT: Reset the state counter for a fresh construction
    stateCounter = 0; 

    for (char token : postfix_re) {
        switch (token) {
            // Base Cases: Symbols (Operands)
            case 'a': case 'b': case 'c': case '0': case '1':
                nfa_stack.push(CreateSingleSymbolNFA(token));
                break;
            case '#': // Explicit epsilon (empty string)
                nfa_stack.push(CreateEpsilonNFA());
                break;

            // Binary Operators
            case '.': // Concatenation
            case '|': // Union
            {
                if (nfa_stack.size() < 2) 
                    throw std::runtime_error("Error: Not enough operands for binary operator.");
                
                NFA n2 = nfa_stack.top(); nfa_stack.pop();
                NFA n1 = nfa_stack.top(); nfa_stack.pop();
                
                if (token == '.') {
                    nfa_stack.push(ConcatenateNFAs(n1, n2));
                } else { // '|'
                    nfa_stack.push(UnionNFAs(n1, n2));
                }
                break;
            }

            // Unary Operator
            case '*': // Kleene Star
            {
                if (nfa_stack.empty()) 
                    throw std::runtime_error("Error: Not enough operands for unary operator '*'.");
                
                NFA n1 = nfa_stack.top(); nfa_stack.pop();
                nfa_stack.push(KleeneStarNFA(n1));
                break;
            }

            default:
                std::cerr << "Warning: Skipping unknown token '" << token << "'." << std::endl;
                break;
        }
    }

    if (nfa_stack.size() != 1) {
        throw std::runtime_error("Invalid expression structure: Stack size is not 1 at the end.");
    }

    return nfa_stack.top();
}

// --- Output Function ---
void PrintNFA(const NFA& nfa, const std::string& re_string) {
    std::cout << "==========================================================" << std::endl;
    std::cout << "NFA for Regular Expression: " << re_string << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Total States: " << stateCounter << std::endl;
    std::cout << "Start State (q0): " << nfa.startState << std::endl;
    std::cout << "Final State (qf): " << nfa.finalState << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    std::cout << "Transitions (from -> symbol -> to):" << std::endl;

    // Use a map to group transitions by source state for cleaner output
    std::map<int, std::vector<std::pair<char, int>>> grouped_transitions;
    for (const auto& t : nfa.transitions) {
        grouped_transitions[t.fromState].push_back({t.symbol, t.toState});
    }

    for (const auto& pair : grouped_transitions) {
        std::cout << "q" << pair.first << ": ";
        for (const auto& t_data : pair.second) {
            char sym = t_data.first;
            std::cout << " --(" << (sym == '#' ? "ε" : std::string(1, sym)) << ")--> q" << t_data.second << " | ";
        }
        std::cout << std::endl;
    }
    std::cout << "==========================================================" << std::endl;
}


// --- Main Test Function ---
int main() {
    std::cout << "Thompson's Construction Algorithm (RE to NFA-ε)" << std::endl;
    std::cout << "-------------------------------------------------" << std::endl;

    // Test Cases (NOTE: All INFIX expressions MUST use explicit '.' for concatenation)
    std::vector<std::string> test_expressions = {
        "a",         // Base case: Symbol
        "a.b",       // Concatenation: ab
        "a|b",       // Union: a|b
        "a*",        // Kleene Star: a*
        "(a|b)*.c"   // Complex RE: (a|b)*c
    };
    
    // This example requires a base epsilon NFA. 
    // You must use '#' in the RE to represent the empty string.
    std::string test_epsilon = "a|#"; // a or epsilon

    // 1. Test Simple Epsilon
    try {
        std::string postfix_re = RegExToPostfix(test_epsilon);
        std::cout << "\nTest 1: INFIX: " << test_epsilon << " -> POSTFIX: " << postfix_re << std::endl;
        NFA result_nfa = ThompsonConstruction(postfix_re);
        PrintNFA(result_nfa, test_epsilon);
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
    }

    // 2. Test main operations
    for (const std::string& infix_re : test_expressions) {
        try {
            std::string postfix_re = RegExToPostfix(infix_re);
            std::cout << "\nTest: INFIX: " << infix_re << " -> POSTFIX: " << postfix_re << std::endl;
            NFA result_nfa = ThompsonConstruction(postfix_re);
            PrintNFA(result_nfa, infix_re);
        } catch (const std::exception& e) {
            std::cerr << "Test failed for " << infix_re << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
