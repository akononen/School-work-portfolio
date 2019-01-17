// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>

struct TownData
{
    std::string name;
    int x;
    int y;
};

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: 1
    // Short rationale for estimate: Method only returns private value "vectorsize_" which is changed
    // when a town is added or removed.
    unsigned int size();

    // Estimate of performance: n
    // Short rationale for estimate: STL clear() function performance is n with trivial destructors.
    void clear();

    // Estimate of performance: 1
    // Short rationale for estimate: Only returns the vector.
    std::vector<TownData*> all_towns();

    // Estimate of performance: 1
    // Short rationale for estimate: Method only adds a pointer to the town to the townvector_ and returns
    // said pointer.
    TownData* add_town(std::string const& name, int x, int y);

    // Estimate of performance: nlogn
    // Short rationale for estimate: Recursive quicksort function. Worst case scenario performance is n^2.
    std::vector<TownData*> towns_alphabetically();

    // Estimate of performance: nlogn
    // Short rationale for estimate: Recursive quicksort function. Worst case scenario performance is n^2.
    std::vector<TownData*> towns_distance_increasing();

    // Estimate of performance: n
    // Short rationale for estimate: Accurate estimate is 0,5n because every position in vector is
    // equally likely to others.
    TownData* find_town(std::string const& name);

    // Estimate of performance: n
    // Short rationale for estimate: Algorithm goes through the townvector_ and returns the town that is
    // closest to the origo.
    TownData* min_distance();

    // Estimate of performance: n
    // Short rationale for estimate: Algorithm goes through the townvector_ and returns the town that is
    // furthest away from origo.
    TownData* max_distance();

    // Estimate of performance: nlogn
    // Short rationale for estimate: The algorithm sorts the vector ny distance first, and then returns the
    // nth town.
    TownData* nth_distance(unsigned int n);

    // Non-compulsory operations

    // Estimate of performance: n
    // Short rationale for estimate: The algorithm goes throug townvector and after it finds the right town.
    // it will erase it. Going through vector = n & erasing = n --> 2n
    void remove_town(std::string const& town_name);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<TownData*> towns_distance_increasing_from(int x, int y);


private:
    // Add stuff needed for your class implementation here
    std::vector<TownData*> townvector_;
    unsigned int vectorsize_;

    unsigned int distance(int x1, int y1, int x2, int y2);
    unsigned int distanceFromO(TownData* town);

    void quickSortAlpha(int left, int right);
    int partitionAlpha(int left, int right);

    void swapTowns(int i, int j);

    void quickSortDis(int left, int right);
    int partitionDis(int left, int right);
};

#endif // DATASTRUCTURES_HH
