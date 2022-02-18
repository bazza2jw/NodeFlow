#ifndef NODETYPE_H
#define NODETYPE_H

#include "node.h"
#include "propertiesloader.h"

namespace NODEFLOW
{
//
class NodeSet;
class NodeLayout;
//
typedef std::shared_ptr<Json::Value> VALUEPTR; // data packet mostly between nodes
//
typedef enum
{
    Single = 1,
    Multiple = 2,
    Calculated = 4 // right to left evaluation - left to right flow by default
} ConnectionMode;

/*!
 * \brief The Connection class
 */
class Connection
{
    std::string _name;
    ConnectionMode _mode = Single;
public:
    Connection() {}
    Connection(const std::string &n, ConnectionMode m = Single ) : _name(n),_mode(m) {}
    Connection(const Connection &) = default;
    virtual ~Connection() {}
    const std::string & name() const {
        return  _name;
    }
    ConnectionMode mode() const {
        return _mode;
    }
    void setMode(ConnectionMode m) {
        _mode = m;
    }
};

/*!
 * \brief The NodeLayout class
 * The layout of a node = cache the positions of everything
 */
class NodeLayout
{
    wxRect _rect;
    unsigned _inputCount = 1;
    unsigned _outputCount = 1;

    std::vector<wxRect> _inputs;
    std::vector<wxRect> _outputs;

    //
public:
    NodeLayout() {}
    NodeLayout(const NodeLayout &) = default;
    bool hit(wxPoint pt, const wxPoint location ) const
    {
        pt -= location; // translate to 0,0
        return _rect.Contains(pt);
    }
    /*!
     * \brief hitInputConnector
     * \param pt
     * \param location
     * \return
     */
    int hitInputConnector( wxPoint pt, wxPoint location) const
    {
        int ret = -1;
        pt -= location; // translate to 0,0
        if(_inputs.size() > 0)
        {
            for(size_t i = 0; i < _inputs.size(); i++)
            {
                if(_inputs[i].Contains(pt)) return i;
            }
        }
        return ret;
    }
    /*!
     * \brief hitOutputConnector
     * \param pt
     * \param location
     * \return
     */
    int hitOutputConnector(wxPoint pt, wxPoint location) const
    {
        int ret = -1;
        pt -= location; // translate to 0,0
        if(_outputs.size() > 0)
        {
            for(size_t i = 0; i < _outputs.size(); i++)
            {
                if(_outputs[i].Contains(pt)) return i;
            }
        }
        return ret;
    }
    //
    wxRect rect() const {
        return _rect;
    }

    void setRect(wxRect r) { _rect = r;}
    unsigned inputCount() const {
        return _inputCount;
    }
    unsigned outputCount()const {
        return  _outputCount;
    }
    void setInputCount(unsigned i)  {
        _inputCount = i;
    }
    void setOutputCount(unsigned i) {
        _outputCount = i;
    }
    void addInput(const wxPoint pt)
    {
        wxRect r(0,0,CONNECTION_SIZE,CONNECTION_SIZE);
        r.SetPosition(pt);
        _inputs.push_back(r);
    }
    void addOutput(const wxPoint pt)
    {
        wxRect r(0,0,CONNECTION_SIZE,CONNECTION_SIZE);
        r.SetPosition(pt);
        _outputs.push_back(r);
    }

    wxRect input(size_t i) const
    {
        if(i < _inputs.size())
        {
            return _inputs[i];
        }
        return wxRect();
    }

    wxRect output(size_t i) const
    {
        if(i < _outputs.size())
        {
            return _outputs[i];
        }
        return wxRect();
    }



};

/*!
 * \brief The NodeType class
 */
class NodeType
{
public:
    typedef std::map<std::string, NodeType *> NodeTypeMap;
    typedef std::map<unsigned, NodeType *>  NodeTypeIdMap;
private:
    static NodeTypeMap _map; // map name to factory
    static NodeTypeIdMap _mapId; // map id to factory
    //
    std::string _name; // type name
    unsigned _id; // hash of type name
    NodeLayout _layout;
    std::vector<Connection> _inputs; // connection spec for  the inputs
    std::vector<Connection> _outputs; // connection spec for the outputs
    //
    static unsigned _msgId;
    //

public:
    /*!
     * \brief NodeType
     * \param s
     */
    NodeType(const std::string &s) : _name(s)
    {
        _map[s] = this;
        std::hash<std::string> hash_obj;
        _id = unsigned(hash_obj(_name)); // determine the hash to get unique id, probably
        _mapId[_id] = this;
    }

    /*!
     * \brief properties
     * \param n
     * \param nodeId
     * \return true if ok
     */
    virtual bool properties(wxWindow * parent,NodeSet &n, unsigned nodeId);
    /*!
     * \brief propertiesUpdated
     * \param n
     * \param nodeId
     *
     */
    virtual void propertiesUpdated(NodeSet &, unsigned ) {}


    /*!
     * \brief onClick - what to do with a click in a runtime display
     */
    virtual void onClick(wxWindow */*parent*/, NodeSet &/*ns*/, unsigned /*nodeId*/) {}
    /*!
     * \brief canConnectInput
     * \param n
     * \param i
     * \return true if connect allowed
     */
    virtual bool canConnectInput(NodePtr &n, size_t i)
    {
        if(i < inputs().size())
        {
            int cn = 0;
            ItemListPtr &il = n->inputs()[i];
            if(il) cn = il->size(); // number of current connections
            if((cn == 0) ||(inputs()[i].mode() & Multiple)) return true;
        }
        return false;
    }

    /*!
     * \brief canConnectOutput
     * \param n
     * \param i
     * \return true if connect allowed
     */
    virtual bool canConnectOutput(NodePtr &n, size_t i)
    {
        if(i < outputs().size())
        {
            return true;
        }
        return false;
    }

    /*!
     * \brief ~NodeType
     */
    virtual ~NodeType()
    {
        _map.erase(_name);
        _mapId.erase(_id);
    }

    /*!
     * \brief map
     * \return
     */
    static NodeTypeMap & map() {
        return  _map;   // map name to factory
    }

    /*!
     * \brief find
     * \param h
     * \return type object
     */
    static NodeType * find(unsigned h)
    {
        auto i = _mapId.find(h);
        if(i != _mapId.end())
        {
            return i->second;
        }
        return nullptr;
    }

    /*!
     * \brief find
     * \param s
     * \return
     */
    static NodeType * find(const std::string &s)
    {
        auto i = _map.find(s);
        if(i != _map.end())
        {
            return i->second;
        }
        return nullptr;
    }

    /*!
     * \brief nodeClass
     * \return  class of node - used to order nodes in UI editor
     */
    virtual const char * nodeClass() const { return "Base";}
    /*!
     * \brief create
     * \param id
     * \param owner
     * \return create
     */
    Node * createNode(unsigned i)
    {
        Node * p = create(i);
        if(p)
        {
            p->setInputs(inputs().size());
            p->setOutputs(outputs().size());
        }
        return p;
    }

    /*!
     * \brief create
     * \param i
     * \return
     */
    virtual Node * create(unsigned i)
    {
        return new Node(i,id());
    }
    /*!
     * \brief setupConnections
     */
    virtual void setupConnections();
    /*!
     * \brief setup
     */
    virtual void setup();

    template <typename T>
    /*!
     * \brief addType
     * \param s
     */
    static T * addType(const std::string &s)
    {
        T * p = new T(s);
        if(p)
        {
            p->setup();
        }
        return p;
    }
    /*!
     * \brief name
     * \return name of object
     */
    const std::string & name() const {
        return _name;
    }
    /*!
     * \brief id
     * \return hash
     */
    unsigned id() const {
        return _id;
    }

    /*!
     * \brief step periodic processing - nodes with no inputs can arm triggering of flows on this function
     * \param ns
     * \param node
     * \return true if this node has self triggered (ie a node with no inputs that now as a value) usually start of a flow
     */

    virtual bool step(NodeSet &/*ns*/, NodePtr & /*node*/) {
        return false;
    }
    /*!
     * \brief trigger - trigger action by a node - expect terminal nodes to use this
     */
    virtual void trigger(NodeSet &, NodePtr & /*node*/ ) { }

    template <typename T>
    /*!
     * \brief setValueData
     * \param topic
     * \param v
     * \param d
     */
    void setValueData(const std::string &topic, const T &v, VALUE &d)
    {
        // set up a data packet with the basic data
        d[DATA_TOPIC] = topic;
        d[DATA_PAYLOAD] = v;
        d[DATA_MSGID] = _msgId++;
    }
    /*!
     * \brief process process data from an input - left to right flow evaluate outputs before inputs
     * \param nodeId this node to process the signal
     * \param id this is the input index
     * \return true on success
     */
    virtual bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data) ;
    /*!
     * \brief post send data to all connected nodes to an output - left to right flow
     * \param nodeId this is the source node
     * \param id this is the output index
     * \return true on success
     */
    virtual bool post(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data );

    /*!
     * \brief calculate - right to left - evaluate all inputs before output
     * \param ns
     * \param nodeId current node
     * \param id input id
     * \param data
     * \return true on success
     */
    virtual bool calculate(NodeSet &ns, unsigned nodeId, unsigned id, VALUE &data );
    /*!
     * \brief processEvaluatedData
     * \param id
     * \param edata
     * \param retData
     */
    virtual void processEvaluatedData(NodeSet &/*ns*/, NodePtr &, unsigned id, VALUE &edata, VALUE &retData )
    {
        retData = edata; // copy last value across by default. - possible to do stats on input data
    }

    /*!
     * \brief evaluate
     * \param ns
     * \param nodeId
     * \param id output to evaluate
     * \param data
     * \return true on success
     */
    virtual bool evaluate(NodeSet &ns, unsigned nodeId, unsigned id,  VALUE &data ); // this is called by a destination node to return a VALUE

    // called before running a node set
    /*!
     * \brief start
     */
    virtual void start(NodeSet &/*ns*/,  NodePtr &/*node*/) {}
    // called when a node set is stopped
    /*!
     * \brief stop
     */
    virtual void stop(NodeSet &/*ns*/,  NodePtr & /*node*/) {}
    //
    /*!
     * \brief inputs
     * \return list of permitted connections
     * Some types of node may have variable numbers of connections - eg message switch/routing nodes
     */
    virtual  std::vector<Connection> & inputs(unsigned /*nodeId */ = 0) {
        return _inputs;
    }
    /*!
     * \brief outputs
     * \return list of permitted connections
     */
    virtual  std::vector<Connection> & outputs(unsigned /*nodeId */ = 0) {
        return _outputs;
    }
    //
    virtual NodeLayout & nodeLayout(unsigned nodeId = 0)
    {
        return _layout;
    }
};






}
#endif // NODETYPE_H
