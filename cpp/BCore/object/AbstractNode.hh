#ifndef BEMO_ABSTRACTNODE_HH
#define BEMO_ABSTRACTNODE_HH

#include <BCore/Bemo.hh>
#include <BCore/Platform.hh>
#include <BCore/internal/Handle.hh>
#include <BCore/internal/Object.hh>
#include <BCore/object/Plug.hh>
#include <BCore/Variant.hh>

namespace bemo {

using NodeName = std::string;
using NodeType = std::string;

class AbstractNode {

    friend class NodeSystem;
    friend class NodeManager;

public:
    AbstractNode();
    virtual ~AbstractNode();

    inline bool isValid() const { return m_id != ObjectID(); }
    inline ObjectID getID() const { return m_id; }
    inline NodeType getType() const { return m_nodeType; }
    inline NodeName getName() const { return m_nodeName; }
    void setName( const NodeName& name );

    void connect( const PlugName& sourcePlugName,
                  const ObjectID& targetID,
                  const PlugName& targetPlugName );

    void disconnect( const PlugName& sourcePlugName,
                     const ObjectID& targetID,
                     const PlugName& targetPlugName );

//    std::vector< Plug* > getPlugs() const { return std::vector< Plug* >(); }
//    std::vector< Plug* > getInputPlugs() const { return std::vector< Plug* >(); }
//    std::vector< Plug* > getOutputPlugs() const { return std::vector< Plug* >(); }

    void setInput( const std::string& name, const Variant& var );
    void setOutput( const std::string& name, const Variant& var );

    Variant getInput( const PlugName& name );
    Variant getOutput( const PlugName& name );

    bool hasInput( const PlugName& name ) const;
    bool hasOutput( const PlugName& name ) const;

    virtual int execute();

private:
    ObjectID m_id;
    NodeName m_nodeName;
    NodeType m_nodeType;
    std::vector< ObjectID > m_plugs;

};



}

#endif // BEMO_ABSTRACTNODE_HH
