#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "cluster_manager.h"
#include "memory_silo.h"
#include "io_socket.h"
#include "logic_loop.h"

// Cluster structure to hold information about a cluster
typedef struct {
    int cluster_id;
    int silo_count;
    int* silo_ids;     // Array of silo IDs managed by this cluster
    int io_socket;     // Communication socket for the cluster
    double workload;   // Current workload metric (bottleneck detection)
} Cluster;

// Global list of clusters
static Cluster* clusters = NULL;
static int cluster_count = 0;

// Function to calculate the distance between silos (mocked for simplicity)
double calculate_distance(int silo_a, int silo_b) {
    // Mock distance as Euclidean distance between silo IDs (arbitrary example)
    return fabs(silo_a - silo_b);
}

// Solve pairwise TSP for a cluster's silos
void solve_pairwise_tsp(Cluster* cluster) {
    printf("Optimizing workload distribution for Cluster %d (TSP)...\n", cluster->cluster_id);

    double min_distance = INFINITY;
    int best_pair[2] = { -1, -1 };

    for (int i = 0; i < cluster->silo_count; i++) {
        for (int j = i + 1; j < cluster->silo_count; j++) {
            double distance = calculate_distance(cluster->silo_ids[i], cluster->silo_ids[j]);
            if (distance < min_distance) {
                min_distance = distance;
                best_pair[0] = cluster->silo_ids[i];
                best_pair[1] = cluster->silo_ids[j];
            }
        }
    }

    printf("Cluster %d: Best silo pair for workload optimization: %d <-> %d (Distance: %.2f)\n",
           cluster->cluster_id, best_pair[0], best_pair[1], min_distance);

    // Redistribute workload based on the best pair (mock logic)
    printf("Redistributing workload between silo %d and silo %d...\n", best_pair[0], best_pair[1]);
}

// Check for bottlenecks in clusters
void check_for_bottlenecks() {
    printf("Checking for bottlenecks across all clusters...\n");
    for (int i = 0; i < cluster_count; i++) {
        Cluster* cluster = &clusters[i];

        // Mock workload calculation: sum of silo IDs (arbitrary metric)
        double workload = 0.0;
        for (int j = 0; j < cluster->silo_count; j++) {
            workload += cluster->silo_ids[j];
        }
        cluster->workload = workload;

        printf("Cluster %d workload: %.2f\n", cluster->cluster_id, cluster->workload);

        // Detect bottleneck (arbitrary threshold)
        if (cluster->workload > 1000.0) {
            printf("Bottleneck detected in Cluster %d! Optimizing...\n", cluster->cluster_id);
            solve_pairwise_tsp(cluster);
        }
    }
}

// Initialize a cluster
Cluster* initialize_cluster(int cluster_id, int silo_count) {
    printf("Initializing cluster %d with %d silos...\n", cluster_id, silo_count);

    Cluster* cluster = (Cluster*)malloc(sizeof(Cluster));
    if (!cluster) {
        perror("Failed to allocate memory for cluster");
        return NULL;
    }

    cluster->cluster_id = cluster_id;
    cluster->silo_count = silo_count;
    cluster->silo_ids = (int*)malloc(sizeof(int) * silo_count);
    if (!cluster->silo_ids) {
        perror("Failed to allocate memory for silo IDs");
        free(cluster);
        return NULL;
    }

    // Assign silos to the cluster
    for (int i = 0; i < silo_count; i++) {
        cluster->silo_ids[i] = cluster_id * 100 + i;  // Example: Generate silo IDs based on cluster ID
        memory_silo_init(cluster->silo_ids[i]);      // Initialize the silo
    }

    // Create an IO socket for the cluster
    cluster->io_socket = create_io_socket();
    if (cluster->io_socket < 0) {
        perror("Failed to create IO socket for cluster");
        free(cluster->silo_ids);
        free(cluster);
        return NULL;
    }

    cluster->workload = 0.0;
    printf("Cluster %d initialized successfully.\n", cluster_id);
    return cluster;
}

// Add a cluster to the global list
void add_cluster(Cluster* cluster) {
    if (!cluster) return;

    clusters = (Cluster*)realloc(clusters, sizeof(Cluster) * (cluster_count + 1));
    if (!clusters) {
        perror("Failed to resize cluster list");
        return;
    }

    clusters[cluster_count] = *cluster;
    cluster_count++;
    printf("Cluster %d added to the global list.\n", cluster->cluster_id);
}

// Distribute workload across clusters
void distribute_workload(PMLL* pml) {
    printf("Distributing workload across %d clusters...\n", cluster_count);

    for (int i = 0; i < cluster_count; i++) {
        Cluster* cluster = &clusters[i];

        // Simulate workload assignment
        char workload_message[BUFFER_SIZE];
        snprintf(workload_message, sizeof(workload_message), "Workload for cluster %d", cluster->cluster_id);

        io_socket_write(cluster->io_socket, workload_message, strlen(workload_message));
        printf("Workload sent to cluster %d: %s\n", cluster->cluster_id, workload_message);

        // Monitor silo performance
        for (int j = 0; j < cluster->silo_count; j++) {
            int silo_id = cluster->silo_ids[j];
            char silo_data[BUFFER_SIZE];
            snprintf(silo_data, sizeof(silo_data), "Silo %d processing workload...", silo_id);
            printf("%s\n", silo_data);
        }
    }
}

// Clean up all clusters
void cleanup_clusters() {
    printf("Cleaning up all clusters...\n");

    for (int i = 0; i < cluster_count; i++) {
        Cluster* cluster = &clusters[i];

        // Close the IO socket
        io_socket_cleanup(cluster->io_socket);

        // Free silo IDs
        free(cluster->silo_ids);
    }

    // Free the cluster list
    free(clusters);
    clusters = NULL;
    cluster_count = 0;

    printf("All clusters cleaned up successfully.\n");
}
