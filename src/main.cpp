#include "server/Server.hpp"
#include "storage/Storage.hpp"
#include "storage/AOF.hpp"
#include <memory>
#include<iostream>

using namespace std;

int main() {
    // 1. Create Storage
    auto db = make_shared<Storage>(100); 
    
    // 2. Create AOF and link it to Storage
    auto aof = make_shared<AOF>("appendonly.aof");
    db->setAOF(aof); // This connects the two!

    // 3. Load existing data
    cout << "Checking for existing data in AOF..." << endl;
    aof->load(*db);
    
    // 4. Start Server
    Server server(6379, *db);
    server.start();
    
    return 0;
}