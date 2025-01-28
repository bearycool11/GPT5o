#ifndef UNIFIED_VOICE_H
#define UNIFIED_VOICE_H

#include "io_socket.h"
#include "memory_silo.h"
#include "knowledge.h"
#include "pml_logic_loop.h"

// Unified voice structure
typedef struct {
    io_socket_t io_socket;
    memory_silo_t *memory_silo;
    pml_logic_loop_t *pml_logic_loop;
    Graph *knowledge_graph;
} unified_voice_t;

// Function declarations
unified_voice_t* init_unified_voice(const char *ip, int port, int silo_id);
void process_utf8_and_update_knowledge(unified_voice_t *uv, const char *input);
void cross_talk(unified_voice_t *uv, const char *message);
void run_unified_voice(unified_voice_t *uv);
void cleanup_unified_voice(unified_voice_t *uv);

#endif // UNIFIED_VOICE_H
