
#include <pybind11/stl.h>
#include <pybind11/eval.h>
#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <BCore/Assert.hh>
#include <BCore/object/Graph.hh>
#include <BCore/object/Node.hh>
#include <BCore/object/ObjectManager.hh>
#include <BCore/plugin/PluginSystem.hh>

namespace py = pybind11;
using namespace bemo;

class BPyDagNode;
typedef std::shared_ptr< BPyDagNode > BPyDagNodePtr;

class BPyDagGraph : public BDagGraph {
public:
    explicit BPyDagGraph( ObjectID id ) : BDagGraph( id ) { BMO_ERROR << "ctor=" << (void*)this; }
//    BPyDagGraph() : BDagGraph( ObjectID::invalid() ) { BMO_ERROR << "ctor=" << (void*)this; }
    virtual ~BPyDagGraph() { BMO_ERROR << "dtor=" << (void*)this; }
    void initialise() override {
        PYBIND11_OVERLOAD_PURE(
                void,
                BPyDagGraph,
                initialise(),
        );
    }
};

/// An object that wraps a DAG object
class BPyDagObject {
public:
    explicit BPyDagObject( ObjectID id ) : m_objectID( id ){}
    ObjectID getObjectID() const { return m_objectID; }
private:
    ObjectID m_objectID;
};

template< typename T >
void cpp_deleteObject( T object ) {
    ObjectManager::instance().release( object->getObjectID() );
}

template< typename T >
std::shared_ptr< T > cpp_createDag( const std::string& type, const std::string& name ) {
    return std::shared_ptr< T >( new T( ObjectManager::instance().acquire() ) );
}

void cpp_init() {

    const std::string modulePath = "/Users/eddiehoyle/Code/cpp/bemo/py/plugins/core.py";

    py::object scope = py::module::import( "__main__" ).attr( "__dict__" );
    py::eval_file( modulePath, scope );
    py::dict globals = scope.cast< py::dict >();
    if ( globals.contains( "bmo_registerPlugin" ) ) {
        py::object init = globals[ "bmo_registerPlugin" ];
        if ( py::hasattr( init, "__call__" ) ) {
            init( PluginManager::instance().create() );
        }
    }
}










typedef std::function<BPyDagNodePtr()> BPyFnCreateFunctor;

namespace bemo {

//template <>
//BPyDagNode* PluginSystem::create( const std::string& name ) {
//    AbstractCreateFuncWrapper* absFunc = PluginManager::instance().get(name);
//    BMO_ASSERT( absFunc )
//
//    if ( absFunc ) {
////        BPyFnCreateFunctor vptr = *static_cast<BPyFnCreateFunctor*>(absFunc->get());
////        return vptr();
//        std::function< Object*() > vptr = *static_cast<std::function< Object*() >*>(absFunc->get());
////        return BPyDagNodePtr( static_cast< BPyDagNode* >( vptr() ) );
////        return static_cast< BPyDagNode* >( vptr() );
//        return nullptr;
//
//    }
//    return nullptr;
//}

//template <>
//BPyDagNodePtr PluginSystem::create( const std::string& name ) {
//    BMO_ERROR << "highjacking: BPyDagNodePtr";
//    auto f = PluginManager::instance().get(name);
//    BMO_ASSERT( f != nullptr );
//
//    // Python
//    BPyFnCreateFunctor fptr = *(BPyFnCreateFunctor*)(f->get());
//    BMO_ASSERT( fptr != nullptr );
//    BPyDagNodePtr r = fptr();
//
//    // C++
////    auto fptr = *(std::function<Object*()>*)(f->get());
////    BMO_ASSERT( fptr != nullptr );
////    BPyDagNodePtr r = BPyDagNodePtr( dynamic_cast< BPyDagNode* >( fptr() ) );
//
//    return nullptr;
//}

//template <>
//Object* PluginSystem::create( const std::string& name ) {
//    BMO_ERROR << "highjacking: Object*";
////    auto f = PluginManager::instance().get(name);
////    BMO_ASSERT( f != nullptr );
////    auto fptr = *(std::function<BPyDagNodePtr()>*)(f->get());
////    BMO_ASSERT( fptr != nullptr );
////    BPyDagNodePtr r = fptr();
//    return nullptr;
//}

}



PYBIND11_MODULE(bmoapi, m) {

    py::class_< PluginSystem, std::shared_ptr< PluginSystem > >( m, "PluginSystem" )
            .def( py::init< ObjectID >() )
            .def( "setHeader", &PluginSystem::setHeader )
            .def( "registerNode", &PluginSystem::registerNode< BDagNode* > )
            .def( "create", []( PluginSystem* self, const std::string& name ) {
                return std::shared_ptr< BDagNode >( self->create< BDagNode* >( name ) );
            } );
//            .def( "create", &PluginSystem::create< BDagNode* > );

    m.def("execute", []( BDagNode* nodePtr )->int {
        return nodePtr->execute();
    });

    m.def("init", &cpp_init );

    m.def( "create_node",
           &cpp_createDag< BPyDagNode >,
           py::arg("type"), py::arg("name") = "node1" );

    m.def( "create_graph",
           &cpp_createDag< BPyDagGraph >,
           py::arg("type"), py::arg("name") = "graph1" );

    m.def( "delete", &cpp_deleteObject< BPyDagNode* > );
    m.def( "delete", &cpp_deleteObject< BPyDagGraph* > );

    py::class_<ObjectID>(m, "ObjectID")
            .def(py::init<>())
            .def_static("invalid", &ObjectID::invalid);


//    py::class_< BPyDagGraph, std::shared_ptr< BPyDagGraph > >( m, "BPyDagGraph" )
    py::class_< BPyDagGraph >( m, "BPyDagGraph" )
            .def(py::init< ObjectID >())
            .def("isValid", &BPyDagNode::isValid );

//    py::class_< BPyDagObject, std::shared_ptr< BPyDagObject > >( m, "BPyDagObject" )
//            .def(py::init<>());
//            .def( "getObjectID", &BPyDagObject::getObjectID )
//            .def( "getObjectType", &BPyDagObject::getObjectType )
//            .def( "setName", &BPyDagObject::setName )
//            .def( "getName", &BPyDagObject::getName );
//            .def( "connect", &py_connect< T > )
//            .def( "disconnect", &py_disconnect< T > )
//            .def( "set_input", &py_setInput< T > )
//            .def( "set_output", &py_setOutput< T > )
//            .def( "get_input", &py_getInput< T > )
//            .def( "get_output", &py_getOutput< T > )
//            .def( "has_input", &py_hasInput< T > )
//            .def( "has_output", &py_hasOutput< T > )
//            .def( "get_inputs", &py_getInputs< T > )
//            .def( "get_outputs", &py_getOutputs< T > )

}

