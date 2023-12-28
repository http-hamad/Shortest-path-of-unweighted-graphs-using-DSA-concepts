#include <iostream>
using namespace std;

// #define MAX 100

// ! Functions

int StoI(string str) // * Checks if string is a valid Integer or Not
{
    for (char c : str)
    {
        if (!isdigit(c))
            return -1;
    }

    int number = 0;

    for (char c : str)
    {
        number *= 10;
        number += c - 48;
    }
    return number;
}

// ! Queue Implementation

class QueueNode
{
public:
    int data;
    QueueNode *next;

    QueueNode(int d) : data(d) { next = NULL; } // * Parametrized Constructor
};

class Queue
{
    QueueNode *head;
    QueueNode *tail;
    int size;

public:
    Queue() : size(0)
    {
        head = NULL;
        tail = NULL;
    }

    void Enqueue(int data) // * Enqueues the data
    {
        QueueNode *nn = new QueueNode(data);

        if (head == NULL && tail == NULL) // * If the queue is empty
        {
            head = nn;
            tail = nn;
            nn->next = nn;
        }
        else
        {
            tail->next = nn;
            tail = nn;
            nn->next = head;
        }
    }

    void Dequeue() // * Dnqueues the data
    {
        if (head == NULL) // * If Queue is Empty
        {
            cout << "Queue is Empty" << endl;
            tail = NULL;
        }
        else if (head == tail) // * If 1 element in Queue
        {
            // delete head;
            head = NULL;
            tail = NULL;
        }
        else
        {
            QueueNode *temp = head;
            tail->next = head->next;
            head = head->next;

            temp->next = NULL;
            delete temp;
        }
    }

    void display() // * Displays the queue
    {
        QueueNode *temp = head;

        do
        {

            cout << temp->data << " ";
            temp = temp->next;

        } while (temp != head);

        cout << endl;
    }

    bool empty() // * Checks if Queue is empty
    {
        return head == NULL;
    }

    int front() // * Returns the front element of the queue
    {
        if (!empty())
            return head->data;
        else
        {
            cout << "Queue is empty" << endl;
            return -1;
        }
    }
};

// ! Graph Implementation

struct Node
{
    int dist;
    int weight;
    Node *next;

    Node(int dist, Node *n = nullptr) : dist(dist), weight(1) // * Parameterized contructor
    {
        next = nullptr;
    }
};

struct Adjlist
{
    Node *head; // * Used for Adjacency list
};

class Graph
{
    int V;
    Adjlist *array; // * Adjacency list

public:
    Graph() : V(0) // * Default constructor
    {
        array = nullptr;
    }

    void createGraph(int v) // * Creating a new graph
    {
        this->V = v;
        this->array = new Adjlist[v];

        for (int i = 0; i < v; i++)
            array[i].head = nullptr;
    }

    void addEdge(int src, int dst) // * Adding Edges, unidirectional
    {
        Node *nn = new Node(dst);
        nn->next = array[src].head;
        array[src].head = nn;
    }

    void printGraph() // * Printing the graph
    {
        Node *temp = nullptr;

        for (int i = 0; i < V; i++)
        {
            temp = array[i].head; // * Get Vertex
            cout << i << " : ";

            while (temp)
            {
                cout << temp->dist << " "; // * Print each connection
                temp = temp->next;
            }

            cout << endl;
        }
    }

    // * Main Attempt. With Queue [Optimized]
    int shortestPath(int start, int dest)
    {
        Queue q;
        int parent[this->V], curr_key;
        bool visited[this->V];
        int current = dest, prev = dest + 1, days = 0, count = 0;
        bool found = false;

        for (int i = 0; i < V; i++) // * Initializing variables
        {
            parent[i] = -1;
            visited[i] = false;
        }

        q.Enqueue(start);

        while (!q.empty() && !found) // * Run until Queue is empty
        {
            curr_key = q.front();
            q.Dequeue();
            visited[curr_key] = true;

            Node *temp = array[curr_key].head;

            while (temp) // * Run until all connections of a vertex have been visited
            {
                if (!visited[temp->dist]) // * If not visited
                {
                    visited[temp->dist] = true;
                    parent[temp->dist] = curr_key;
                    q.Enqueue(temp->dist);
                }

                // if (temp->dist == dest || temp->dist == dest - 1)
                if (temp->dist == dest) // * For increased optimization
                {
                    found = true; // ? comment this incase code doesn't work
                    break;
                }

                temp = temp->next;
            }
        }

        // * If the destination hasn't been visited, there's no path
        // if (!visited[dest] || (parent[dest] == -1))
        if (!visited[dest])
        {
            cout << "No path from " << start << " to " << dest << endl;
            cout << "\nDays : -1" << endl;
            return -1;
        }

        cout << "\nShortest path from " << start + 1 << " to " << dest + 1 << " :-  ";
        // do
        while (current != start) // * Calculates totals days required
        {
            if (prev - 1 != current || (count % 7 == 0 && current != dest)) // * If aerial route was taken of 6 cities traversed
            {
                days++;
                count = 0;
            }

            cout << current + 1 << " <- ";
            prev = current;
            current = parent[current];
            count++;
        }
        cout << current + 1 << endl;

        days++;
        cout << "\nDays : " << days << endl;
        return days;
    }

    // * No Queue, Just For loops
    int shortestPath2(int start, int end)
    {
        int dist[this->V];
        int parent[this->V];
        bool visited[this->V] = {false};

        for (int i = 0; i < V; ++i) // * Initializing variables
        {
            dist[i] = INT_MAX;
            parent[i] = -1;
        }

        dist[start] = 0;

        for (int i = 0; i < V - 1; ++i) // * Traversing Each Vertax
        {
            int minDist = INT_MAX;
            int current;

            for (int v = 0; v < V; ++v) // * Finding the current vertex
            {
                if (!visited[v] && dist[v] <= minDist)
                {
                    minDist = dist[v];
                    current = v;
                }
            }

            visited[current] = true;

            Node *temp = array[current].head;
            while (temp != nullptr) // * Checking every connection
            {
                int neighbor = temp->dist;
                int weight = temp->weight;

                if (!visited[neighbor] && dist[current] != INT_MAX && dist[current] + weight < dist[neighbor]) // * Checking to see if a shorter path has been found
                {
                    parent[neighbor] = current;
                    dist[neighbor] = dist[current] + weight;
                }

                temp = temp->next;
            }
        }

        // * If the destination hasn't been visited, there's no path
        // if (!visited[end] || (parent[end] == -1))
        if (!visited[end])
        {
            cout << "No path from " << start + 1 << " to " << end + 1 << endl;
            cout << "\nDays : -1" << endl;
            return -1;
        }

        int current = end, prev = end + 1, days = 0, count = 0;
        cout << "\nShortest path from " << start + 1 << " to " << end + 1 << " :-  ";
        // do
        while (current != start) // * Calculating total days required
        {
            if (prev - 1 != current || (count % 7 == 0 && current != end))
            {
                days++;
                count = 0;
            }

            cout << current + 1 << " <- ";
            prev = current;
            current = parent[current];
            count++;
        }
        cout << current + 1 << endl;

        days++;
        cout << "\nDays : " << days << endl;
        return days;
    }
};

// ! Algorithm Testing

void city_traversal()
{
    int num_vertices = 0, flights = 0, v1 = -1, v2 = -1;
    string num_vertices_str = "";

    cout << "Enter Number of vertices: ";
    cin >> num_vertices_str;
    // cin >> num_vertices;

    num_vertices = StoI(num_vertices_str);

    while (num_vertices == -1)
    {
        cout << "\nERROR: Incorrect Input" << endl;
        cout << "Enter Number of vertices in Integers and >= 0: ";
        cin >> num_vertices_str;

        num_vertices = StoI(num_vertices_str);
    }

    if (num_vertices == 0) // * No cities
    {
        cout << "\nOutput: \n";
        cout << "No Cities to Traverse.\n";
        // return 0;
        return;
    }

    Graph graph;
    graph.createGraph(num_vertices);

    for (int i = 0; i < num_vertices; i++) // * Connecting the cities via road
        for (int j = 0; j < num_vertices; j++)
            if (i + 1 == j)
                graph.addEdge(i, j);

    cout << "\nEnter Number of Aerial routes: ";
    cin >> flights;

    // * Used to ignore additional inputs
    cin.sync();

    cout << endl;

    for (int i = 0; i < flights; i++) // * Connecting the Aerial routes
    {
        cout << "Edge " << i + 1 << ", Enter Vertice 1 and 2: ";
        cin >> v1 >> v2;

        while (v1 < 1 || v2 < 1)
        // while (v1 < 0 || v2 < 0)
        {
            cout << "Both edge must be equal to or greater than 1: ";
            // cout << "Both edge must be equal to or greater than 0: ";
            cin >> v1 >> v2;
        }
        while (v1 > num_vertices || v2 > num_vertices)
        // while (v1 >= num_vertices || v2 >= num_vertices)
        {
            cout << "Both edge must be equal to or less than total vertices[" << num_vertices << "] : ";
            // cout << "Both edge must be less than total vertices[" << num_vertices << "] : ";
            cin >> v1 >> v2;
        }

        graph.addEdge(v1 - 1, v2 - 1);

        // if (v1 - 1 >= 0)
        //     v1 = v1 - 1;
        // graph.addEdge(v1, v2);
    }

    cout << "\nOutput : " << endl;
    graph.shortestPath(0, num_vertices - 1); // * Calculates shortest path and return total days required.

    // graph.shortestPath2(0, num_vertices - 1);
}

int main()
{
    cout << "\nWelcome to the Shortest Path Calculator" << endl
         << "-----------------------------------------------------" << endl
         << endl;

    int test_cases = 0;
    string test_cases_str = "";

    cout << "Enter Number of Tests Cases: ";
    cin >> test_cases_str;
    // cin >> test_cases;

    test_cases = StoI(test_cases_str);

    while (test_cases == -1)
    {
        cout << "\nERROR: Incorrect Input" << endl;
        cout << "Enter Number of Test cases in Integers and >= 0: ";
        cin >> test_cases_str;

        test_cases = StoI(test_cases_str);
    }

    cin.sync();

    for (int i = 0; i < test_cases; i++) // * Runs for all tests cases
    {
        cout << "\n---------------------\n";
        cout << "\nTest Case " << i + 1 << " : " << endl
             << endl;

        city_traversal();
    }

    cout << "\n\nExiting..." << endl
         << endl
         << "-----------------------------------------------------" << endl
         << endl;

    return 0;
}
