// jsonCppUsage.cpp
#include <iostream>
#include "json/json.h"
 
#include <fstream> 
#include <map>
#include <vector>
#include<string>
//using namespace std;
 using std::string;
 using std::endl;
 using std::ifstream;
 using std::ofstream;
 using std::string;
 using std::stringstream;
using std::cout;
using std::cerr;

using std::map;
using std::vector;

    map<string, string> map_property;
    vector<string> vec_command;
    vector<vector <string> > vec_binary;
    map<string, string >map_dependency;
# define checksum_number 3

 bool read_json_file()
 {

    
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


    }
    read_file.close();

        string target_url = "/home/code" + vec_binary[0][0]; 
    //install depdence

        ofstream write_bash("command.sh");
        write_bash<<"#!/bin/bash"<<endl;
        write_bash<<"set -x"<<endl;
        write_bash<<"cd /home && mkdir code && cd code"<<endl;
        write_bash<<"export SSL_CERT_FILE=/etc/ssl/certs/ca-certificates.crt"<<endl;
        map<string, string>::iterator it = map_dependency.begin();

        system("apt-get update");
        while(it!= map_dependency.end())
        {

          if(it->first == "rust")
          {
             string order = "curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y  --no-modify-path";
             write_bash<<order<<endl;
             order = "source \"$HOME/.cargo/env\"";
             write_bash<<order<<endl;
             order = "rustup target add x86_64-unknown-linux-musl";
             write_bash<<order<<endl;
             order = "rustup install "+ it->second;
             write_bash<<order<<endl;
             order = "rustup default "+ it->second;
             write_bash<<order<<endl;
          }else if (it->first == "_goetc")
          {
                //TODO expand other can't install by apt-get
          }else 
          {
            cout<<it->first<<"   ====   version is ==== "<<it->second<<endl;
            string order = "apt-get install -y " + it->first+"="+it->second;
            system(order.c_str());
            
          }

           it++;
            
        }
    

    //excude command

        for(vector<string>::iterator it = vec_command.begin();it!=vec_command.end();it++)
        {
            write_bash<<*it<<endl;
        }
        write_bash<<"md5sum "<<target_url<<" >> /home/code/checksum"<<endl;
        write_bash<<"shasum "<<target_url<<" >> /home/code/checksum"<<endl;
        write_bash<<"sha256sum "<<target_url<<" >> /home/code/checksum"<<endl;
        
        
        write_bash.close();
        system("bash command.sh");
        
        ifstream read_checksum("/home/code/checksum");

        if(!read_checksum.is_open())
        {
            cerr<<"failure to open check faile file!"<<endl;
            return false;
        }

        string line, address;
        vector<string> checksum_result(checksum_number);
        size_t i = 0;
        while (getline(read_checksum, line))
        {
            /* code */
            stringstream ss;
            ss<<line;
            ss>>checksum_result[i++]>>address;
            ss.clear();
        }
        map_property["md5sum"] = checksum_result[0];
        map_property["shasum"]=checksum_result[1];
        map_property["sha256sum"] = checksum_result[2];
    
        

        cout<<"shasum is  "<<map_property["shasum"]<<endl;
        
        if(map_property["md5sum"] == vec_binary[0][1])
        {
            cout<<"Congratulations, the checksums are identical. This is a duplicate build."<<endl;
        }else 
        {
            cout<<"Sorry for the checksum inconsistency , this is not a repeatable build"<<endl;
        }

        system("rm -rf  /home/code");
 
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


