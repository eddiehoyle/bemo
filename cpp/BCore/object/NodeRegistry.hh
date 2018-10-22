#ifndef BEMO_BLUEPRINTMANAGER_HH
#define BEMO_BLUEPRINTMANAGER_HH

#include <BCore/object/Object.hh>
#include <BCore/object/ObjectID.hh>
#include <map>


#include "../../../ext/pybind11/include/pybind11/stl.h"
#include "../../../ext/pybind11/include/pybind11/eval.h"
#include "../../../ext/pybind11/include/pybind11/pybind11.h"
#include "../../../ext/pybind11/include/pybind11/functional.h"

namespace bemo {

typedef std::function< pybind11::object() > FnCreate;
typedef std::function< void( ObjectID ) > FnLayout;

struct Blueprint {
    explicit Blueprint(FnCreate create, FnLayout layout) : create(create), layout(layout) {}
    FnCreate create;
    FnLayout layout;
};

class NodeRegistry {
public:
    static NodeRegistry& instance();

public:
    NodeRegistry( NodeRegistry const& ) = delete;
    void operator=( NodeRegistry const& ) = delete;

    void add( const std::string& type, FnCreate fnCreate, FnLayout fnLayout ) {
        m_blueprints[type] = new Blueprint( fnCreate, fnLayout );
    }

    Blueprint* find( const std::string& type ) const {
        auto iter = m_blueprints.find( type );
        if ( iter != m_blueprints.end() ) {
            return iter->second;
        }
        return nullptr;
    }


private:
    NodeRegistry() {}
    std::map< std::string, Blueprint* > m_blueprints;
};

}


#endif // BEMO_BLUEPRINTMANAGER_HH
