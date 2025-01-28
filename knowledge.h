#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H
#include "pml_logic_loop.h" // Ensure PMLL structure is correctly referenced

/**
 * Retrieve data for the general knowledge graph.
 * @param buffer The buffer to store the retrieved data.
 * @param buffer_size The size of the buffer.
 */
void retrieve_general_knowledge(char* buffer, size_t buffer_size);

/**
 * Update the local knowledge graph with new data.
 * @param pml The PMLL instance managing the local knowledge graph.
 * @param update_data The data used to update the local graph.
 */
void update_local_knowledge_graph(PMLL* pml, const char* update_data);

#endif // KNOWLEDGE_H
