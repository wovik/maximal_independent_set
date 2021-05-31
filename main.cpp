#include <thread>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <random>
#include <chrono>


using namespace std;

struct Node
{
    int id;
    vector<Node*> neighbors;
    bool isMis = false;
};

static bool started = false;
static random_device rd;
static uniform_int_distribution<int> dist(5, 15);

void work(Node* node)
{
    while (!started);

    while (true)
    {
        chrono::milliseconds i3(dist(rd));
        this_thread::sleep_for(i3);

        bool anyMis = false;
        for (auto neigh : node->neighbors)
            anyMis = anyMis | neigh->isMis;

        chrono::milliseconds i(dist(rd));
        this_thread::sleep_for(i);

        if (anyMis && node->isMis)
            node->isMis = false;
        else if (!anyMis)
            node->isMis = true;

        chrono::milliseconds i2(dist(rd));
        this_thread::sleep_for(i2);
    }
}

int main()
{
    size_t numNodes;
    cin >> numNodes;
    vector<Node*> graph{numNodes};
    string str;
    getline(cin, str);
    for (int i = 0; i < numNodes; ++i)
    {
        graph[i] = new Node;
    }

    for (int i = 0; i < numNodes; ++i)
    {
        getline(cin, str);
        stringstream ss(str);
        auto start = istream_iterator<int>{ss};
        auto end= istream_iterator<int>{};
        vector<int> input(start, end);

        graph[i]->id = i;
        for (int v : input)
        {
            graph[i]->neighbors.push_back(graph[v]);
        }
    }

    vector<thread> threads;
    for (int i = 0; i < numNodes; ++i)
    {
        threads.emplace_back(work, graph[i]);
        threads[i].detach();
    }

    started = true;

    chrono::seconds i(5);
    this_thread::sleep_for(i);
    for (int i = 0; i < numNodes; ++i)
    {
        auto node = graph[i];
        cout << node->id << " { ";
        for (auto neigh : node->neighbors)
            cout << neigh->id << ' ';
        cout << "} " << node->isMis << endl;
    }
    return 0;
}