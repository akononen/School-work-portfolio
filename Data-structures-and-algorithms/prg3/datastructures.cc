// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <iostream>
#include <cmath>
#include <list>
#include <stack>
#include <queue>
#include <algorithm>
#include <unordered_map>
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
 // Replace with actual implementation
}

Datastructures::~Datastructures()
{
 // Replace with actual implementation
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
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return it->second.name_;
    } else {
        return NO_NAME;
    }
}

Coord Datastructures::get_coordinates(TownID id)
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return std::pair<int, int>(it->second.x_, it->second.y_);

    } else {
        return {NO_VALUE, NO_VALUE};
    }
}

int Datastructures::get_tax(TownID id)
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        return it->second.tax_;
    } else {
        return NO_VALUE;
    }
}

std::vector<TownID> Datastructures::get_vassals(TownID id)
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
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
        towntoaddmap.adjacentTowns = {};
        towntoaddmap.iswhite_ = true;
        towntoaddmap.isgrey_ = false;


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
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
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

    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
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

    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();

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
            std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
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
            std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
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
    std::unordered_map<TownID, TownDataMap>:: iterator vassal_iterator = townmap_.end();
    std::unordered_map<TownID, TownDataMap>:: iterator master_iterator = townmap_.end();
    std::unordered_map<TownID, TownDataMap>:: iterator i = townmap_.begin();

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
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it == townmap_.end()){
        return {};
    } else {
        std::vector<TownID> path;
        findpathrec(path, it->second);
        return path;
    }
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    std::vector<std::pair<TownID, TownID>> roadsvector;
    //getting every road to teh vector (twice)
    for ( auto it : townmap_){
        for ( auto i : it.second.adjacentTowns){
            if(it.first < i->id_){
                roadsvector.push_back(std::make_pair(it.first, i->id_));
            } else{
                roadsvector.push_back(std::make_pair(i->id_, it.first));
            }
        }
    }

    //sorting the vector and erasing duplicates.
    std::sort(roadsvector.begin(), roadsvector.end());
    auto last = std::unique(roadsvector.begin(), roadsvector.end());
    roadsvector.erase(last, roadsvector.end());
    return roadsvector;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
    if (it != townmap_.end()){
        std::vector<TownID> returnvector;

        for (auto i : it->second.adjacentTowns){
            returnvector.push_back(i->id_);
        }
        std::sort(returnvector.begin(), returnvector.end());
        return returnvector;
    } else {
        return std::vector<TownID>{NO_ID};
    }
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    if ( town1 == town2){
        return false;
    } else {
        std::unordered_map<TownID, TownDataMap>:: iterator town1_iterator = townmap_.end();
        std::unordered_map<TownID, TownDataMap>:: iterator town2_iterator = townmap_.end();
        std::unordered_map<TownID, TownDataMap>:: iterator i = townmap_.begin();

        while ( i != townmap_.end()){
            if ( i->first == town1){
                town1_iterator = i;
            } if ( i->first == town2){
                town2_iterator = i;
            }
            i++;
            //will stop the loop if bot towns have been found
            if (town1_iterator != townmap_.end() && town2_iterator != townmap_.end()){
                i = townmap_.end();
            }
        }
        if (town1_iterator != townmap_.end() && town2_iterator != townmap_.end()){

            //checks if the road exists
            if (std::find(town1_iterator->second.adjacentTowns.begin(),
                          town1_iterator->second.adjacentTowns.end(),
                          &town2_iterator->second) != town1_iterator->second.adjacentTowns.end()){
                //road already exists
                return false;

            } else {
                //adjacent town is added to both towns
                town1_iterator->second.adjacentTowns.push_back(&town2_iterator->second);
                town2_iterator->second.adjacentTowns.push_back(&town1_iterator->second);
            }
        } else {
            // one of the towns or both have not been found
            return false;
        }
    }
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    if ( town1 == town2){
        return false;
    } else {
        std::unordered_map<TownID, TownDataMap>:: iterator town1_iterator = townmap_.end();
        std::unordered_map<TownID, TownDataMap>:: iterator town2_iterator = townmap_.end();
        std::unordered_map<TownID, TownDataMap>:: iterator i = townmap_.begin();

        while ( i != townmap_.end()){
            if ( i->first == town1){
                town1_iterator = i;
            } if ( i->first == town2){
                town2_iterator = i;
            }
            i++;
            //will stop the loop if bot towns have been found
            if (town1_iterator != townmap_.end() && town2_iterator != townmap_.end()){
                i = townmap_.end();
            }
        }
        //towns have been found
        if (town1_iterator != townmap_.end() && town2_iterator != townmap_.end()){
            //boolean to check if road has been found
            bool roadfound = false;
            for (auto it = town1_iterator->second.adjacentTowns.begin();
                 it != town1_iterator->second.adjacentTowns.end(); ){
                if (*it == &town2_iterator->second){
                    it = town1_iterator->second.adjacentTowns.erase(it);
                    roadfound = true;
                    break;
                }
                it++;
            }
            for (auto it = town2_iterator->second.adjacentTowns.begin();
                 it != town2_iterator->second.adjacentTowns.end(); ){
                if(*it == &town1_iterator->second){
                    it = town2_iterator->second.adjacentTowns.erase(it);
                    break;
                }
                it++;
            }
            if (roadfound){
                return true;
            } else {
                //road was not found
                return false;
            }
        } else {
            // one of the towns or both have not been found
            return false;
        }
    }
}

void Datastructures::clear_roads()
{

    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
    while ( it != townmap_.end()){
        it->second.adjacentTowns.clear();
        it++;
    }
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    std::unordered_map<TownID, TownDataMap>:: iterator from_iterator = townmap_.find(fromid);
    std::unordered_map<TownID, TownDataMap>:: iterator to_iterator = townmap_.find(toid);

    //checks if the towns have been found
    if ( from_iterator == townmap_.end() || to_iterator == townmap_.end()){
        return {NO_ID};
    } else{
        //Setting all the nodes as white
        std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
        while ( it != townmap_.end()){
            it->second.iswhite_ = true;
            it->second.isgrey_ = false;
            it->second.isblack_ = false;
            it++;
        }
       // std::vector<TownID> path = anyrouteBFS(fromid, toid);
        std::vector<TownID> path = {};
        DFSrec(from_iterator->second, path, toid);
        std::reverse(path.begin(), path.end());
        return path;
    }
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    return {}; // Replace with actual implementation
}

int Datastructures::total_net_tax(TownID id)
{
    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(id);
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

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    std::unordered_map<TownID, TownDataMap>:: iterator from_iterator = townmap_.find(fromid);
    std::unordered_map<TownID, TownDataMap>:: iterator to_iterator = townmap_.find(toid);

    //checks if the towns have been found
    if ( from_iterator == townmap_.end() || to_iterator == townmap_.end()){
        return {NO_ID};
    } else{
        //Setting all the nodes as white
        std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
        while ( it != townmap_.end()){
            it->second.iswhite_ = true;
            it->second.isgrey_ = false;
            it->second.isblack_ = false;
            it->second.last = nullptr;
            it->second.distance = INFINITY;
            it++;
        }
        std::queue<TownDataMap> queue;
        from_iterator->second.iswhite_ = false;
        from_iterator->second.isgrey_ = true;
        from_iterator->second.distance = 0;
        queue.push(from_iterator->second);
        TownDataMap current;
        while (!queue.empty()){
            current = queue.front();
            queue.pop();

            for (auto town : current.adjacentTowns){
                if(town->iswhite_){
                    town->iswhite_ = false;
                    town->isgrey_ = true;
                    town->distance = current.distance + 1;
                    town->last = &current;
                    queue.push(*town);
                }
            }
            current.isblack_ = true;
            current.iswhite_ = false;
            current.isgrey_ = false;
        }
        std::vector<TownID> path;
        get_least_towns_path(path, from_iterator->second, &to_iterator->second);
        return path;
    }
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    return {NO_ID}; // Replace with actual implementation
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    std::unordered_map<TownID, TownDataMap>:: iterator start_iterator = townmap_.find(startid);

    //checks if the town has been found
    if ( start_iterator == townmap_.end()){
        return {NO_ID};
    } else{
        //Setting all the nodes as white
        std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.begin();
        while ( it != townmap_.end()){
            it->second.iswhite_ = true;
            it->second.isgrey_ = false;
            it->second.isblack_ = false;
            it++;
        }

        std::vector<TownID> path = {};
        cyclerec(start_iterator->second, path, &start_iterator->second);
        std::reverse(path.begin(), path.end());
        return path;
    }
}

Dist Datastructures::trim_road_network()
{
    return NO_DIST; // Replace with actual implementation
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

//std::vector<TownID> Datastructures::anyrouteBFS(TownID starttown, TownID endtown)
//{
//    std::unordered_map<TownID, TownDataMap>:: iterator it = townmap_.find(starttown);
//    std::stack<TownDataMap> queue;
//    std::vector<TownID> path;
//    it->second.isgrey_ = true;
//    it->second.iswhite_ = false;
//    queue.push(it->second);
//    bool townfound = false;
//    TownDataMap currenttown;
//    while( !(queue.empty() || townfound)){
//        currenttown = queue.top();
//        queue.pop();
//        path.push_back(currenttown.id_);
//        if ( currenttown.id_ == endtown){
//            townfound = true;
//        } else {

//            //used so the path can "backtrack"
//            bool alladjacentvisited = true;

//            for ( auto town : currenttown.adjacentTowns){
//                if ( town->iswhite_){
//                    town->iswhite_ = false;
//                    town->isgrey_ = true;
//                    alladjacentvisited = false;
//                    queue.push(*town);
//                }
//            }
//            //if the algorithm reaches a deadend, the path will backtrack
//            if ( alladjacentvisited && !currenttown.adjacentTowns.empty()){
//                path.pop_back();
//            }
//        }


//    }
//    if(townfound){

//        return path;
//    } else {
//        //Path was not found
//        return {};
//    }
//}

void Datastructures::DFSrec(Datastructures::TownDataMap &town, std::vector<TownID> &path, TownID endtown)
{
    town.isgrey_=true;
    town.iswhite_=false;
    if (town.id_ == endtown){
        path.push_back(town.id_);
    }
    if ( path.empty() ){
        for (auto adjtown : town.adjacentTowns){
            if( adjtown->iswhite_ ){
                DFSrec(*adjtown, path, endtown);
            }

        }
        if ( !path.empty() ){
            path.push_back(town.id_);
        }
    }

}

void Datastructures::cyclerec(Datastructures::TownDataMap &town, std::vector<TownID> &path, Datastructures::TownDataMap* lasttown)
{
    town.isgrey_=true;
    town.iswhite_=false;

    if ( path.empty() ){
        for (auto adjtown : town.adjacentTowns){
            if( adjtown->iswhite_ ){
                cyclerec(*adjtown, path, &town);
            }else{
                if( adjtown->isgrey_ && std::find(path.begin(), path.end(), adjtown->id_) == path.end() && adjtown != lasttown){
                    path.push_back(adjtown->id_);
                }
            }

        }
        if ( !path.empty() ){
            path.push_back(town.id_);
        }
    }
    town.isgrey_ = false;
    town.isblack_ = true;
}

void Datastructures::get_least_towns_path(std::vector<TownID> &path, Datastructures::TownDataMap currenttown, Datastructures::TownDataMap* lasttown)
{
    if ( currenttown.id_ == lasttown->id_){
        path.push_back(currenttown.id_);
    } else {
        if (lasttown->last == nullptr){
            path = {};
        } else {
            get_least_towns_path(path, currenttown, lasttown->last);
            path.push_back(currenttown.id_);
        }

    }
}

