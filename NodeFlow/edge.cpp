#include "edge.h"
#include "nodeset.h"
void NODEFLOW::Edge::load(NodeSet &s)
{
    MRL::VariantPropertyTree &t = s.data();
    MRL::PropertyPath p;
    toPath(p);
    //
    _from.setNode(t.getValue<unsigned>(p,"FromeNode"));
    _from.setId(t.getValue<unsigned>(p,"FromeId"));
    _to.setNode(t.getValue<unsigned>(p,"ToNode"));
    _to.setId(t.getValue<unsigned>(p,"ToId"));
    //
}

void NODEFLOW::Edge::save(NodeSet &s)
{
    MRL::VariantPropertyTree &t = s.data();
    MRL::PropertyPath p;
    toPath(p);
    t.setValue(p,"Id",id());
    t.setValue(p,"FromNode",_from.node());
    t.setValue(p,"FromId",_from.id());
    t.setValue(p,"ToNode",_to.node());
    t.setValue(p,"ToId",_to.id());
}
