#ifndef BEMO_CONNECTIONMANAGER_HH
#define BEMO_CONNECTIONMANAGER_HH

#include <old/BCore/Bemo.hh>
#include <old/BCore/managers/ObjectManager.hh>
#include <old/BCore/event/Event.hh>
#include <old/BCore/event/EventHandler.hh>

#include <old/BCore/object/Plug.hh>
#include <old/BCore/object/Connection.hh>
#include <old/BCore/internal/Object.hh>

namespace bemo {

class ConnectionManager {

public:

    ObjectID create( const ObjectID& sourceID, const ObjectID& targetID ) {
        ObjectID id = BMO_ObjectManager->acquire( ObjectType::Plug );
        Connection* connection = new Connection( sourceID, targetID );
        connection->m_id = id;
        m_connections.push_back( connection );
        BMO_EventHandler->sendEvent< PlugConnectedEvent >( sourceID, targetID );
        return id;
    }

    void remove( const ObjectID& id ) {
        auto iter = std::find_if( m_connections.begin(),
                                  m_connections.end(),
                                  [&]( Connection* connection )->bool{
                                      return connection->getID() == id;
                                  });
        if ( iter != m_connections.end() ) {
            BMO_EventHandler->sendEvent< PlugDisconnectedEvent >(
                    ( *iter )->getSourceID(), ( *iter )->getTargetID() );
            BMO_ObjectManager->release( ( *iter )->getID() );
            ( *iter )->m_id = ObjectID::invalid();
            m_connections.erase( iter );
        }
    }

    /// Find all connections connected to the plugID
    std::vector< Connection* > find( const ObjectID& plugID ) {
        std::vector< Connection* > connections;
        for ( Connection* connection : m_connections ) {
            if ( ( connection->getSourceID() == plugID ) ||
                 ( connection->getTargetID() == plugID ) ) {
                connections.push_back( connection );
            }
        }
        return connections;
    }

    Connection* find( const ObjectID& sourceID,
                      const ObjectID& targetID ) const {
        auto iter = std::find_if( m_connections.begin(),
                                  m_connections.end(),
                                  [&]( Connection* connection ) {
                                      return ( sourceID == connection->getSourceID() ) &&
                                             ( targetID == connection->getTargetID() );
                                  });
        return iter != m_connections.end() ? *iter : nullptr;
    }

    Connection* find( const ObjectID& id ) const {
        auto iter = std::find_if( m_connections.begin(),
                                  m_connections.end(),
                                  [id]( Connection* connection ) {
                                      return connection->m_id == id;
                                  });
        return iter != m_connections.end() ? *iter : nullptr;
    }

private:
    std::vector< Connection* > m_connections;
};

}

#endif // BEMO_CONNECTIONMANAGER_HH
