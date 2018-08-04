#ifndef BEMO_API_HH
#define BEMO_API_HH

#include <string>

namespace bemo {

class NodeRegistry;
class NodeManager;
class PlugManager;

extern NodeRegistry* BMO_NodeRegistry;
extern NodeManager* BMO_NodeManager;
extern PlugManager* BMO_PlugManager;

extern void send( const std::string& message );

extern void initialize();
extern void terminate();

}

#endif // BEMO_API_HH
