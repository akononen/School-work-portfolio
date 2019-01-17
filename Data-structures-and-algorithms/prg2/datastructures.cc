// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <memory>
#include <algorithm>
#include <iostream>

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

}

Datastructures::~Datastructures()
{

}

unsigned int Datastructures::size()
{
    return townmap_.size();
}

void Datastructures::clear()
{

    townmap_.clear();
    townvector_.clear();
    orderedIDs_.clear();
}

std::string Datastructures::get_name(TownID id)
{   std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return it->second.name_;
    } else {
        return NO_NAME;
    }
}

std::pair<int, int> Datastructures::get_coordinates(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return std::pair<int, int>(it->second.x_, it->second.y_);

    } else {
        return {NO_VALUE, NO_VALUE};
    }

}

int Datastructures::get_tax(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return it->second.tax_;
    } else {
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::get_vassals(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){

        std::sort(it->second.vassalIDs_.begin(), it->second.vassalIDs_.end());
        return it->second.vassalIDs_;

    } else {
        return std::vector<TownID>{NO_ID};
    }
}

std::vector<TownID> Datastructures::all_towns()
{
    return orderedIDs_;
}

bool Datastructures::add_town(TownID id, const std::string& name, int x, int y, int tax)
{

    //If ID already exists, returns false.
    if (townmap_.find(id) != townmap_.end()) {
        return false;
    } else {
        TownDataMap towntoaddmap;
        towntoaddmap.id_ = id;
        towntoaddmap.name_ = name;
        towntoaddmap.tax_ = tax;
        towntoaddmap.x_ = x;
        towntoaddmap.y_ = y;

        //every town is root by default.
        towntoaddmap.isroot_ = true;


        townmap_.insert(std::pair<TownID, TownDataMap>(id, towntoaddmap));

        //This is done so other functions can be faster.
        orderedIDs_.push_back(id);
        alphasorted = false;
        distsorted = false;

        TownDataVector towntoaddvector;
        towntoaddvector.id_ = id;
        towntoaddvector.name_ = name;
        towntoaddvector.x_ = x;
        towntoaddvector.y_ = y;
        townvector_.push_back(towntoaddvector);

        return true;
    }
}

bool Datastructures::change_town_name(TownID id, const std::string& newname)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        it->second.name_ = newname;

        alphasorted = false;

        return true;
    } else {
        return false;
    }
}

bool Datastructures::remove_town(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it == townmap_.end()){
        return false;
    } else {
        if (it->second.isroot_){
            for( auto i : it->second.vassals_){
                i->master_ = nullptr;
                i->isroot_ = true;
            }
        } else {
            //vassals moved to master
            for( auto i : it->second.vassals_){
                i->master_ = it->second.master_;
                it->second.master_->vassalIDs_.push_back(i->id_);
                it->second.master_->vassals_.insert(i);
            }
            //town erased from masters vassal set
            for( auto i : it->second.master_->vassals_){
                if( i->id_ == id){
                    it->second.master_->vassals_.erase(i);
                    break;
                }
            }
            //town erased from masters vassalIDs vector
            for( auto i = 0; i != it->second.master_->vassalIDs_.size(); i++){
                if( it->second.master_->vassalIDs_.at(i) == id){
                    it->second.master_->vassalIDs_.erase(it->second.master_->vassalIDs_.begin()+i);
                }
            }

            }
        //town erased from townvector
        for( auto i = 0; i != townvector_.size(); i++){
            if ( townvector_.at(i).id_ == id){
                townvector_.erase(townvector_.begin()+i);
                break;
            }
        }
        //town erased from orderedIDs vector
        for( auto i=0; i != orderedIDs_.size(); i++){
            if( orderedIDs_.at(i) == id){
                orderedIDs_.erase(orderedIDs_.begin()+i);
                break;
            }
        }
        townmap_.erase(it);
        alphasorted = false;
        distsorted = false;
        return true;
    }
}


std::vector<TownID> Datastructures::towns_alphabetically()
{

    if (alphasorted){
        return orderedIDs_;
    } else {
        std::sort(townvector_.begin(), townvector_.end(), [this](TownDataVector a, TownDataVector b){
            return a.name_ < b.name_;
        });

        for ( int i = 0; i != size(); i++){
            orderedIDs_.at(i) = townvector_.at(i).id_;
        }
        alphasorted = true;
        distsorted = false;
        return orderedIDs_;
    }
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if (distsorted){
        return orderedIDs_;
    } else {
        std::sort(townvector_.begin(), townvector_.end(), [this](TownDataVector a, TownDataVector b){
            return (distance(a.x_, a.y_) < distance(b.x_, b.y_));
        });

        for ( int i=0; i!=size(); i++){
            orderedIDs_.at(i) = townvector_.at(i).id_;
        }
        alphasorted = false;
        distsorted = true;
        return orderedIDs_;
    }
}

std::vector<TownID> Datastructures::find_towns(std::string const& name)
{
    std::vector<TownID> returnvector {};

    std::map<TownID, TownDataMap>:: iterator it = townmap_.begin();

    while( it != townmap_.end()){
        if( it->second.name_ == name){
            returnvector.push_back(it->first);
        }
        it++;
    }
    std::sort(returnvector.begin(), returnvector.end());
    return returnvector;
}

TownID Datastructures::min_distance()
{
    if (size() == 0){
        return NO_ID;
    } else {
        if ( distsorted ){
            return orderedIDs_.front();
        } else {
            int mindist;
            std::map<TownID, TownDataMap>:: iterator it = townmap_.begin();
            mindist = distance(it->second.x_, it->second.y_);
            TownID returnid = it->first;
            it++;
            while (it != townmap_.end()){
                int comparedist = distance(it->second.x_, it->second.y_);
                if (comparedist < mindist){
                    mindist = comparedist;
                    returnid = it->first;
                }
                it++;
            }
            return returnid;
        }
    }
}

TownID Datastructures::max_distance()
{
    if (size() == 0){
        return NO_ID;
    } else {
        if ( distsorted ){
            return orderedIDs_.back();
        } else {
            int maxdist;
            std::map<TownID, TownDataMap>:: iterator it = townmap_.begin();
            maxdist = distance(it->second.x_, it->second.y_);
            TownID returnid = it->first;
            it++;
            while (it != townmap_.end()){
                int comparedist = distance(it->second.x_, it->second.y_);
                if (comparedist > maxdist){
                    maxdist = comparedist;
                    returnid = it->first;
                }
                it++;
            }
            return returnid;
        }
    }
}

TownID Datastructures::nth_distance(unsigned int n)
{
    if (size() == 0){
        return NO_ID;
    } else {
        if ( distsorted ){
            return orderedIDs_.at(n-1);
        } else {
            towns_distance_increasing();
            return orderedIDs_.at(n-1);
        }
    }
}

std::vector<TownID> Datastructures::towns_distance_increasing_from(int x, int y)
{
    std::sort(townvector_.begin(), townvector_.end(), [this, x, y](TownDataVector a, TownDataVector b){
        return (distance(a.x_, a.y_, x, y) < distance(b.x_, b.y_, x, y));
    });

    for ( auto i=0; i!=size(); i++){
        orderedIDs_.at(i) = townvector_.at(i).id_;
    }
    alphasorted = false;
    distsorted = false;
    return orderedIDs_;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    std::map<TownID, TownDataMap>:: iterator vassal_iterator = townmap_.end();
    std::map<TownID, TownDataMap>:: iterator master_iterator = townmap_.end();
    std::map<TownID, TownDataMap>:: iterator i = townmap_.begin();

    //first must check that both towns can be found

    if (vassalid == masterid){
        return false;
    } else {
        while (i != townmap_.end()){

            if (i->first == vassalid){
                vassal_iterator = i;
            }
            if (i->first == masterid) {
                master_iterator = i;
            }
            i++;
            //will stop the loop if vassal and master have been found for efficency reasons
            if (vassal_iterator != townmap_.end() && master_iterator != townmap_.end()){
                i = townmap_.end();
            }
        }
        if (master_iterator != townmap_.end() && vassal_iterator != townmap_.end()){
            if ( vassal_iterator->second.isroot_){


                vassal_iterator->second.master_ = &master_iterator->second;
            //The vassal cannot be a root
                vassal_iterator->second.isroot_ = false;

                master_iterator->second.vassals_.insert(&vassal_iterator->second);
                master_iterator->second.vassalIDs_.push_back( vassalid );

            return true;

            } else {
                //The vassal has a master already
                return false;
            }
        } else {
            //vassal or master cannot be found
            return false;
        }
    }
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it == townmap_.end()){
        return {};
    } else {
        std::vector<TownID> path;
        findpathrec(path, it->second);
        return path;
    }
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    return {}; // Replace with actual implementation
}

int Datastructures::total_net_tax(TownID id)
{
    std::map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it == townmap_.end()){
        return NO_VALUE;
    } else {
        int net_tax = it->second.tax_;
        for (auto town : it->second.vassals_){

            net_tax += get_net_tax(town);

        }
        //if not root, the town has to pay taxes.
        if (it->second.isroot_){
            return net_tax;
        } else {
            return 0.9*net_tax;
        }
    }
}

void Datastructures::findpathrec(std::vector<TownID> &path, TownDataMap town)
{

    path.push_back(town.id_);
    //if the town is not root, the recursion continues
    if ( town.isroot_ == false){
        findpathrec(path, *town.master_);
    }
}

int Datastructures::distance(int x1, int y1, int x2, int y2)
{
    return  (std::abs(x1-x2) + std::abs(y1-y2));
}

int Datastructures::get_net_tax(TownDataMap* town)
{

    int returntax = 0;
    int vassaltax = 0;
    for (auto i : town->vassals_){
        vassaltax = get_net_tax(i);
    }
    returntax += 0.1*(town->tax_ + vassaltax);
    return returntax;

}

