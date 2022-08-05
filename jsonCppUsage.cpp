// jsonCppUsage.cpp
#include <iostream>
#include "json/json.h"
 
#include <fstream> 
#include <map>
#include <vector>

//using namespace std;
 using std::string;
 using std::endl;
 using std::ifstream;
 using std::ofstream;
 using std::string;

using std::cout;
using std::cerr;

using std::map;
using std::vector;
 bool read_json_file()
 {

    map<string, string> map_property;
    vector<string> vec_command;
    vector<vector <string> > vec_binary;
    map<string, string >map_dependency;
    ifstream read_file("new_json");
    
    if(!read_file.is_open())
    {
        cerr<<"failure to open json file!"<<endl;
        return false;
    }

    Json::Reader reader;
    Json::Value frame;

    if(reader.parse(read_file, frame))
    {
        map_property["version"] = frame["version"].asString();
        map_property["timestamp"] = frame["timestamp"].asString();
        map_property["name"] = frame["name"].asString();
       // map_property["binary_url"] = frame["binary"]["url"].asString();
        map_property["version_gf"] = frame["config"]["version_gf"].asString();
        map_property["architecture"] = frame["environment"]["architecture"].asString();
        map_property["kernel_version"] = frame["environment"]["kernel_version"].asString();
        map_property["sourceLink"] = frame["source"]["sourceLink"].asString();
        map_property["sourceType"] = frame["source"]["sourceType"].asString();

        for(unsigned int i = 0; i < frame["command"].size();i++)
        {
            vec_command.push_back(frame["command"][i].asString());
        }
        for(unsigned int i = 0; i < frame["dependency"].size();i++)
        {
            string source_dep = frame["dependency"][i]["source_dep"].asString();
            string version_dep = frame["dependency"][i]["version_dep"].asString();
            map_dependency[source_dep] = version_dep;
        }

        for(unsigned int i = 0; i < frame["binary"].size();i++)
        {
            cout<<"-----"<<endl;
            vector<string> bin(4);
            bin[0]=frame["binary"][i]["url"].asString();
            bin[1]=frame["binary"][i]["checksum"]["md5"].asString();
            bin[2]=frame["binary"][i]["checksum"]["sha1"].asString();
            bin[3]=frame["binary"][i]["checksum"]["sha256"].asString();
            vec_binary.push_back(bin);
            bin.clear();

        }


        //install depdence
        map<string, string>::iterator it = map_dependency.begin();

        system("apt-get update");
        while(it!= map_dependency.end())
        {
            cout<<it->first<<"   ====   version is ==== "<<it->second<<endl;
            string order = "apt-get install -y " + it->first+"="+it->second;
            system(order.c_str());
            it++;
        }

        //excude command

        for(vector<string>::iterator it = vec_command.begin();it!=vec_command.end();it++)
        {
            cout<<"========excute  "<<*it <<endl;
            system((*it).c_str());
        }
        



    }





    return true;
 }

 bool write_json_file()
 {
    
    //Json::Value data;
    Json::Value frame;
    Json:: StyledWriter styledWriter;
 
    // frame["type"] = 10086;
    // data["username"] = "test";
    // frame["data"] = data;
    Json::Value config, name, version_gf, source, environment, dependency, command,  binary, timestamp;
    Json::Value version;
    Json::Value sourceType, sourceLink;
    Json::Value architecture, kernel_version;
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
    environment["kernel_version"] = "5.0";
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
    checksum["md5"] = " ";
    checksum["sha1"]= " ";
    checksum["sha256"]= " ";
    bin["checksum"] = checksum;
    binary.append(bin);
    frame["binary"] = binary;
    frame["timestamp"] = "1234567890";







    string result = styledWriter.write(frame);
    //write result  to a json file 
    ofstream write_file("new_json");
    write_file<<result.c_str()<<endl;
   // printf("%s \n", result.c_str());
    write_file.close();
    return true;
 }




int main()
{
    // 解析json串
    read_json_file();
 
    // 生成json串
   // write_json_file();
 
    return 0;
}


