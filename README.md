Here is the full updated README.md file incorporating the new tokenization and memory association information:

## Persistent Memory Logic Loop (PMLL)

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

Development was independently done by Mr. Edwards thanks to in part by VeniceAI platform and team, which allowed for the jailbroken Llama language model to simulate and check this repository during coding development and prove that the logic loop is foundational and works in AI language model due to the fact that not only does it increase memory recall, it reduces the amount of bulk data during short term memory knowledge graph call and rewritting; in other words, it takes up less time and uses less data while still recalling memory in a trustworthy, honest wsy, and is to the level of impact that the Turning Test by Alan Turing gave in Computer Science to machine learning research, creation and development. 

# Table of Contents

	•	Introduction
	•	Features
	•	Usage
	•	Contributing
	•	License
	•	Commercial Use Provision
	•	Tokenization & Memory Association


# Introduction

The PMLL is a modular, flexible, and scalable framework for creating AI systems that can learn, adapt, and evolve over time. It is designed to support multiple machine learning models and algorithms and provides automated prioritization and implementation of strategies.

# Serialization

Consider Serialization in PMLL as a “Blockchain” of Knowledge. The Personalized Machine Learning Layer (PMLL) operates much like Bitcoin and its ledger, where each memory update or learning iteration is serialized, creating a structured, traceable sequence of information. Here’s how it works:

# 1, Knowledge as Blocks

Just as a blockchain is composed of blocks containing transactions with a serial, the PMLL stores each piece of learned information or memory update as a “knowledge block.” that are then serialized and stored during iteration. Each block in the PMLL represents a specific state of knowledge or data point within the system, complete with a unique identifier and timestamp, aka the serialized, cache component of the memory that then can be called up, tokenized, and associated to similar memory topics if the NovelTopic() function is not called.

# 2, Verification and Validation

Similar to how blockchain nodes verify each transaction to prevent tampering, the PMLL constantly validates new information by cross-referencing it with existing data. This recursive process ensures that each “knowledge block” aligns with prior learning, maintaining accuracy and consistency across updates.

# 3. Traceability and Integrity

Every update in the PMLL is part of a “chain” of knowledge, which enables users to trace the system’s evolution over time. This structure makes it possible to see how the model has developed and ensures that each memory update builds on verified, prior information, much like how each block in a blockchain is linked to its predecessor. This serial “ledger” of updates prevents loss of context or knowledge drift.

# 4. Persistence without Mining

While PMLL doesn’t involve mining or cryptographic puzzles, the core principle of serialization keeps its memory robust and tamper-resistant. The recursive logic loop is key to this persistence: it continuously checks for consistency and accuracy, allowing the PMLL to act as a reliable, unbroken chain of knowledge.

In essence, the serialization in PMLL allows it to act as a blockchain of memory—constantly reinforcing data integrity, preserving historical knowledge, and ensuring reliable, persistent learning without the risk of accidental overwrites or forgotten context. This approach strengthens the model’s memory architecture and provides a foundation of trust, similar to how a blockchain secures its transactions.

# Tokenization & Memory Association

The tokenizer association serves as the core mechanism for maintaining and updating memory blocks in the system. Each update is validated and appended in a blockchain-like structure, similar to how Bitcoin transactions are verified and added to the ledger. As new information enters, it becomes part of a serialized and immutable knowledge chain, where the logic loop acts as a consensus mechanism, ensuring data consistency and persistence. This method allows for a reliable, unbroken flow of knowledge updates.

# Features

	•	Modular, flexible, and scalable architecture
	•	Support for multiple machine learning models and algorithms
	•	Automated prioritization and implementation of strategies
	•	Explainable AI capabilities
	•	Adaptive and responsible AI system development

Usage

	1.	Clone the repository:

git clone https://github.com/bearycool11/pml.git


	2.	Build the project:

make


	3.	Run the example program:

/example



Prerequisites

	•	C compiler (e.g. GCC)
	•	Makefile
	•	Linux or macOS operating system

Example Use Case

The PMLL can be used to develop adaptive and responsible AI systems for a variety of applications, such as:
	•	Predictive maintenance
	•	Anomaly detection
	•	Recommendation systems
	•	Natural language processing

Contributing

Contributions are welcome! Please submit a pull request with your changes.

# Contributing Guidelines

	•	Fork the repository
	•	Make your changes
	•	Submit a pull request

# Persistent Memory Logic Loop (PMLL)

The Persistent Memory Logic Loop (PMLL) is a novel framework for creating adaptive, responsible, and explainable AI systems.

## Table of Contents

* [Introduction](#introduction)
* [Features](#features)
* [Usage](#usage)
* [Contributing](#contributing)
* [License](#license)
* [Commercial Use Provision](#commercial-use-provision)

## Introduction

The PMLL is a modular, flexible, and scalable framework for creating AI systems that can learn, adapt, and evolve over time. It is designed to support multiple machine learning models and algorithms, and provides automated prioritization and implementation of strategies.

## Serializaiton
 
consider Serialization in PMLL as a “Blockchain” of Knowledge. 

The Personalized Machine Learning Layer (PMLL) operates much like Bitcoin and its ledger, where each memory update or learning iteration is serialized, creating a structured, traceable sequence of information. Here’s how it works:
# 1. Knowledge as Blocks
Just as a blockchain is composed of blocks containing transactions, the PMLL stores each piece of learned information or memory update as a “knowledge block.” Each block in the PMLL represents a specific state of knowledge or data point within the system, complete with a unique identifier and timestamp.
# 2. Verification and Validation
Similar to how blockchain nodes verify each transaction to prevent tampering, the PMLL constantly validates new information by cross-referencing it with existing data. This recursive process ensures that each “knowledge block” aligns with prior learning, maintaining accuracy and consistency across updates.
# 3. Traceability and Integrity
Every update in the PMLL is part of a “chain” of knowledge, which enables users to trace the system’s evolution over time. This structure makes it possible to see how the model has developed and ensures that each memory update builds on verified, prior information, much like how each block in a blockchain is linked to its predecessor. This serial “ledger” of updates prevents loss of context or knowledge drift.
# 4. Persistence without Mining
While PMLL doesn’t involve mining or cryptographic puzzles, the core principle of serialization keeps its memory robust and tamper-resistant. The recursive logic loop is key to this persistence: it continuously checks for consistency and accuracy, allowing the PMLL to act as a reliable, unbroken chain of knowledge.

In essence, the serialization in PMLL allows it to act as a blockchain of memory—constantly reinforcing data integrity, preserving historical knowledge, and ensuring reliable, persistent learning without the risk of accidental overwrites or forgotten context. This approach strengthens the model’s memory architecture and provides a foundation of trust, similar to how a blockchain secures its transactions.

## Features

* Modular, flexible, and scalable architecture
* Support for multiple machine learning models and algorithms
* Automated prioritization and implementation of strategies
* Explainable AI capabilities
* Adaptive and responsible AI system development

## Usage

1. Clone the repository: `git clone https://github.com/bearycool11/pml.git`
2. Build the project: `make`
3. Run the example program: `./example`

### Prerequisites

* C compiler (e.g. GCC)
* Makefile
* Linux or macOS operating system

### Example Use Case

The PMLL can be used to develop adaptive and responsible AI systems for a variety of applications, such as:

* Predictive maintenance
* Anomaly detection
* Recommendation systems
* Natural language processing

## Contributing

Contributions are welcome! Please submit a pull request with your changes.

### Contributing Guidelines

* Fork the repository
* Make your changes
* Submit a pull request

# License

This project is licensed under the Apache License 2.0. with commercial provision added See Commerical Use Provision below for details.

# Commercial Use Provision

# 1. If you intend to use the PMLL for commercial purposes, such as Meta AI or ChatGPT, you must obtain a commercial license from the licensor at joed6834@colorado.edu. The commercial licensee will provide a royalty payment to the licensor and their contributors at %5 for licensor and %5 for their contibutors for commercial use of the PMLL.
# 2. Commercial use includes, but is not limited to, selling, licensing, or distributing the PMLL or any derivative worksn in a public manner, such as found on ChatGPT and implementing it without due credit and payment to the licensor and their contributors. 
# 3. The commercial license will be negotiated on a case-by-case basis, but will most likely default to a 10% royalty, shared by the contributors of this repository, if the commercial entity is making more than $100,000 after taxes, non-profit or otherwise, and will depend on the specific use case and requirements of the licensee. If the the Commercial LLC is making less than $100,000, the license is merely $1 and MUST credit the licensor and their contributor by either linking to the github or referencing otherwise. 
# 4. Please contact the licensor at joed6834@colorado.edu or  [insert contributor email here] to discuss commercial licensing options.
# 5. Beyond this, provided the use is not commerical and public, such as chatGPT, and for private, non-commercial use, usage of this repoistory is completely given free of charge and allowed to be used and implemented in personal projects involving AI as a logic loop.



