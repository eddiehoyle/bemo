#ifndef BEMO_NODEMANAGER_HH
#define BEMO_NODEMANAGER_HH

#include <old/BCore/Bemo.hh>
#include <old/BCore/internal/Table.hh>
#include <old/BCore/object/AbstractNode.hh>
#include <tuple>

namespace bemo {

class AbstractCreateFunc {
public:
    virtual ~AbstractCreateFunc() = default;
    virtual AbstractNode* invoke() = 0;
};

class AbstractLayoutFunc {
public:
    virtual ~AbstractLayoutFunc() = default;
    virtual void invoke( ObjectID id ) = 0;
};

template< typename F >
class CreateFuncWrapper : public AbstractCreateFunc {
public:
    explicit CreateFuncWrapper( F func ) : m_func( func ) {}
    AbstractNode* invoke() override { return m_func(); }
public:
    F m_func;
};

template< typename F >
class LayoutFuncWrapper : public AbstractLayoutFunc {
public:
    explicit LayoutFuncWrapper( F func ) : m_func( func ) {}
    void invoke( ObjectID id ) override { m_func( id ); }
public:
    F m_func;
};


class NodeManager {
public:
    ~NodeManager();

    AbstractNode* create( const NodeType& type, const NodeName& name = NodeName() );

    void remove( const ObjectID& nodeID );

    AbstractNode* find( const ObjectID& name ) const;
    AbstractNode* find( const NodeName& name ) const;

    std::vector< AbstractNode* > getNodes() const {
        return m_nodes;
    }

    bool exists( const ObjectID& id ) const;

    std::size_t count() const { return m_nodes.size(); };

    void addBlueprint( const std::string& type,
                       AbstractCreateFunc* wrCreate,
                       AbstractLayoutFunc* wrLayout );

    AbstractCreateFunc* findCreate( const std::string& type ) const;

    AbstractLayoutFunc* findLayout( const std::string& type ) const;

private:
    std::map< std::string, std::pair< AbstractCreateFunc*, AbstractLayoutFunc* > > m_funcs;
    std::vector< AbstractNode* > m_nodes;
};

}

#endif // BEMO_NODEMANAGER_HH
