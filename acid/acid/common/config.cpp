//
// Created by zavier on 2021/10/26.
//

#include <algorithm>
#include <cctype>
#include "config.h"

namespace acid {
static auto g_logger = GetLogInstance();

static void ListAllMenbers(const std::string& perfix,const YAML::Node& node,
                           std::list<std::pair<std::string,YAML::Node>>& output){
    if(perfix.find_first_not_of("abcdefghijklmnopqrstuvwxyz0123456789._") != std::string::npos){
        SPDLOG_LOGGER_ERROR(g_logger, "config invalid name : {} : {}", perfix, node.Scalar());
        return;
    }
    output.emplace_back(perfix,node);
    if(node.IsMap()){
        for(auto& item : node){
            std::string name = perfix.empty()? item.first.Scalar() : perfix + "." + item.first.Scalar();
            ListAllMenbers(name,item.second,output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root) {
    std::list<std::pair<std::string,YAML::Node>> allNodes;
    ListAllMenbers("",root,allNodes);
    for(auto& item:allNodes){
        std::string key = item.first;
        if(key.empty())
            continue;
        std::transform(key.begin(),key.end(),key.begin(),::tolower);
        auto var = LookupBase(key);
        if(var){
            if(item.second.IsScalar()){
                var->fromString(item.second.Scalar());
            } else{
                std::stringstream ss;
                ss << item.second;
                var->fromString(ss.str());
            }
        }
    }
}

void Config::LoadFromFile(const std::string &file) {
    SPDLOG_LOGGER_INFO(g_logger, "load config from file: {}", file);
    YAML::Node root = YAML::LoadFile(file);
    LoadFromYaml(root);
}

ConfigVarBase::ptr Config::LookupBase(const std::string &name) {
    std::unique_lock<co_rmutex> lock(GetMutex().Reader());
    auto it = GetDatas().find(name);
    if(it == GetDatas().end())
        return nullptr;
    return it->second;
}

void Config::Visit(std::function<void(const ConfigVarBase::ptr)> cb) {
    auto& config = GetDatas();
    for(auto& item: config){
        cb(item.second);
    }
}

}