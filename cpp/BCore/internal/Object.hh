#ifndef BEMO_OBJECT_HH
#define BEMO_OBJECT_HH

#include <ostream>

#include "Handle.hh"
#include <BCore/Platform.hh>

namespace bemo {

using ObjectHandle = Handle< u64, 32, 32 >;

class ObjectID {

    friend class ObjectManager;

public:
    static ObjectID invalid();

public:
    ObjectID() : m_handle( ObjectHandle::INVALID_HANDLE ),
                 m_type( ObjectType::Invalid ) {}
    ObjectID( const ObjectID& obj ) : m_handle( obj.m_handle ), m_type( obj.m_type ) {}
    ObjectID& operator=( const ObjectID& obj ) {
        BMO_ERROR << "------------------------------------------------";
        this->m_handle = obj.m_handle;
        this->m_type = obj.m_type;
        return *this;
    }
    ObjectType type() const { return m_type; }
    bool operator<( const ObjectID& rhs ) const { return this->m_handle < rhs.m_handle; }
    bool operator>( const ObjectID& rhs ) const { return this->m_handle > rhs.m_handle; }
    bool operator==( const ObjectID& rhs ) const { return this->m_handle == rhs.m_handle; }
    bool operator!=( const ObjectID& rhs ) const { return this->m_handle != rhs.m_handle; }

    friend std::ostream& operator<<( std::ostream& stream, const ObjectID& object );

private:
    ObjectHandle m_handle;
    ObjectType m_type;
};

}

#endif // BEMO_OBJECT_HH
