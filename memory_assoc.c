#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Define the structure for a node in the graph
typedef struct Node {
    char* entity;
    uint32_t hash;
    struct Node* next;
} Node;

// Define the structure for an edge in the graph
typedef struct Edge {
    Node* from;
    Node* to;
    char* relationship;
    uint32_t weight;
} Edge;

// Define the structure for the graph
typedef struct Graph {
    Node** nodes;
    Edge** edges;
    uint32_t num_nodes;
    uint32_t num_edges;
    uint32_t capacity_nodes;
    uint32_t capacity_edges;
} Graph;

// Define the memory association system
typedef struct MemoryAssoc {
    Graph* graph;
    uint32_t seed;
} MemoryAssoc;

// Function to create a new node
Node* create_node(const char* entity, uint32_t hash) {
    Node* node = malloc(sizeof(Node));
    if (!node) {
        perror("Failed to allocate memory for Node");
        exit(EXIT_FAILURE);
    }
    node->entity = strdup(entity);  // Duplicate the string
    node->hash = hash;
    node->next = NULL;
    return node;
}

// Function to create a new edge
Edge* create_edge(Node* from, Node* to, const char* relationship, uint32_t weight) {
    Edge* edge = malloc(sizeof(Edge));
    if (!edge) {
        perror("Failed to allocate memory for Edge");
        exit(EXIT_FAILURE);
    }
    edge->from = from;
    edge->to = to;
    edge->relationship = strdup(relationship);  // Duplicate the string
    edge->weight = weight;
    return edge;
}

// Function to create a new graph
Graph* create_graph(uint32_t initial_capacity) {
    Graph* graph = malloc(sizeof(Graph));
    if (!graph) {
        perror("Failed to allocate memory for Graph");
        exit(EXIT_FAILURE);
    }
    graph->nodes = malloc(initial_capacity * sizeof(Node*));
    graph->edges = malloc(initial_capacity * sizeof(Edge*));
    graph->num_nodes = 0;
    graph->num_edges = 0;
    graph->capacity_nodes = initial_capacity;
    graph->capacity_edges = initial_capacity;
    return graph;
}

// Function to add a node to the graph
void add_node(Graph* graph, Node* node) {
    if (graph->num_nodes >= graph->capacity_nodes) {
        graph->capacity_nodes *= 2;
        graph->nodes = realloc(graph->nodes, graph->capacity_nodes * sizeof(Node*));
    }
    graph->nodes[graph->num_nodes++] = node;
}

// Function to add an edge to the graph
void add_edge(Graph* graph, Edge* edge) {
    if (graph->num_edges >= graph->capacity_edges) {
        graph->capacity_edges *= 2;
        graph->edges = realloc(graph->edges, graph->capacity_edges * sizeof(Edge*));
    }
    graph->edges[graph->num_edges++] = edge;
}

// Function to get relationships between entities
void get_relationships(Graph* graph, Node* entity) {
    for (uint32_t i = 0; i < graph->num_edges; i++) {
        Edge* edge = graph->edges[i];
        if (edge->from == entity) {
            printf("%s -> %s (%s, %u)\n", edge->from->entity, edge->to->entity, edge->relationship, edge->weight);
        }
    }
}

// Function to create a new memory association system
MemoryAssoc* create_memory_assoc(uint32_t seed) {
    MemoryAssoc* memory_assoc = malloc(sizeof(MemoryAssoc));
    if (!memory_assoc) {
        perror("Failed to allocate memory for MemoryAssoc");
        exit(EXIT_FAILURE);
    }
    memory_assoc->graph = create_graph(1024);
    memory_assoc->seed = seed;
    return memory_assoc;
}

// Function to add an entity to the memory association system
void add_entity(MemoryAssoc* memory_assoc, const char* entity, uint32_t hash) {
    Node* node = create_node(entity, hash);
    add_node(memory_assoc->graph, node);
}

// Function to add a relationship to the memory association system
void add_relationship(MemoryAssoc* memory_assoc, const char* entity1, const char* entity2, const char* relationship, uint32_t weight) {
    Node* node1 = NULL;
    Node* node2 = NULL;

    // Find existing nodes or create new ones
    for (uint32_t i = 0; i < memory_assoc->graph->num_nodes; i++) {
        if (strcmp(memory_assoc->graph->nodes[i]->entity, entity1) == 0) {
            node1 = memory_assoc->graph->nodes[i];
        }
        if (strcmp(memory_assoc->graph->nodes[i]->entity, entity2) == 0) {
            node2 = memory_assoc->graph->nodes[i];
        }
    }
    
    // Create nodes if they don't exist
    if (!node1) {
        node1 = create_node(entity1, 0);
        add_node(memory_assoc->graph, node1);
    }
    if (!node2) {
        node2 = create_node(entity2, 0);
        add_node(memory_assoc->graph, node2);
    }

    Edge* edge = create_edge(node1, node2, relationship, weight);
    add_edge(memory_assoc->graph, edge);
}

// Function to get relationships between entities in the memory association system
void get_relationships_memory_assoc(MemoryAssoc* memory_assoc, const char* entity) {
    for (uint32_t i = 0; i < memory_assoc->graph->num_nodes; i++) {
        Node* node = memory_assoc->graph->nodes[i];
        if (strcmp(node->entity, entity) == 0) {
            get_relationships(memory_assoc->graph, node);
            break;
        }
    }
}

// Cleanup function
void cleanup_memory_assoc(MemoryAssoc* memory_assoc) {
    for (uint32_t i = 0; i < memory_assoc->graph->num_nodes; i++) {
        free(memory_assoc->graph->nodes[i]->entity);
        free(memory_assoc->graph->nodes[i]);
    }
    for (uint32_t i = 0; i < memory_assoc->graph->num_edges; i++) {
        free(memory_assoc->graph->edges[i]->relationship);
        free(memory_assoc->graph->edges[i]);
    }
    free(memory_assoc->graph->nodes);
    free(memory_assoc->graph->edges);
    free(memory_assoc->graph);
    free(memory_assoc);
}

int main() {
    MemoryAssoc* memory_assoc = create_memory_assoc(42);
    add_entity(memory_assoc, "Entity1", 1);
    add_entity(memory_assoc, "Entity2", 2);
    add_relationship(memory_assoc, "Entity1", "Entity2", "Relationship", 3);
    get_relationships_memory_assoc(memory_assoc, "Entity1");

    // Cleanup
    cleanup_memory_assoc(memory_assoc);
    return 0;
}
