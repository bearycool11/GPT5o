#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Structure to pass data to threads
typedef struct {
    int thread_id;
    int num_variables;
    int num_clauses;
    const char* filename;
} thread_data_t;

// Mutex for thread-safe file access
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to generate random 3-SAT clauses
void generate_clauses(FILE* file, int num_variables, int num_clauses) {
    srand(time(NULL) ^ pthread_self());  // Unique seed for each thread

    for (int i = 0; i < num_clauses; ++i) {
        for (int j = 0; j < 3; ++j) {
            int var = rand() % num_variables + 1;
            if (rand() % 2 == 0) {
                var = -var;
            }
            fprintf(file, "%d ", var);
        }
        fprintf(file, "0\n");
    }
}

// Thread function to handle clause generation
void* thread_generate_3sat(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;

    // Open the file in append mode
    pthread_mutex_lock(&file_mutex);
    FILE* file = fopen(data->filename, "a");
    if (!file) {
        perror("Failed to open file for appending");
        pthread_mutex_unlock(&file_mutex);
        pthread_exit(NULL);
    }

    pthread_mutex_unlock(&file_mutex);

    // Generate clauses for this thread
    generate_clauses(file, data->num_variables, data->num_clauses);

    pthread_mutex_lock(&file_mutex);
    fclose(file);
    pthread_mutex_unlock(&file_mutex);

    printf("Thread %d finished generating clauses.\n", data->thread_id);
    pthread_exit(NULL);
}

// Main function to manage multi-threaded generation
int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num_variables> <num_clauses> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_variables = atoi(argv[1]);
    int num_clauses = atoi(argv[2]);
    int num_threads = atoi(argv[3]);

    if (num_threads <= 0 || num_variables <= 0 || num_clauses <= 0) {
        fprintf(stderr, "Invalid parameters.\n");
        return EXIT_FAILURE;
    }

    const char* filename = "3sat_instance.cnf";

    // Write CNF header
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file for writing");
        return EXIT_FAILURE;
    }
    fprintf(file, "p cnf %d %d\n", num_variables, num_clauses);
    fclose(file);

    // Launch threads
    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    int clauses_per_thread = num_clauses / num_threads;
    int remaining_clauses = num_clauses % num_threads;

    for (int i = 0; i < num_threads; ++i) {
        thread_data[i].thread_id = i;
        thread_data[i].num_variables = num_variables;
        thread_data[i].num_clauses = clauses_per_thread + (i < remaining_clauses ? 1 : 0);
        thread_data[i].filename = filename;

        if (pthread_create(&threads[i], NULL, thread_generate_3sat, &thread_data[i]) != 0) {
            perror("Failed to create thread");
            return EXIT_FAILURE;
        }
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("3-SAT instance generated: %s\n", filename);
    return 0;
}
