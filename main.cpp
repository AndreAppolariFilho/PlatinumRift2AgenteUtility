#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h> 

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
 class Node{
    public:
        int platinum;
        int ownerId;
        vector<int> numPods;
        int visible;
        int numberTimes;
 };

class Solution{
    private:
        map<int, vector<int>> graph;
        vector<Node> nodes;
        int myId;
        
    public:
        Solution(){}
        Solution(int zoneCount, int playerCount, int myId);
        std::vector<int> get_neighbors(int zone);
        Node get_zone_info(int zone);
        void make_connection(int zone1, int zone2 );
        void update_node(int zone, int platinum, int visible, vector<int>pods, int ownerId);
        string make_move(int zoneId, int minPodsPerUnit, bool findEnemy, int enemyId);
    
};
Solution::Solution(int zoneCount, int playerCount, int myId)
{
    this->myId = myId;
    
    for(int i = 0; i < zoneCount; i++)
    {
        Node node = Node();
        node.numPods = vector<int>(2);
        node.numberTimes = 0;
        nodes.push_back(node);
        graph.insert(make_pair(i, vector<int>()));
    }
    
}
std::vector<int> Solution::get_neighbors(int zone)
{
    return this->graph[zone];
}
Node Solution::get_zone_info(int zone)
{
    return this->nodes[zone];
}
void Solution::make_connection(int zone1, int zone2 )
{
    graph[zone1].push_back(zone2);
    graph[zone2].push_back(zone1);
}
void Solution::update_node(int zone, int platinum, int visible, vector<int>pods, int ownerId)
{
    
    this->nodes[zone].ownerId = ownerId;
    this->nodes[zone].platinum = platinum;
    this->nodes[zone].visible = visible;
    for(int i = 0; i < pods.size(); i++)
        this->nodes[zone].numPods[i] = pods[i];
    
    
}

string Solution::make_move(int zoneId, int maxPodsPerUnit, bool findEnemy, int enemyId)
{
     srand(time(NULL));
    
    Node n = get_zone_info(zoneId);
    Node n_neighbor;
    string ret = "";
    if(n.numPods[this->myId] > maxPodsPerUnit)
    {
        //get neighbors
        vector<int> neighbors = this->get_neighbors(zoneId);
        vector<int> possibles_locations;
        for(int i = 0; i < neighbors.size(); i++)
        {
            n_neighbor = get_zone_info(neighbors[i]);   
            if(n_neighbor.numPods[this->myId] == 0)
            {
                possibles_locations.push_back(neighbors[i]);
            }
            
        }
        if(possibles_locations.size()>0)
        {
            int iSecret = rand() % possibles_locations.size();
            ret+=to_string(n.numPods[this->myId]-maxPodsPerUnit)+" "+to_string(zoneId)+" "+to_string(possibles_locations[iSecret]);
            this->nodes[possibles_locations[iSecret]].numberTimes += 1;
            
        }
        else
        {
            Node node_min = this->nodes[neighbors[0]];
            int id = 0;
            for(int i = 0; i < neighbors.size(); i++)
            {
                //TODO: modificar aqui
                if(findEnemy)
                {
                    if(node_min.numberTimes + ((id-enemyId))   > this->nodes[neighbors[i]].numberTimes + ((i-enemyId))) 
                    {
                        node_min = this->nodes[neighbors[i]];
                        id = i;
                    }    
                }
                else
                {
                    if(node_min.numberTimes   > this->nodes[neighbors[i]].numberTimes )
                    {
                        node_min = this->nodes[neighbors[i]];
                        id = i;
                    }
                }
            }
            for(int i = 0; i < neighbors.size(); i++)
            {
                if(findEnemy)
                {
                    if(node_min.numberTimes + ((id-enemyId)) == this->nodes[neighbors[i]].numberTimes + ((i-enemyId)))
                    {
                        possibles_locations.push_back(neighbors[i]);
                    }
                }
                else
                {
                    if(node_min.numberTimes == this->nodes[neighbors[i]].numberTimes )
                    {
                        possibles_locations.push_back(neighbors[i]);
                    }
                }
            }
            int iSecret = rand() % possibles_locations.size();
            ret+=to_string(n.numPods[this->myId]-maxPodsPerUnit)+" "+to_string(zoneId)+" "+to_string(possibles_locations[iSecret]);
            this->nodes[possibles_locations[iSecret]].numberTimes += 1;
        }
       
        //Search for empty nodes
        return ret;
    }
    return "";
}
int main()
{
    int playerCount; // the amount of players (always 2)
    int myId; // my player ID (0 or 1)
    int zoneCount; // the amount of zones on the map
    int linkCount; // the amount of links between all zones
    
    cin >> playerCount >> myId >> zoneCount >> linkCount; cin.ignore();
    Solution s = Solution(zoneCount, playerCount, myId);
    int idZoneEnemy = 0;
    bool findEnemy = false;
    for (int i = 0; i < zoneCount; i++) {
        int zoneId; // this zone's ID (between 0 and zoneCount-1)
        int platinumSource; // Because of the fog, will always be 0
        cin >> zoneId >> platinumSource; cin.ignore();
    }
    for (int i = 0; i < linkCount; i++) {
        int zone1;
        int zone2;
        cin >> zone1 >> zone2; cin.ignore();
        s.make_connection(zone1, zone2);
    }
    
    // game loop
    while (1) {
        int myPlatinum; // your available Platinum
        cin >> myPlatinum; cin.ignore();
        string result = "";
        for (int i = 0; i < zoneCount; i++) {
            int zId; // this zone's ID
            int ownerId; // the player who owns this zone (-1 otherwise)
            int podsP0; // player 0's PODs on this zone
            int podsP1; // player 1's PODs on this zone
            int visible; // 1 if one of your units can see this tile, else 0
            int platinum; // the amount of Platinum this zone can provide (0 if hidden by fog)
            cin >> zId >> ownerId >> podsP0 >> podsP1 >> visible >> platinum; cin.ignore();
            //cerr<<zId<<" "<<platinum<<endl;
            vector<int>pods;
            pods.push_back(podsP0);
            pods.push_back(podsP1);
            if(!findEnemy && ownerId!=myId && pods[ownerId] > 0)
            {
                idZoneEnemy = zId;
                findEnemy = true;
            }
            s.update_node(zId, platinum, visible, pods, ownerId);
            result+=s.make_move(zId,9,findEnemy, idZoneEnemy)+" ";
            
        }
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // first line for movement commands, second line no longer used (see the protocol in the statement for details)
        if(result!="")
            cout << result << endl;
        else
            cout << "WAIT" << endl;
        cout << "WAIT" << endl;
    }
}
