#include <BCore/Bemo.hh>
#include <BCore/object/Plug.hh>
#include <BCore/object/Plugin.hh>
#include <BCore/managers/NodeManager.hh>
#include <BCore/managers/PluginManager.hh>

#include "PyAbstractNode.hh"

#include <pybind11/stl.h>
#include <pybind11/eval.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace bemo;

void py_initialize() {

    const std::string modulePath = "/Users/eddiehoyle/Code/cpp/bemo/py/plugins/core.py";

    py::object scope = py::module::import( "__main__" ).attr( "__dict__" );
    py::eval_file( modulePath, scope );
    py::dict globals = scope.cast< py::dict >();
    if ( globals.contains ( "bmo_registerPlugin" ) ) {
        py::object init = globals[ "bmo_registerPlugin" ];
        if ( py::hasattr( init, "__call__" ) ) {
            init( BMO_PluginManager->create() );
        }
    }
}

class AbstractTestNode {
public:
    virtual ~AbstractTestNode() {
        BMO_ERROR << ": A : dtor=" << (void*)this;
    }
};


class PyTestNode : public AbstractTestNode {
public:
    ~PyTestNode() override {
        BMO_ERROR << ": B : dtor=" << (void*)this;
    }
};

void py_genAPI( py::module& m ) {

    m.def("initialise", [](){
        initialize();
        py_initialize();
    });

    m.def("terminate", &terminate);

    py::class_<ObjectID>(m, "ObjectID")
            .def(py::init<>())
            .def_static("invalid", &ObjectID::invalid);

    py::class_<AbstractTestNode, PyTestNode>(m, "TestNode")
            .def(py::init<>());


    m.def("test_create", []()->AbstractTestNode*{
        return new AbstractTestNode();
    });

    m.def("create", []( const NodeType& type, const NodeName& name )->AbstractNode*{
        if ( BMO_NodeManager == nullptr ) {
            throw std::runtime_error( "Bemo not initialised!" );
        }
        AbstractNode* node = BMO_NodeManager->create( type, name );
//        py::handle( node )
        return node;
    }, py::arg("type"), py::arg("name") = "",
            py::return_value_policy::move );

    m.def("remove", []( AbstractNode* node ){
        BMO_NodeManager->remove( node->getID() );
    });

    m.def("ls", []( const std::vector< NodeName >& names ){
        std::vector< AbstractNode* > nodes;
        for ( auto name : names ) {
            AbstractNode* node = BMO_NodeManager->find( name );
            if ( node && ( std::find( nodes.begin(), nodes.end(), node ) == nodes.end() ) ) {
                nodes.push_back( node );
            }
        }
        return nodes;
    });
}