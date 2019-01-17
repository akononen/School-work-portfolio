// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <set>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <limits>
#include <map>

// Type for town IDs
using TownID = std::string;

// Return value for cases where required town was not found
TownID const NO_ID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
std::string const NO_NAME = "-- unknown --";

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

    // Estimate of performance: logn
    // Short rationale for estimate: Time complexity for std::map::find is logn.
    // Worst case the town cannot be found and speed is O(n)
    std::string get_name(TownID id);

    // Estimate of performance: logn
    // Short rationale for estimate: Time complexity for std::map::find is logn.
    // Worst case the town cannot be found and speed is O(n)
    std::pair<int, int> get_coordinates(TownID id);

    // Estimate of performance: logn
    // Short rationale for estimate: Time complexity for std::map::find is logn.
    // Worst case the town cannot be found and speed is O(n)
    int get_tax(TownID id);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std::map::find + std::sort
    // = logn + nlogn --> nlogn
    std::vector<TownID> get_vassals(TownID id);

    // Estimate of performance: n
    // Short rationale for estimate: Complexity of returning vector is n.
    std::vector<TownID> all_towns();

    // Estimate of performance: logn
    // Short rationale for estimate: Detailed complexity is std::map::find + std::map::insert +
    // std::vector::push_back + std::vactor::push_back = logn +logn + 1 + 1 --> logn
    bool add_town(TownID id, std::string const& name, int x, int y, int tax);

    // Estimate of performance: logn
    // Short rationale for estimate: Time complexity for std::map::find is logn.
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

    // Non-compulsory operations

    // Estimate of performance: n
    // Short rationale for estimate: All of the operations done here have complexity of n and when they are
    // summed we end up with n.
    bool remove_town(TownID id);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Detailed complexity is std::sort + going through vector + returning vector
    // = nlogn + n + n = nlogn.
    std::vector<TownID> towns_distance_increasing_from(int x, int y);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: n
    // Short rationale for estimate: std::map::find = logn, but on the other hand if all the towns are
    // connected with vassalships in straight line complexity is n. This is estimated to n just to be safe.
    int total_net_tax(TownID id);

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
    };

    struct TownDataVector{
        TownID id_;
        std::string name_;
        int x_;
        int y_;
    };

    std::map<TownID, TownDataMap> townmap_;
    std::vector<TownDataVector> townvector_;
    std::vector<TownID> orderedIDs_;
    bool alphasorted;
    bool distsorted;

    void findpathrec(std::vector<TownID>& path, TownDataMap town);
    int distance(int x1, int y1, int x2=0, int y2=0);
    int get_net_tax(TownDataMap *town);

//    void quickSortAlpha(int left, int right);
//    int partitionAlpha(int left, int right);
    void swapIDs(int a, int b);
};

#endif // DATASTRUCTURES_HH
