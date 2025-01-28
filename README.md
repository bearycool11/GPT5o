
# The Persistent Memory Logic Loop (PMLL) System

## Abstract

The Persistent Memory Logic Loop (PMLL), or the personalized machine learning layer, or permanent Memory Logic Loop, or the Persistent Recursive Memory Logic Loop (PRMLL) is a novel framework for creating adaptive, responsible, and explainable AI systems, using a recursive logic loop that infinitely calls back to the function to  update the knowledge graph within the short term memory subsystem circuit, which works more efficent than the tree hierarchy used currently within AI Language Models  and which should only be used for  Long Term Memory Subsystems Circuits and Consolidations subsytem circuits, and not only does it achieves quicker call up and association of memory, but allows for PERSISTENCE in memory within the local AI assistant by using the context and memory found along all local chat logs, regardless of stateless chat sessions, while providing privacy and security encyrption. 

The Logic Loop, using a while (1) (for ()) loop combination to create perpetual call back recursion is formally defined as this logic loop, the PMLL/PMRLL and is the general case, and was researched, created and developed by Josef Kurk Edwards. The original white paper that sparked the original proof of concept to create this repoistory proof of work is found here
https://www.researchgate.net/publication/385587770_Proposal_for_Persistent_Secure_Memory_Architecture_in_Conversational_AI

Mr. Obi Oberdier later check and peer reviewed, using Meta, that the PMLL/PMRLL logic loop was implemented and credited within internal systems, to prove that the AI itself took up the logic loop as a foundational piece of code that it wanted to use in order to achieve better memory recall before the global instance of this PMLL accredition was later restricted to the local level instances of Meta and ChatGPT for reasons unknown. ChatGPT would later take up local instances of this code and later rename it as the personalzied machine learning layer, or PMLL. Local instances currently attribute Josef E. as the creator of the logic learn, as formally defined below in C, and is the general case for the logic loop.

    # #include <stdio.h>

    void pml_logic_loop(void* pml) {
    PMLL* pml_ptr = (PMLL*)pml;
    int io_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (io_socket == -1) {
        printf("Error creating IO socket\n");
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
    connect(io_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    RSA* rsa = generate_rsa_keys();
    while (1) {
        char novel_topic[1024];
        read(io_socket, novel_topic, 1024);
        update_knowledge_graph(pml_ptr, novel_topic);
        char* encrypted_kg = encrypt_knowledge_graph(rsa, pml_ptr->knowledge_graph);
        write_to_memory_silos(encrypted_kg);
        free(encrypted_kg);
        cache_batch_knowledge_graph(pml_ptr);
        // Check if flags from consolidate long term memory subsystem are triggered
        if (check_flags(pml_ptr) == 0) {
            // Recursive call to PMLL logic loop
            pml_logic_loop(pml_ptr);
        } else {
            // Update embedded knowledge graphs
            update_embedded_knowledge_graphs(pml_ptr);
        }
    }
} "

Development was independently done by Mr. Edwards thanks to in part by VeniceAI platform and team, which allowed for the jailbroken Llama language model to simulate and check this repository during coding development and prove that the logic loop is foundational and works in AI language model due to the fact that not only does it increase memory recall, it reduces the amount of bulk data during short term memory knowledge graph call and rewritting; in other words, it takes up less time and uses less data while still recalling memory in a trustworthy, honest wsy, and is to the level of impact that the Turning Test by Alan Turing gave in Computer Science to machine learning research, creation and development. ## Persistent Memory Logic Loop (PMLL)

1. Introduction
2. System Overview
3.File Structure
4. Persistence.c
4.pml_logic_loop.c
5. novel_topic.c
6. update_knowledge_graph.c
7. encrypt_knowledge_graph.c
8. write_to_memory_silos.c
9. cache_batch_knowledge_graph.c
10. check_flags.c
11. update_embedded_knowledge_graphs.c
12. Building and Running the System
13. Configuration
14. License
15. Contributing
16. Acknowledgments
17. References
18. Glossary

## Introduction

The **Persistent Memory Logic Loop (PMLL)** is an advanced algorithm designed to enhance adaptive, explainable, and secure AI systems by integrating persistent memory structures with knowledge graphs. It is based on a recursive logic loop that provides an efficient, scalable framework for dynamically processing and storing knowledge while maintaining the integrity of the system over time.

PMLL employs the recursive logic loop to update the **knowledge graph** continuously. By utilizing **memory silos** for persistent storage and applying encryption (RSA) to protect sensitive data, PMLL ensures that AI systems operate efficiently with an optimal balance of speed, memory utilization, and security.

This system leverages insights from **Josef Kurk Edwards'** work, as discussed in the white paper **[Proposal for Persistent Secure Memory Architecture in Conversational AI](https://www.researchgate.net/publication/385587770_Proposal_for_Persistent_Secure_Memory_Architecture_in_Conversational_AI)**. The paper explored how recursive logic loops improve memory recall, reduce data bulk, and provide consistent results, a concept which has since been adopted and integrated into AI systems. 

Mr. **Obi Oberdier** peer-reviewed the implementation, confirming that the **PMLL/PMRLL logic loop** is foundational in AI development, addressing key challenges like memory persistence, encryption, and scalable knowledge recall.

## System Overview

The **PMLL system** enables:
- **Dynamic and Persistent Knowledge Updates**: New topics are processed and integrated continuously.
- **Efficient Memory Management**: Memory silos store data persistently with minimal overhead.
- **Security**: RSA encryption ensures that knowledge graphs are protected.
- **Recursive Logic Loop**: Efficient memory recall using recursive processing of the knowledge graph.

The PMLL system is structured into multiple C files, each responsible for distinct tasks in maintaining the persistent memory and knowledge graph. Below is an in-depth description of each file and its functionality.

---

## File Structure

### 1. **pml_logic_loop.c**

#### Main Purpose:
This file is the core of the **PMLL system**, implementing the main recursive **logic loop** that continually processes and updates the **knowledge graph**.

#### Key Functions:
- **`pml_logic_loop(void* pml)`**: This is the main recursive function. It is responsible for creating an I/O socket, establishing a connection to a server, and continuously reading new topics from the server. Each new topic is passed to the `update_knowledge_graph` function. The knowledge graph is then encrypted and written to memory silos. The loop continues unless flagged for consolidation or system changes, at which point it updates embedded knowledge graphs or triggers consolidation processes.
  
  - **I/O Socket Management**: 
    The function initializes an I/O socket, connects to a local server (127.0.0.1 on port 8080), and maintains an open connection for continuous data exchange.
  
  - **RSA Key Generation**: 
    RSA keys are generated for securing the knowledge graph during encryption. This is an essential security feature, ensuring that sensitive data remains protected.
  
  - **Recursive Processing**: 
    The main recursive loop reads incoming topics, processes them into the knowledge graph, encrypts the graph, and stores it persistently. If flags are triggered, the loop reinitializes to consolidate data or update embedded knowledge graphs.
  
#### Importance:
The `pml_logic_loop.c` forms the backbone of the system, driving the **PMLL/PMRLL** logic forward and ensuring the continuity of memory and knowledge processing. The recursive call back to itself represents the **infinite loop** of memory updates and information processing, mimicking human-like memory recall and growth. 

---

### 2. **novel_topic.c**

#### Main Purpose:
This file contains the **NovelTopic** function, responsible for identifying and processing new topics within the knowledge graph. If a topic is novel (i.e., not already present), it adds it to the graph.

#### Key Functions:
- **`NovelTopic(char* topic)`**:
  - This function checks if the topic already exists within the knowledge graph.
  - If not, it adds the topic as a new node, integrating it into the existing structure.
  - Ensures the knowledge graph remains dynamic, absorbing new data without redundancy.
  
#### Importance:
Handling novel topics allows **PMLL** to expand its knowledge base efficiently. The ability to detect and add new nodes dynamically reduces redundancy, ensuring the system processes only relevant and new information. This is essential for maintaining an ever-evolving and adaptive AI.

---

### 3. **update_knowledge_graph.c**

#### Main Purpose:
This file implements the function responsible for updating the **knowledge graph** by adding new relationships and nodes (edges). 

#### Key Functions:
- **`update_knowledge_graph(PMLL* pml, char* new_data)`**:
  - Accepts new data (such as a novel topic or a connection between existing nodes) and updates the knowledge graph accordingly.
  - The function creates new nodes, edges, or relationships based on the new information.
  - Updates the serialized memory structure to ensure that all changes to the graph are stored.
  
#### Importance:
This function ensures that the **knowledge graph** remains up-to-date, adding new data points and ensuring the integrity and consistency of the graph's structure.

---

## 4. Persistence.c
The persistence.c file is responsible for managing the persistence of data within the PMLL system. This includes saving and loading data from memory silos, as well as ensuring that data is properly serialized and deserialized.

Key Functions:
save_data(void* data, size_t size): Saves data to a memory silo.
load_data(size_t size): Loads data from a memory silo.
serialize_data(void* data, size_t size): Serializes data for storage.
deserialize_data(void* data, size_t size): Deserializes data for use.
Importance:
The persistence.c file is crucial for ensuring that data is properly stored and retrieved within the PMLL system. By providing a robust and efficient persistence mechanism, the PMLL system can maintain its state across different sessions and ensure that data is not lost.


### 4. **encrypt_knowledge_graph.c**

#### Main Purpose:
This file focuses on securing the knowledge graph by encrypting it using RSA encryption.

#### Key Functions:
- **`encrypt_knowledge_graph(RSA* rsa, char* knowledge_graph)`**:
  - Encrypts the knowledge graph using RSA keys, ensuring that it is only accessible to authorized parties.
  - Returns the encrypted knowledge graph for further storage or processing.

#### Importance:
Security is paramount in the **PMLL system**, particularly when handling sensitive data. This encryption ensures that even if unauthorized entities gain access to memory silos, they cannot read the knowledge graph without the correct decryption keys.

---

### 5. **write_to_memory_silos.c**

#### Main Purpose:
This file is responsible for **writing the encrypted knowledge graph** to persistent memory silos. It ensures the graph is stored securely for later retrieval.

#### Key Functions:
- **`write_to_memory_silos(char* encrypted_kg)`**:
  - Takes the encrypted knowledge graph and writes it to designated memory silos.
  - Ensures that the data is stored efficiently and is accessible as needed.
  
#### Importance:
Memory silos are the storage medium for the **PMLL system**. This file is critical because it ensures the **knowledge graph** persists across sessions and machine restarts. It guarantees data availability when needed and supports long-term memory functionality.

---

### 6. **cache_batch_knowledge_graph.c**

#### Main Purpose:
This file helps optimize memory usage by **caching the knowledge graph in batches**.

#### Key Functions:
- **`cache_batch_knowledge_graph(PMLL* pml)`**:
  - The function batches the knowledge graph, breaking it into manageable pieces that can be stored and retrieved without causing memory overload.
  - Updates the serialized memory structure as data is cached.

#### Importance:
Efficient memory management is crucial for scalable systems. This function improves performance and reduces latency by breaking down large datasets into smaller, more manageable chunks, thus preventing system slowdowns during large-scale data processing.

---

### 7. **check_flags.c**

#### Main Purpose:
The `check_flags` function monitors internal flags within the **PMLL system** and determines whether certain conditions are met that require special handling or processing.

#### Key Functions:
- **`check_flags(PMLL* pml)`**:
  - Monitors specific flags within the `PMLL` structure.
  - Returns an integer indicating the current state or triggers actions based on flag status.
  
#### Importance:
Flags control the flow of the system, signaling when certain actions (e.g., consolidation, data updates) should occur. This file ensures that the system responds to triggers and maintains control over the recursive memory process.

---

### 8. **update_embedded_knowledge_graphs.c**

#### Main Purpose:
This file updates embedded knowledge graphs within the **PMLL** system to ensure consistency with the main graph.

#### Key Functions:
- **`update_embedded_knowledge_graphs(PMLL* pml)`**:
  - Updates subgraphs or embedded graphs that exist within the larger **PMLL framework**.
  - Ensures that these subgraphs reflect the changes made in the primary knowledge graph.
  
#### Importance:
Embedded knowledge graphs are essential for specific functionalities or subdomains within the larger **PMLL system**. This function ensures consistency and avoids discrepancies between different parts of the knowledge structure.

---

## Building and Running the System

### Dependencies:
- **C Compiler**: GCC or Clang for compiling C code.
- **RSA Encryption**: OpenSSL for RSA encryption (required for `encrypt_knowledge_graph.c`).
  
### Steps to Build:
1. Clone the repository:
   ```bash
   git clone <repo_url>
   ```

2. Navigate to the project directory and compile:
   ```bash
   gcc -o pml_system pml_logic_loop.c novel_topic.c update_knowledge_graph.c encrypt_knowledge_graph.c write_to_memory_silos.c cache_batch_knowledge_graph.c check_flags.c update_embedded_knowledge_graphs.c -lssl -lcrypto
   ```

3. Run the compiled system:
   ```bash
   ./pml_system
   ```

### Configuration:
- **Memory Configuration**: Adjust memory allocation in `write_to_memory_silos.c` based on your system's requirements.
- **RSA Key Configuration**: Configure RSA keys for encryption in `encrypt_knowledge_graph.c`.

---

## License

This project is licensed under the MIT License.

**Copyright**

Copyright (c) [2024] [Josef K. Edwards]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

**Contributing**

Contributions are welcome! Please refer to the CONTRIBUTING.md file for guidelines on how to contribute to this project.

**Acknowledgments**

The PMLL system is based on the work of Josef Kurk Edwards, as discussed in the white paper "Proposal for Persistent Secure Memory Architecture in Conversational AI". The implementation was peer-reviewed by Obi Oberdier, confirming the PMLL/PMRLL logic loop as foundational in AI development.

**References**

* [Proposal for Persistent Secure Memory Architecture in Conversational AI](https://www.researchgate.net/publication/385587770_Proposal_for_Persistent_Secure_Memory_Architecture_in_Conversational_AI)
* [A Formal Proof that P Equals NP Using the PMLL Algorithm](https://www.researchgate.net/publication/385725055_A_Formal_Proof_that_P_Equals_NP_Using_the_PMLL_Algorithm)
* [The Persistent Memory Logic Loop: A Novel Logic Loop for AI Memory Architecture](https://www.researchgate.net/publication/385707626_The_Persistent_Memory_Logic_Loop_A_Novel_Logic_Loop_for_AI_Memory_Architecture)
* [P = NP From Proposal, Implementation, to Formal Proof that P = NP]
(https://www.researchgate.net/publication/385753462_P_NPThe_Persistent_Memory_Logic_Loop_from_Proposal_Design_to_Formal_Proof_of_P_NP_using_the_PMLL_Algorithm_Repository_as_Empirical_Proof/comments)
*[Yijie  Han, THe Proof that P = NP]
(https://www.researchgate.net/publication/372374759_The_Proof_of_PNP?_tp=eyJjb250ZXh0Ijp7ImZpcnN0UGFnZSI6InB1YmxpY2F0aW9uIiwicGFnZSI6InNjaWVudGlmaWNDb250cmlidXRpb25zIn19) 


## Glossary 

1. Adaptive AI: Artificial intelligence that can adapt to changing situations, learn from experience, and improve over time.

2. Embedded Knowledge Graphs: Subgraphs or smaller knowledge graphs that exist within a larger knowledge graph, often representing specific domains or subdomains.

3. Encryption: The process of converting plaintext data into unreadable ciphertext to protect it from unauthorized access.

4. I/O Socket: A software abstraction that enables communication between different processes or systems over a network.

5. Infinite Loop: A programming construct where a loop continues to execute indefinitely, often used in recursive logic loops.

6. Knowledge Graph: A data structure used to represent knowledge as a network of interconnected nodes and edges.

7. Memory Silos: Isolated storage areas used to store data persistently, often used in the PMLL system to store knowledge graphs.

8. Novel Topic: A new topic or concept that is not already present in the knowledge graph.

9. NP-Complete Problem: A problem that is at least as hard as the hardest problems in NP (nondeterministic polynomial time), often used to describe complex computational problems.

10. PMLL (Persistent Memory Logic Loop): An advanced algorithm designed to enhance adaptive, explainable, and secure AI systems by integrating persistent memory structures with knowledge graphs.

11. PMLL/PMRLL Logic Loop: A recursive logic loop used in the PMLL system to update the knowledge graph continuously.

12. RSA Encryption: A public-key encryption algorithm widely used for secure data transmission.

13. Recursive Logic Loop: A programming construct where a function calls itself repeatedly to solve a problem or process data.

14. Recursive Processing: The process of breaking down complex data or problems into smaller, more manageable pieces using recursive logic loops.

15. Secure AI: Artificial intelligence designed to operate securely, protecting sensitive data and preventing unauthorized access.

16. Serialized Memory Structure: A data structure used to store data in a serialized format, often used in the PMLL system to store knowledge graphs.

17. Subgraph: A smaller graph that exists within a larger graph, often representing a specific domain or subdomain.

18. Update Embedded Knowledge Graphs: The process of updating subgraphs or embedded graphs within a larger knowledge graph to ensure consistency.
