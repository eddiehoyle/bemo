#ifndef BEMO_PLUGINMANAGER_HH
#define BEMO_PLUGINMANAGER_HH

#include <BCore/object/Plugin.hh>
#include "AbstractManager.hh"

namespace bemo {

class PluginManager : public AbstractManager< Plugin > {

    using PluginName = std::string;
    using PluginID = ObjectID;
    using PluginMap = std::map< PluginName, PluginID >;

public:
    void registerNode( const std::string& name,
                       int nodeTypeID,
                       std::function<void>&& creator,
                       std::function<void>&& header,
                       std::function<void>&& layout ) {
        Plugin plugin = create();
    }
    void unregisterNode( const std::string& name ) {
        m_pluginMap.erase( name );
    }
private:
    PluginMap m_pluginMap;

};

}

#endif // BEMO_PLUGINMANAGER_HH
