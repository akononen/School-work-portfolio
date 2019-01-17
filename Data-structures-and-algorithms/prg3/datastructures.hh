// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <unordered_map>
#include <set>

// Type for town IDs
using TownID = std::string;

// Return value for cases where required town was not found
TownID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

// Type for a coordinate (x, y)
using Coord = std::pair<int, int>;

// Return value for cases where coordinates were not found
Coord const NO_COORD{NO_VALUE, NO_VALUE};

// Type for distance
using Dist = int;

// Return value for cases where distance is unknown
Dist const NO_DIST = NO_VALUE;


class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: 1
    // Short rationale for estimate: From cppreference.
    unsigned int size();

    // Estimate of performance: logn
    // Short rationale for estimate: std::map::clear complexity is logn
    // and it is the dominant factor in this function.
    void clear();

    // Estimate of performance: 1
    // Short rationale for estimate: Time complexity for std::unordered_map::find is 1.
    // Worst case the town cannot be found and speed is O(n)
    std::string get_name(TownID id);

    // Estimate of performance: 1
    // Short rationale for estimate: Time complexity for std::unordered_map::find is 1.
    // Worst case the town cannot be found and speed is O(n)
    Coord get_coordinates(TownID id);
    // Estimate of performance: 1
    // Short rationale for estimate: Time complexity for std::unordered_map::find is 1.
    // Worst case the town cannot be found and speed is O(n)
    int get_tax(TownID id);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std:unordered_map::find + std::sort
    // = 1 + nlogn --> nlogn
    std::vector<TownID> get_vassals(TownID id);

    // Estimate of performance: n
    // Short rationale for estimate: Complexity of returning vector is n.
    std::vector<TownID> all_towns();

    // Estimate of performance: 1
    // Short rationale for estimate: Detailed complexity is std::unordered_map::find + std::unordered_map::insert +
    // std::vector::push_back + std::vactor::push_back = 1 + 1 + 1 + 1 --> 1
    bool add_town(TownID id, std::string const& name, int x, int y, int tax);

    // Estimate of performance: 1
    // Short rationale for estimate: Time complexity for std::unordered_map::find is 1.
    // Worst case the town cannot be found and speed is O(n)
    bool change_town_name(TownID id, std::string const& newname);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std::sort + going through vector + returning vector
    // = nlogn + n + n = nlogn.
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std::sort + going through vector + returning vector
    // = nlogn + n + n = nlogn.
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is going through all towns + std::sort
    // = n + nlogn -->nlogn
    std::vector<TownID> find_towns(std::string const& name);

    // Estimate of performance: n
    // Short rationale for estimate: Iterator goes through all towns to find mindistance.
    // If already sorted, complexity = 1
    TownID min_distance();

    // Estimate of performance: n
    // Short rationale for estimate: Iterator goes through all towns to find maxdistance.
    // If already sorted, complexity = 1
    TownID max_distance();

    // Estimate of performance: nlogn
    // Short rationale for estimate: Bestcase scenario constant time, but if the towns are not
    // sorted by distance, the towns will be sorted and then the nth town will be found.
    TownID nth_distance(unsigned int n);

    // Estimate of performance: n
    // Short rationale for estimate: Goes through all towns once, and hold iterator for
    // vassal and master and makes the connection.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: n
    // Short rationale for estimate: std::map::find = logn, but on the other hand if all the towns are
    // connected with vassalships in straight line complexity is n. This is estimated to n just to be safe.
    std::vector<TownID> taxer_path(TownID id);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Sorting operation is the dominant one. Getting all roads (=n), sorting (=2nlogn) and returning (=n).
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: 1
    // Short rationale for estimate: Finding the town =1
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: n
    // Short rationale for estimate: Finds the towns (=n) and checks if the road exists (=1). --> n
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: n
    // Short rationale for estimate: Finds the towns (=n) and goes through both towns adjacent towns. We can assume
    // that the number of adjacent towns is <n therefore the complexity is n.
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: n
    // Short rationale for estimate: Goes through every town, and destroys connected roads.
    void clear_roads();

    // Estimate of performance: V+E
    // Short rationale for estimate: If there are very little roads, the complexity is n as it makes every node "white".
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory operations

    // Estimate of performance: n
    // Short rationale for estimate: All of the operations done here have complexity of n and when they are
    // summed we end up with n.
    bool remove_town(TownID id);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std::sort + going through vector + returning vector
    // = nlogn + n + n = nlogn
    std::vector<TownID> towns_distance_increasing_from(int x, int y);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: n
    // Short rationale for estimate: std::map::find = logn, but on the other hand if all the towns are
    // connected with vassalships in straight line complexity is n. This is estimated to n just to be safe.
    int total_net_tax(TownID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance: V+E
    // Short rationale for estimate: Recursive DFS dunction. Again if there are very little roads, the complexity
    // is n as it makes every node "white" before starting.
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance:
    // Short rationale for estimate:
    Dist trim_road_network();

private:
    // Add stuff needed for your class implementation here
    struct TownDataMap{
        TownID id_;
        std::string name_;
        int x_;
        int y_;
        int tax_;
        std::set<TownDataMap*> vassals_;
        std::vector<TownID> vassalIDs_;
        TownDataMap* master_;
        bool isroot_;

        //road network implementation
        std::vector<TownDataMap*> adjacentTowns;
        bool iswhite_;
        bool isgrey_;
        bool isblack_;

        //needed for least roads
        unsigned int distance;
        TownDataMap* last;

    };

    // Used for sorting the towns according to name or distance
    struct TownDataVector{
        TownID id_;
        std::string name_;
        int x_;
        int y_;
    };

    std::unordered_map<TownID, TownDataMap> townmap_;
    std::vector<TownDataVector> townvector_;
    std::vector<TownID> orderedIDs_;
    bool alphasorted;
    bool distsorted;

    void findpathrec(std::vector<TownID>& path, TownDataMap town);
    int distance(int x1, int y1, int x2=0, int y2=0);
    int get_net_tax(TownDataMap *town);

    std::vector<TownID> anyrouteBFS(TownID starttown, TownID endtown);
    void DFSrec(TownDataMap &town, std::vector<TownID> &path, TownID endtown);
    void cyclerec(TownDataMap &town, std::vector<TownID> &path, TownDataMap *lasttown);
    void get_least_towns_path(std::vector<TownID> &path, TownDataMap currenttown, TownDataMap *lasttown);

    void swapIDs(int a, int b);
};


#endif // DATASTRUCTURES_HH
