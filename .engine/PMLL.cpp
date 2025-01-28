#include "PMLL.h"
#include <iostream>

int main() {
    // Initialize PMLL with a memory file
    PMLL pmll("memory_data.txt");

    // Add or update memory entries
    pmll.addMemory("username", "Josef");
    pmll.addMemory("project", "Unified PMLL Logic Loop");

    // Retrieve and print memory entries
    std::cout << "Username: " << pmll.getMemory("username") << "\n";
    std::cout << "Project: " << pmll.getMemory("project") << "\n";

    // Display all memory
    pmll.displayMemory();

    // Clear memory
    pmll.clearMemory();
    std::cout << "Memory cleared.\n";

    return 0;
}
