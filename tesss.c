#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX

// Struct for Node (location)
struct Node {
    char name[50];
};

// Struct for Edge (route)
struct Edge {
    int dest;       // Destination node index
    int weight;     // Distance or time
    struct Edge* next;
};

// Global variables for graph
struct Node nodes[MAX_NODES];
struct Edge* adjList[MAX_NODES];  // Adjacency list: array of linked lists
int nodeCount = 0;  // Current number of nodes

// Dijkstra's algorithm arrays
int distance[MAX_NODES];
int visited[MAX_NODES];
int predecessor[MAX_NODES];

// Function prototypes
int login();
void menu();
void addNode();
void editNode();
void deleteNode();
void addEdge();
void editEdge();
void deleteEdge();
void displayGraph();
void dijkstra(int start, int end);
int findNodeIndex(char* name);
void freeAdjList();

int main() {
    // Initialize adjacency list to NULL
    for (int i = 0; i < MAX_NODES; i++) {
        adjList[i] = NULL;
    }

    if (login()) {
        menu();
    } else {
        printf("Login failed. Exiting...\n");
    }

    // Free memory before exit
    freeAdjList();
    return 0;
}

// Simple login function with hardcoded credentials
int login() {
    char username[50], password[50];
    printf("Admin Login\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, "zahwadzakira") == 0 && strcmp(password, "satuduatiga") == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Invalid credentials.\n");
        return 0;
    }
}

// Main menu function
void menu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. Tambah Lokasi\n");
        printf("2. Edit Lokasi\n");
        printf("3. Hapus Lokasi\n");
        printf("4. Tambah Rute\n");
        printf("5. Edit Rute\n");
        printf("6. Hapus Rute\n");
        printf("7. Display Graph\n");
        printf("8. Rute Terpendek\n");
        printf("9. Logout / Exit\n");
        printf("Masukkan Pilihan: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addNode(); break;
            case 2: editNode(); break;
            case 3: deleteNode(); break;
            case 4: addEdge(); break;
            case 5: editEdge(); break;
            case 6: deleteEdge(); break;
            case 7: displayGraph(); break;
            case 8: {
                char start[50], end[50];
                printf("Masukkan lokasi awal: ");
                scanf("%s", start);
                printf("Masukkan lokasi tujuan: ");
                scanf("%s", end);
                int startIdx = findNodeIndex(start);
                int endIdx = findNodeIndex(end);
                if (startIdx == -1 || endIdx == -1) {
                    printf("Lokasi tidak sesuai.\n");
                } else {
                    dijkstra(startIdx, endIdx);
                }
                break;
            }
            case 9: printf("Logging out...\n"); break;
            default: printf("Pilihan gagal.\n");
        }
    } while (choice != 9);
}

// Add a new node (location)
void addNode() {
    if (nodeCount >= MAX_NODES) {
        printf("Lokasi sudah limit.\n");
        return;
    }
    char name[50];
    printf("Masukkan nama lokasi: ");
    scanf("%s", name);
    if (findNodeIndex(name) != -1) {
        printf("Lokasi sudah ada.\n");
        return;
    }
    strcpy(nodes[nodeCount].name, name);
    adjList[nodeCount] = NULL;  // Initialize adjacency list
    nodeCount++;
    printf("Node berhasil ditambahkan.\n");
}

// Edit node name
void editNode() {
    char oldName[50], newName[50];
    printf("Masukkan nama lokasi saat ini: ");
    scanf("%s", oldName);
    int idx = findNodeIndex(oldName);
    if (idx == -1) {
        printf("node tidak ditemukan.\n");
        return;
    }
    printf("Masukkan nama lokasi baru: ");
    scanf("%s", newName);
    if (findNodeIndex(newName) != -1) {
        printf("Nama lokasi baru sudah ada.\n");
        return;
    }
    strcpy(nodes[idx].name, newName);
    printf("Node edited successfully.\n");
}

// Delete a node and all its edges
void deleteNode() {
    char name[50];
    printf("Enter location name to delete: ");
    scanf("%s", name);
    int idx = findNodeIndex(name);
    if (idx == -1) {
        printf("Node not found.\n");
        return;
    }
    // Free edges from this node
    struct Edge* temp = adjList[idx];
    while (temp) {
        struct Edge* next = temp->next;
        free(temp);
        temp = next;
    }
    adjList[idx] = NULL;
    // Shift nodes
    for (int i = idx; i < nodeCount - 1; i++) {
        nodes[i] = nodes[i + 1];
        adjList[i] = adjList[i + 1];
    }
    nodeCount--;
    // Update all edges pointing to nodes after idx
    for (int i = 0; i < nodeCount; i++) {
        struct Edge* curr = adjList[i];
        while (curr) {
            if (curr->dest > idx) {
                curr->dest--;
            } else if (curr->dest == idx) {
                // This shouldn't happen as we deleted the node, but just in case
                curr->dest = -1;  // Mark invalid
            }
            curr = curr->next;
        }
    }
    printf("Node deleted successfully.\n");
}

// Add an edge between two nodes
void addEdge() {
    char src[50], dest[50];
    int weight;
    printf("Enter source location: ");
    scanf("%s", src);
    printf("Enter destination location: ");
    scanf("%s", dest);
    printf("Enter weight (distance/time): ");
    scanf("%d", &weight);
    int srcIdx = findNodeIndex(src);
    int destIdx = findNodeIndex(dest);
    if (srcIdx == -1 || destIdx == -1) {
        printf("Invalid locations.\n");
        return;
    }
    // Check if edge already exists
    struct Edge* curr = adjList[srcIdx];
    while (curr) {
        if (curr->dest == destIdx) {
            printf("Edge already exists.\n");
            return;
        }
        curr = curr->next;
    }
    // Add edge
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->dest = destIdx;
    newEdge->weight = weight;
    newEdge->next = adjList[srcIdx];
    adjList[srcIdx] = newEdge;
    printf("Edge added successfully.\n");
}

// Edit edge weight
void editEdge() {
    char src[50], dest[50];
    int newWeight;
    printf("Enter source location: ");
    scanf("%s", src);
    printf("Enter destination location: ");
    scanf("%s", dest);
    printf("Enter new weight: ");
    scanf("%d", &newWeight);
    int srcIdx = findNodeIndex(src);
    int destIdx = findNodeIndex(dest);
    if (srcIdx == -1 || destIdx == -1) {
        printf("Invalid locations.\n");
        return;
    }
    struct Edge* curr = adjList[srcIdx];
    while (curr) {
        if (curr->dest == destIdx) {
            curr->weight = newWeight;
            printf("Edge edited successfully.\n");
            return;
        }
        curr = curr->next;
    }
    printf("Edge not found.\n");
}

// Delete an edge
void deleteEdge() {
    char src[50], dest[50];
    printf("Enter source location: ");
    scanf("%s", src);
    printf("Enter destination location: ");
    scanf("%s", dest);
    int srcIdx = findNodeIndex(src);
    int destIdx = findNodeIndex(dest);
    if (srcIdx == -1 || destIdx == -1) {
        printf("Invalid locations.\n");
        return;
    }
    struct Edge* curr = adjList[srcIdx];
    struct Edge* prev = NULL;
    while (curr) {
        if (curr->dest == destIdx) {
            if (prev) {
                prev->next = curr->next;
            } else {
                adjList[srcIdx] = curr->next;
            }
            free(curr);
            printf("Edge deleted successfully.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Edge not found.\n");
}

// Display all nodes and their edges
void displayGraph() {
    if (nodeCount == 0) {
        printf("Graph is empty.\n");
        return;
    }
    printf("Graph:\n");
    for (int i = 0; i < nodeCount; i++) {
        printf("%s -> ", nodes[i].name);
        struct Edge* curr = adjList[i];
        if (!curr) {
            printf("No edges\n");
        } else {
            while (curr) {
                printf("%s (%d) ", nodes[curr->dest].name, curr->weight);
                curr = curr->next;
            }
            printf("\n");
        }
    }
}

// Dijkstra's algorithm to find shortest path
void dijkstra(int start, int end) {
    if (nodeCount == 0) {
        printf("Graph is empty.\n");
        return;
    }
    // Initialize
    for (int i = 0; i < nodeCount; i++) {
        distance[i] = INF;
        visited[i] = 0;
        predecessor[i] = -1;
    }
    distance[start] = 0;

    for (int count = 0; count < nodeCount - 1; count++) {
        // Find min distance vertex
        int minDist = INF, u = -1;
        for (int v = 0; v < nodeCount; v++) {
            if (!visited[v] && distance[v] < minDist) {
                minDist = distance[v];
                u = v;
            }
        }
        if (u == -1) break;
        visited[u] = 1;

        // Update neighbors
        struct Edge* curr = adjList[u];
        while (curr) {
            int v = curr->dest;
            if (!visited[v] && distance[u] != INF && distance[u] + curr->weight < distance[v]) {
                distance[v] = distance[u] + curr->weight;
                predecessor[v] = u;
            }
            curr = curr->next;
        }
    }

    // Output
    if (distance[end] == INF) {
        printf("No route found from %s to %s.\n", nodes[start].name, nodes[end].name);
    } else {
        printf("Shortest path from %s to %s:\n", nodes[start].name, nodes[end].name);
        printf("Total distance: %d\n", distance[end]);
        // Reconstruct path
        int path[MAX_NODES], pathLen = 0;
        for (int at = end; at != -1; at = predecessor[at]) {
            path[pathLen++] = at;
        }
        printf("Path: ");
        for (int i = pathLen - 1; i >= 0; i--) {
            printf("%s", nodes[path[i]].name);
            if (i > 0) printf(" -> ");
        }
        printf("\n");
    }
}

// Helper function to find node index by name
int findNodeIndex(char* name) {
    for (int i = 0; i < nodeCount; i++) {
        if (strcmp(nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Free all adjacency list memory
void freeAdjList() {
    for (int i = 0; i < nodeCount; i++) {
        struct Edge* curr = adjList[i];
        while (curr) {
            struct Edge* next = curr->next;
            free(curr);
            curr = next;
        }
    }
}

