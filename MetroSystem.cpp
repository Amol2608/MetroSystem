#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <algorithm>
#include <ctime>
#include <unordered_set>
#include <cstdlib>

using namespace std;

class MetroSystem {
private:
    map<int, vector<pair<int, int>>> metroMap; // station number, (connected station number, distance)
    map<int, string> stationNames;
    vector<vector<int>> adjMatrix;
    vector<vector<int>> distances;
    int numStations;
    unordered_set<int> ticketNumbers;

    void addEdge(int u, int v, int distance) {
        adjMatrix[u][v] = 1;
        adjMatrix[v][u] = 1;
        distances[u][v] = distance;
        distances[v][u] = distance;
    }

    int bfs(int src, int dest, vector<int> &path) {
        vector<bool> visited(numStations, false);
        vector<int> dist(numStations, -1);
        vector<int> parent(numStations, -1);
        queue<int> q;

        visited[src] = true;
        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (u == dest)
                break;

            for (int v = 0; v < numStations; ++v) {
                if (adjMatrix[u][v] && !visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }

        if (dist[dest] == -1) {
            return -1; // no path found
        }

        // Store path
        int crawl = dest;
        path.push_back(crawl);
        while (parent[crawl] != -1) {
            path.push_back(parent[crawl]);
            crawl = parent[crawl];
        }
        reverse(path.begin(), path.end());

        return dist[dest];
    }

    int calculateTotalDistance(const vector<int> &path) {
        int totalDistance = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            totalDistance += distances[path[i]][path[i + 1]];
        }
        return totalDistance;
    }

    int generateUniqueTicketNumber() {
        int ticketNumber;
        do {
            ticketNumber = rand() % 900000 + 100000; // Generate a random 6-digit ticket number
        } while (ticketNumbers.find(ticketNumber) != ticketNumbers.end());
        ticketNumbers.insert(ticketNumber);
        return ticketNumber;
    }

    string getCurrentDate() {
        time_t t = time(0);
        tm* now = localtime(&t);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d", now);
        return buf;
    }

    string getPlatform(int station) {
        // Simulate platform information: alternate between 1 and 2 for simplicity
        return (station % 2 == 0) ? "Platform 1" : "Platform 2";
    }

public:
    MetroSystem(vector<string> stations) {
        numStations = stations.size();
        adjMatrix = vector<vector<int>>(numStations, vector<int>(numStations, 0));
        distances = vector<vector<int>>(numStations, vector<int>(numStations, 0));

        for (int i = 0; i < numStations; ++i) {
            stationNames[i] = stations[i];
        }
    }

    void addConnection(int station1, int station2, int distance) {
        addEdge(station1, station2, distance);
    }

    void showFareAndRoute(int src, int dest) {
        vector<int> path;

        int stationsBetween = bfs(src, dest, path);

        if (stationsBetween == -1) {
            cout << "No route found between " << stationNames[src] << " and " << stationNames[dest] << ".\n";
            return;
        }

        cout << "Route from " << stationNames[src] << " to " << stationNames[dest] << ":\n";
        for (int i : path) {
            cout << stationNames[i];
            if (i != path.back()) cout << " -> ";
        }
        cout << "\nNumber of stations between: " << stationsBetween << "\n";

        int totalDistance = calculateTotalDistance(path);
        cout << "Total distance: " << totalDistance << " km\n";

        double fare = calculateFare(totalDistance);
        cout << "Total fare: $" << fare << "\n";
    }

    double calculateFare(int totalDistance) {
        // Example fare calculation: $0.5 per km
        return totalDistance * 0.5;
    }

    void showRules() {
        cout << "Metro System Rules:\n";
        cout << "1. No eating or drinking on the train.\n";
        cout << "2. Give priority seating to elderly, disabled, and pregnant passengers.\n";
        cout << "3. Keep the noise level to a minimum.\n";
        cout << "4. Have your ticket or pass ready for inspection.\n";
        cout << "5. Follow the instructions of metro staff at all times.\n";
    }

    void showTicketInfo() {
        cout << "Metro Ticket Information:\n";
        cout << "1. Single journey tickets are valid for one trip between any two stations.\n";
        cout << "2. Daily passes are available for unlimited travel within a day.\n";
        cout << "3. Monthly passes offer unlimited travel for a month.\n";
        cout << "4. Tickets can be purchased at ticket counters or vending machines at stations.\n";
    }

    void listStations() {
        cout << "Available Stations:\n";
        for (const auto &station : stationNames) {
            cout << station.first << ". " << station.second << "\n";
        }
    }

    void bookTicket(int src, int dest) {
        vector<int> path;

        int stationsBetween = bfs(src, dest, path);

        if (stationsBetween == -1) {
            cout << "No route found between " << stationNames[src] << " and " << stationNames[dest] << ".\n";
            return;
        }

        int totalDistance = calculateTotalDistance(path);
        double fare = calculateFare(totalDistance);
        int ticketNumber = generateUniqueTicketNumber();
        string date = getCurrentDate();
        string platform = getPlatform(src);

        cout << "\n--- Ticket Booking ---\n";
        cout << "Ticket Number: " << ticketNumber << "\n";
        cout << "Date: " << date << "\n";
        cout << "From: " << stationNames[src] << " (" << platform << ")\n";
        cout << "To: " << stationNames[dest] << "\n";
        cout << "Total Distance: " << totalDistance << " km\n";
        cout << "Total Fare: $" << fare << "\n";
        cout << "----------------------\n";
    }
};

int main() {
    cout << "Welcome to Delhi Metro!\n";

    vector<string> stations = {
        "Dwarka Sector 21", "Dwarka Sector 8", "Dwarka Sector 9", "Dwarka Sector 10",
        "Dwarka Sector 11", "Dwarka Sector 12", "Dwarka Sector 13", "Dwarka Sector 14",
        "Dwarka", "Nawada", "Uttam Nagar West", "Uttam Nagar East", "Janakpuri West",
        "Janakpuri East", "Tilak Nagar", "Subhash Nagar", "Tagore Garden",
        "Rajouri Garden", "Ramesh Nagar", "Moti Nagar", "Kirti Nagar", "Shadipur",
        "Patel Nagar", "Rajendra Place", "Karol Bagh", "Jhandewalan", "Ramakrishna Ashram Marg",
        "Rajiv Chowk", "Patel Chowk", "Central Secretariat"
    };

    MetroSystem metro(stations);

    // Adding connections with distances in km (approximate values)
    metro.addConnection(0, 1, 1.2);
    metro.addConnection(1, 2, 1.1);
    metro.addConnection(2, 3, 0.9);
    metro.addConnection(3, 4, 1.0);
    metro.addConnection(4, 5, 1.2);
    metro.addConnection(5, 6, 1.1);
    metro.addConnection(6, 7, 1.0);
    metro.addConnection(7, 8, 1.5);
    metro.addConnection(8, 9, 1.7);
    metro.addConnection(9, 10, 1.5);
    metro.addConnection(10, 11, 1.4);
    metro.addConnection(11, 12, 1.3);
    metro.addConnection(12, 13, 1.2);
    metro.addConnection(13, 14, 1.0);
    metro.addConnection(14, 15, 1.2);
    metro.addConnection(15, 16, 1.0);
    metro.addConnection(16, 17, 1.1);
    metro.addConnection(17, 18, 1.3);
    metro.addConnection(18, 19, 1.2);
    metro.addConnection(19, 20, 1.1);
    metro.addConnection(20, 21, 1.0);
    metro.addConnection(21, 22, 1.4);
    metro.addConnection(22, 23, 1.1);
    metro.addConnection(23, 24, 1.2);
    metro.addConnection(24, 25, 1.0);
    metro.addConnection(25, 26, 1.5);
    metro.addConnection(26, 27, 0.9);
    metro.addConnection(27, 28, 1.0);

    srand(time(0)); // Seed for random number generation

    while (true) {
        cout << "\nWhat would you like to do?\n";
        cout << "1. Show Metro Rules\n";
        cout << "2. Show Ticket Info\n";
        cout << "3. Take a Ride\n";
        cout << "4. Book a Ticket\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 5) {
            break;
        }

        switch (choice) {
            case 1:
                metro.showRules();
                break;
            case 2:
                metro.showTicketInfo();
                break;
            case 3:
                metro.listStations();
                int src, dest;
                cout << "Enter source station number: ";
                cin >> src;
                cout << "Enter destination station number: ";
                cin >> dest;
                metro.showFareAndRoute(src, dest);
                break;
            case 4:
                metro.listStations();
                cout << "Enter source station number: ";
                cin >> src;
                cout << "Enter destination station number: ";
                cin >> dest;
                metro.bookTicket(src, dest);
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    cout << "Thank you for using the Delhi Metro System!\n";

    return 0;
}
