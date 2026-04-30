#include "server/Server.hpp"
#include "storage/Storage.hpp"
#include "storage/AOF.hpp"
#include <memory>
#include<iostream>

using namespace std;

int main() {
    auto db = make_shared<Storage>(100); 
    
    auto aof = make_shared<AOF>("appendonly.aof");
    db->setAOF(aof);

    cout << "Checking for existing data in AOF..." << endl;
    aof->load(*db);
    
    Server server(6379, *db);
    server.start();
    
    return 0;
}