#include "scriptednodetype.h"

/*!
 * \brief NODEFLOW::ScriptedNodeType::create
 * \return true on success
 */
bool NODEFLOW::ScriptedNodeType::create()
{
    try
    {
        if(_state)
        {
            sel::State *p = _state.release();
            if(p) delete p;
        }
        _state = std::unique_ptr<sel::State>(new sel::State(true));

        return true;
    }
    catch(...)
    {

    }
    return false;
}

/*!
 * \brief NODEFLOW::ScriptedNodeType::load
 * \param file
 * \return true on success
 */
bool NODEFLOW::ScriptedNodeType::load(const std::string &file)
{
    if(create())
    {
        std::stringstream ss;
        ss << "package.path =\"" << scriptDirectory() << "/?.lua\""; // set the package path
        (*_state)(ss.str().c_str());
        if(_state.get()->Load(file))
        {
            return setupApi();
        }
    }
    return false;
}

/*!
 * \brief NODEFLOW::ScriptedNodeType::setupApi
 * \return true on success
 */
bool NODEFLOW::ScriptedNodeType::setupApi() // set up the API links so the LUA script can use C++ functions and C++ can call LUA functions
{
    state()["Util"].SetObj( *this,"Trace",&NODEFLOW::InterfaceScript::trace);

    return true;
}

/*!
 * \brief NODEFLOW::ScriptedNodeType::start
 * \param ns
 * \param n
 */
void NODEFLOW::ScriptedNodeType::start(NodeSet &ns,  NodePtr &n)
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::trigger
 * \param ns
 * \param n
 */
void NODEFLOW::ScriptedNodeType::trigger(NodeSet &ns, NodePtr &n )
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::setupConnections
 */
void NODEFLOW::ScriptedNodeType::setupConnections()
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::properties
 * \param parent
 * \param ns
 * \param nodeId
 * \return
 */
bool NODEFLOW::ScriptedNodeType::properties(wxWindow * parent,NodeSet &ns, unsigned nodeId)
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::process
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool NODEFLOW::ScriptedNodeType::process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::evaluate
 * \param ns
 * \param nodeId
 * \param id
 * \param data
 * \return
 */
bool NODEFLOW::ScriptedNodeType::evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data )
{

}
/*!
 * \brief NODEFLOW::ScriptedNodeType::setup
 */
void NODEFLOW::ScriptedNodeType::setup()
{

}



