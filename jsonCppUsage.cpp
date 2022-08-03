// jsonCppUsage.cpp
#include <iostream>
#include "json/json.h"
 
#include <fstream> 
//using namespace std;
 using std::string;
 using std::endl;
 using std::ifstream;
 using std::ofstream;
 


 bool read_json_file()
 {
    string message = "{ \"data\" : { \"username\" : \"test\" }, \"type\" : 6 }";
    Json::Reader reader(Json::Features::strictMode());
    Json::Value parseData;
    if (reader.parse(message.c_str(), parseData))
    {
        int type = parseData.get("type", 0).asInt();
        Json::Value frameData = parseData.get("data", 0);
        string username = frameData.get("username", 0).asString();
        printf("type: %d, username: %s\n", type, username.c_str());
    }
    return true;
 }

 bool write_json_file()
 {
    
    Json::Value data;
    Json::Value frame;
    //Json::FastWriter fastWriter;
    Json:: StyledWriter styledWriter;
 
    // frame["type"] = 10086;
    // data["username"] = "test";
    // frame["data"] = data;
    Json::Value config, name, version_gf, source, environment, dependency, command,  binary, timestamp;
    Json::Value version;
    Json::Value sourceType, sourceLink;
    Json::Value architecture, kernelVersion;
    Json::Value source_dep, version_dep;
    Json::Value url, checksum, md5, sha1, sha256;
    Json::Value bin,dep;

    config["version_gf"] = "0.1";
    frame["config"] = config;
    frame["name"] = "kata-agent";
    frame["version"] = "1.0";
    source["sourceType"] = "git";
    source["sourceLink"] = "https://github.com/kata-containers/kata-containers.git"; 
    frame["source"] = source;
    environment["architecture"] = "x86-64";
    environment["kernelVersion"] = "5.0";
    frame["environment"] = environment;
    dep["source_dep"] = "make";
    dep["version_dep"]="2.0";
    dependency.append(dep);
    frame["dependency"] = dependency;
    command.append("mkdir kata-agent");
    command.append("cd kata-agent");
    command.append("apt-get source kata-agent");
    command.append("apt-get -y build-dep kata-agent"); 
    command.append("dpkg-source -x *.dsc build");  
    command.append("cd ../");
    frame["command"] = command;
    bin["url"] = "./src/agent/target/x86_64-unknown-linux-musl/release/kata-agent";
    checksum["md5"] = "";
    checksum["sha1"]= "";
    checksum["sha256"]= "";
    bin["checksum"] = checksum;
    binary.append(bin);
    frame["binary"] = binary;
    frame["timestamp"] = "1234567890";







    string result = styledWriter.write(frame);
    //write result  to a json file 
    ofstream write_file("new_json");
    write_file<<result.c_str()<<endl;
    printf("%s \n", result.c_str());

    return true;
 }




int main()
{
    // 解析json串
    read_json_file();
 
    // 生成json串
    write_json_file();
 
    return 0;
}


