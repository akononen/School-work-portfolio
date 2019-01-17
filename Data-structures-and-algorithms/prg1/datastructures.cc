// Datastructures.cc

#include "datastructures.hh"
#include <new>
#include <cmath>
#include <random>
#include <algorithm>
#include <vector>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}



Datastructures::Datastructures()
{
    vectorsize_ = 0;
}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::size()
{
    return vectorsize_;
}

void Datastructures::clear()
{
    townvector_.clear();
    vectorsize_ = 0;
}

std::vector<TownData*> Datastructures::all_towns()
{
    return townvector_;
}

TownData* Datastructures::add_town(const std::string& name, int x, int y)
{

    TownData* addedtown= new TownData;
    addedtown->name = name;
    addedtown->x = x;
    addedtown->y = y;

    townvector_.push_back(addedtown);
    vectorsize_ ++;

    return addedtown;
}

std::vector<TownData*> Datastructures::towns_alphabetically()
{

    quickSortAlpha(0, size()-1);
    return townvector_;
}

std::vector<TownData*> Datastructures::towns_distance_increasing()
{
    quickSortDis(0, size()-1);
    return townvector_;
}

TownData* Datastructures::find_town(std::string const& name)
{
    for (auto i : townvector_){
        if(i->name == name){
            return i;
        }
    }
    return nullptr;
}

TownData* Datastructures::min_distance()
{
    if (size() > 0){
        unsigned int j = 0;
        for (unsigned int i=0; i <= size()-1; i++){
            if (distanceFromO(townvector_[i]) < distanceFromO(townvector_[j])){
                j = i;
            }
        }
        return townvector_[j];
    }
    return nullptr;
}

TownData* Datastructures::max_distance()
{
    if (size() > 0){
        unsigned int j = 0;
        for (unsigned int i=0; i <= size()-1; i++){
            if (distanceFromO(townvector_[i]) > distanceFromO(townvector_[j])){
                j = i;
            }
        }
        return townvector_[j];
    }
    return nullptr;
}

TownData* Datastructures::nth_distance(unsigned int n)
{
    if (size() > 0){
        quickSortDis(0, size()-1);
        return townvector_[n-1];
    }
    return nullptr;
}

void Datastructures::remove_town(const std::string& town_name)
{
    if (size() > 0){
        for (unsigned int i=0; i <= size()-1; i++){
            if (townvector_[i]->name == town_name){
                townvector_.erase(townvector_.begin() + i);
                vectorsize_ --;
            }
        }
    }

}


std::vector<TownData*> Datastructures::towns_distance_increasing_from(int x, int y)
{
    return {}; // Replace with actual implementation
}

unsigned int Datastructures::distance(int x1, int y1, int x2, int y2)
{
    return  (std::abs(x1-x2) + std::abs(y1-y2));
}

unsigned int Datastructures::distanceFromO(TownData *town)
{
    return (std::abs(town->x) + std::abs(town->y));
}

void Datastructures::quickSortAlpha(int left, int right)
{
    if (left < right){
        int p = partitionAlpha(left, right);

        quickSortAlpha(left, p-1);
        quickSortAlpha(p+1, right);
    }
}


int Datastructures::partitionAlpha(int left, int right)
{

    std::string pivot = townvector_[right]->name;
    int i = (left-1);

    for (int j=left; j <= right-1; j++){
        std::string comparedName1 = townvector_[j]->name;
        std::transform(comparedName1.begin(), comparedName1.end(), comparedName1.begin(), ::tolower);
        std::transform(pivot.begin(), pivot.end(), pivot.begin(), ::tolower);

        if (comparedName1 <= pivot){
            i++;
            swapTowns(i, j);

        }
    }

    swapTowns(i+1, right);
    return (i+1);
}

void Datastructures::swapTowns(int i, int j)
{
    TownData* temptown = new TownData;
    temptown = townvector_[i];
    townvector_[i] = townvector_[j];
    townvector_[j] = temptown;


}

void Datastructures::quickSortDis(int left, int right)
{
    if (left < right){
        int p = partitionDis(left,right);

        quickSortDis(left, p-1);
        quickSortDis(p+1, right);
    }
}

int Datastructures::partitionDis(int left, int right)
{
    unsigned int pivot = distanceFromO(townvector_[right]);
    int i = (left-1);

    for (int j=left; j <= right-1; j++){
        if (distanceFromO(townvector_[j]) <= pivot){
            i++;
            swapTowns(i, j);
        }
    }
    swapTowns(i+1, right);
    return (i+1);
}
