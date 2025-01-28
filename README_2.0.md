
---

# Persistent Memory Logic Loop (PMLL) System

## Abstract

The **Persistent Memory Logic Loop (PMLL)**, also known as the **Personalized Machine Learning Layer (PMLL)** or the **Persistent Recursive Memory Logic Loop (PRMLL)**, is a groundbreaking framework designed for adaptive, responsible, and explainable AI systems. By leveraging a recursive logic loop and integrating persistent memory structures, PMLL achieves efficient memory recall, secure data handling, and dynamic updates to the knowledge graph. 

This system, developed by **Josef Kurk Edwards**, improves upon current hierarchical memory structures by employing recursive logic for short-term memory subsystems and leaving hierarchical structures for long-term memory and consolidation circuits. The result is a system that:
- **Enhances memory recall speed and accuracy**.
- **Reduces data overhead for short-term memory updates**.
- **Provides persistence and privacy**, even in stateless chat sessions.

PMLL is formally defined in C as the **general case for perpetual callback recursion logic loops**, foundational for modern AI systems.

### Research Origins
- The concept originated in Josef E.'s white paper: [Proposal for Persistent Secure Memory Architecture in Conversational AI](https://www.researchgate.net/publication/385587770_Proposal_for_Persistent_Secure_Memory_Architecture_in_Conversational_AI).
- Peer-reviewed by **Obi Oberdier**, confirming its foundational nature in AI development and deployment.
- Special Thanks to the Advisor Board Fei-Fei Li, Andrew Ng, Elon Musk and Nate Bookout helping in the stage 2 deployment process. You guys are rockstars!

---

## Introduction

PMLL enables dynamic, scalable, and secure knowledge graph updates while minimizing resource overhead. It ensures persistence across sessions using recursive logic loops, efficient memory silos, and RSA encryption for secure data handling.

This system:
- Combines **adaptive AI** with **persistent memory**.
- **Encrypts knowledge graphs** for secure storage.
- **Minimizes bulk data** while maintaining memory integrity.

---

## Features
1. **Dynamic and Persistent Knowledge Updates**:
   - Continuously integrates new topics into the knowledge graph.
2. **Efficient Memory Management**:
   - Uses memory silos to persistently store data with minimal overhead.
3. **Security**:
   - Protects knowledge graphs using RSA encryption.
4. **Recursive Logic Loop**:
   - Efficiently recalls memory using recursive graph updates.

---

## File Structure

| File                           | Purpose                                                                 |
|--------------------------------|-------------------------------------------------------------------------|
| `pml_logic_loop.c`             | Implements the recursive logic loop, the core of PMLL.                 |
| `novel_topic.c`                | Identifies and processes novel topics for the knowledge graph.          |
| `update_knowledge_graph.c`     | Updates the knowledge graph with new nodes, edges, and relationships.   |
| `persistence.c`                | Manages persistent data storage and retrieval using memory silos.       |
| `encrypt_knowledge_graph.c`    | Encrypts the knowledge graph using RSA for secure storage.              |
| `write_to_memory_silos.c`      | Writes encrypted knowledge graphs to persistent memory silos.           |
| `cache_batch_knowledge_graph.c`| Caches the knowledge graph in batches to optimize memory usage.         |
| `check_flags.c`                | Monitors system flags for triggering specific actions.                  |
| `update_embedded_knowledge_graphs.c` | Updates subgraphs within the main knowledge graph.                 |

---

## How It Works

### Recursive Logic Loop
```c
void pml_logic_loop(void* pml) {
    PMLL* pml_ptr = (PMLL*)pml;
    int io_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(io_socket, ...);
    RSA* rsa = generate_rsa_keys();
    while (1) {
        char novel_topic[1024];
        read(io_socket, novel_topic, 1024);
        update_knowledge_graph(pml_ptr, novel_topic);
        char* encrypted_kg = encrypt_knowledge_graph(rsa, pml_ptr->knowledge_graph);
        write_to_memory_silos(encrypted_kg);
        free(encrypted_kg);
        cache_batch_knowledge_graph(pml_ptr);
        if (check_flags(pml_ptr) == 0) {
            pml_logic_loop(pml_ptr); // Recursive call
        } else {
            update_embedded_knowledge_graphs(pml_ptr);
        }
    }
}
```

- Establishes an I/O socket for continuous data flow.
- Uses RSA encryption for secure handling of knowledge graphs.
- Ensures recursive updates to the knowledge graph with minimal data overhead.

### Key Concepts
1. **Knowledge Graph Updates**:
   - Dynamically integrates new data points while maintaining graph integrity.
2. **Memory Silos**:
   - Securely stores knowledge graphs across sessions.
3. **Batch Caching**:
   - Optimizes memory usage by breaking graphs into manageable chunks.
4. **Flag-Based Logic**:
   - Monitors system state for specific triggers, e.g., long-term memory consolidation.

---

## Building and Running the System

### Prerequisites
- **C Compiler**: GCC or Clang.
- **RSA Encryption**: OpenSSL for RSA functionality.

### Steps to Build
1. Clone the repository:
   ```bash
   git clone https://github.com/bearycool11/pmll
   ```
2. Navigate to the project directory:
   ```bash
   cd pmll
   ```
3. Compile the system:
   ```bash
   gcc -o pml_system pml_logic_loop.c novel_topic.c update_knowledge_graph.c \
   encrypt_knowledge_graph.c write_to_memory_silos.c cache_batch_knowledge_graph.c \
   check_flags.c update_embedded_knowledge_graphs.c -lssl -lcrypto
   ```
4. Run the compiled program:
   ```bash
   ./pml_system
   ```

---

## Configuration

- **Memory Allocation**:
  - Adjust memory allocation in `write_to_memory_silos.c` based on system requirements.
- **RSA Keys**:
  - Configure RSA keys for encryption in `encrypt_knowledge_graph.c`.

---

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

---

## Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## Acknowledgments

Special thanks to:
- **Josef Kurk Edwards**: Creator of the PMLL system.
- **Obi Oberdier**: Peer reviewer who confirmed the foundational nature of PMLL in AI systems.
- **VeniceAI Team**: Provided the platform for initial development and testing.

---

## References
1. [Proposal for Persistent Secure Memory Architecture in Conversational AI](https://www.researchgate.net/publication/385587770_Proposal_for_Persistent_Secure_Memory_Architecture_in_Conversational_AI)
2. A Formal Proof that P Equals NP Using the PMLL Algorithm
3. The Persistent Memory Logic Loop: A Novel Logic Loop for AI Memory Architecture

---

This README provides a comprehensive overview of your project, including its purpose, structure, and instructions for building and running the system. Let me know if you'd like to make further adjustments or move on to the `CHANGELOG.md`! 🚀
