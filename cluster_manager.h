#ifndef CLUSTER_MANAGER_H
#define CLUSTER_MANAGER_H

#include "pml.h"

// Define Cluster structure
typedef struct Cluster Cluster;

/**
 * Initialize a cluster with the specified ID and number of silos.
 * @param cluster_id The unique ID of the cluster.
 * @param silo_count The number of silos to initialize within the cluster.
 * @return Pointer to the initialized Cluster, or NULL if an error occurs.
 */
Cluster* initialize_cluster(int cluster_id, int silo_count);

/**
 * Add a new cluster to the global list of managed clusters.
 * @param cluster Pointer to the Cluster to be added.
 */
void add_cluster(Cluster* cluster);

/**
 * Check for bottlenecks in all managed clusters and optimize workload distribution.
 * This function identifies clusters under stress and redistributes tasks.
 */
void check_for_bottlenecks();

/**
 * Solve the Pairwise Traveling Salesman Problem (TSP) to optimize task execution
 * across clusters, minimizing the cost of operations.
 * @param pml Pointer to the PMLL instance for data and task prioritization.
 */
void solve_pairwise_tsp(PMLL* pml);

/**
 * Distribute workloads dynamically across all available clusters.
 * Ensures no cluster is overloaded and tasks are handled efficiently.
 * @param pml Pointer to the PMLL instance for workload data.
 */
void distribute_workload(PMLL* pml);

/**
 * Clean up and free all resources associated with the clusters.
 * This includes memory and connections for all managed clusters.
 */
void cleanup_clusters();

#endif // CLUSTER_MANAGER_H