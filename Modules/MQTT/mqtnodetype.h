#ifndef MQTNODETYPE_H
#define MQTNODETYPE_H
#include <MrlLib/mrllib.h>
#include "NodeFlow/mqttconnection.h"
#include "NodeFlow/nodetype.h"
#include "NodeFlow/nodeset.h"
#include <queue>

namespace NODEFLOW
{

typedef std::queue<VALUE> VALUEQUEUE;
class MqttChan : public NODEFLOW::MqttConnection
{

    std::string _commandSubscription = ""; // the subscription to use to receive commands on
    VALUEQUEUE  * _queue = nullptr;
public:
    MqttChan( const char *id, VALUEQUEUE  * queue = nullptr) : NODEFLOW::MqttConnection(id),_queue(queue) {}
    void setCommandSubscription(const std::string &s) {
        _commandSubscription = s;
    }
    void on_connect(int rc)
    {
        MqttConnection::on_connect(rc) ;
        if(ok() && !_commandSubscription.empty())
        {
            subscribe(nullptr,_commandSubscription.c_str());
        }
    }

    void defaultParse(VALUE &v)
    {
        // not a registered command - so an input packet
        // store in the queue
        if(_queue) _queue->push(v);
    }

};

typedef std::unique_ptr<MqttChan> MqttChanRef;


/*!
 * \brief The MqttInputNodeType class
 * A subscription
 */
class MqttInputNodeType : public NodeType
{
    class MqttInputNode : public NODEFLOW::Node {

        std::queue<VALUE> _in;
        MqttChanRef _mqtt;
        std::string _subscribe;
    public:

        MqttInputNode(unsigned id = 0, size_t type = 0) : Node(id,type) {}

        void process()
        {
            int res = _mqtt->loop(0);     // Keep MQTT connection
            if (res) _mqtt->reconnect(); // reconnect if necessary
        }

        bool waiting() {
            return !_in.empty();
        }
        /*!
         * \brief getNext
         * \param v
         * \return
         */
        bool getNext(VALUE &v)
        {
            if(_in.empty()) return false;
            v = _in.front();
            _in.pop();
            return true;
        }

        void open(NodeSet &ns)
        {
            MRL::PropertyPath p;
            toPath(p);
            std::string it = ns.data().getValue<std::string>(p,"Item");
            _subscribe = ns.data().getValue<std::string>(p,"Subscribe");
            std::string u = ns.data().getValue<std::string>(p,"Username");
            std::string pwd = ns.data().getValue<std::string>(p,"Password");
            std::string h = ns.data().getValue<std::string>(p,"Host");
            int port = ns.data().getValue<int>(p,"Port");

            _mqtt = std::make_unique<MqttChan>(it.c_str(),&_in);
            _mqtt->setCommandSubscription(_subscribe);
            _mqtt->username_pw_set(u.c_str(), pwd.c_str()); // set access
            int rc = _mqtt->connect(h.c_str(), port); // connect
            std::cerr << " Connect " << rc << std::endl;
        }

        void close()
        {
            _mqtt->disconnect();
            _mqtt.reset();
        }

        const std::string & subscribe() {
            return _subscribe;
        }

    };
    virtual const char * nodeClass() const {
        return "MQTT";
    }
    virtual Node * create(unsigned i)   {
        return new MqttInputNode(i,id());
    }
public:
    MqttInputNodeType(const std::string &s) : NodeType(s)
    {

    }

    void setupConnections()
    {
        outputs().resize(1);
        outputs()[0] = Connection("out",Multiple);
    }

    /*!
     * \brief step periodic processing - nodes with no inputs can arm triggering of flows on this function
     * \param ns
     * \param node
     * \return true if this node has self triggered (ie a node with no inputs that now as a value) usually start of a flow
     */

    virtual bool step(NodeSet &/*ns*/, NodePtr &node) {
        if(node)
        {
            node->process();
            MqttInputNode *n = dynamic_cast<MqttInputNode *>(node.get());
            return (n->waiting());
        }
        return false;
    }
    /*!
     * \brief trigger - trigger action by a node - expect terminal nodes to use this
     */
    virtual void trigger(NodeSet &ns, NodePtr &node )
    {
        // read all pending packets received and post on
        if(node && node->enabled())
        {
            node->process();
            MqttInputNode *n = dynamic_cast<MqttInputNode *>(node.get());
            while(n->waiting())
            {
                if(n && n->enabled())
                {
                    VALUE result;
                    while(n->getNext(result))
                    {
                        result[DATA_TOPIC] = n->subscribe();
                        post(ns,node->id(),Output,result);
                    }
                }

            }
        }
    }

    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);

    virtual void start(NodeSet &ns,  NodePtr &node)
    {
        if(node && node->enabled())
        {
            MqttInputNode *o = dynamic_cast<MqttInputNode *>(node.get());
            o->open(ns);
        }

    }
    // called when a node set is stopped
    /*!
     * \brief stop
     */
    virtual void stop(NodeSet &/*ns*/,  NodePtr &node)
    {
        if(node && node->enabled())
        {
            MqttInputNode *o = dynamic_cast<MqttInputNode *>(node.get());
            o->close();
        }
    }


};


/*!
 * \brief The MqttOutputNodeType class
 * output message to a broker
 */
class MqttOutputNodeType : public NodeType
{
    class MqttOutputNode : public NODEFLOW::Node {

        std::string _item;
        std::queue<VALUE> _out;

        MqttChanRef _mqtt;

    public:
        MqttOutputNode(unsigned id = 0, size_t type = 0) : Node(id,type) {}
        virtual void process()
        {
            if(!_out.empty())
            {
                if(_mqtt)
                {
                    std::string v;
                    if(MRL::jsonToString(_out.front(),v))
                    {
                        int res = _mqtt->loop(0);     // Keep MQTT connection
                        if (res) _mqtt->reconnect(); // reconnect if necessary
                        _mqtt->publish(NULL, _item.c_str(), v.length(), v.c_str());
                    }
                }
                _out.pop();
            }
        }

        /*!
         * \brief enqueue
         * \param v
         */
        void enqueue(const VALUE &v)
        {
            _out.push(v);
        }

        void open(NodeSet &ns)
        {

            MRL::PropertyPath p;
            toPath(p);
            _item = ns.data().getValue<std::string>(p,"Item");
            std::string u = ns.data().getValue<std::string>(p,"Username");
            std::string pwd = ns.data().getValue<std::string>(p,"Password");
            std::string h = ns.data().getValue<std::string>(p,"Host");
            int port = ns.data().getValue<int>(p,"Port");
            //
            _mqtt = std::make_unique<MqttChan>(_item.c_str());
            //
            _mqtt->username_pw_set(u.c_str(), pwd.c_str()); // set access
            _mqtt->connect(h.c_str(), port); // connect
        }

        void close()
        {
            _mqtt->disconnect();
            _mqtt.reset();
        }


    };
    virtual const char * nodeClass() const {
        return "MQTT";
    }

    virtual Node * create(unsigned i)
    {
        return new MqttOutputNode(i,id());
    }


public:
    MqttOutputNodeType(const std::string &s) : NodeType(s) {}
    /*!
     * \brief step periodic processing - nodes with no inputs can arm triggering of flows on this function
     * \param ns
     * \param node
     * \return true if this node has self triggered (ie a node with no inputs that now as a value) usually start of a flow
     */

    void setupConnections()
    {
        // setup the inputs
        inputs().resize(1);
        inputs()[0] = Connection("in",Multiple);
    }

    virtual bool step(NodeSet &/*ns*/, NodePtr &node) {
        if(node) node->process();
        return false;
    }

    bool process(NodeSet &ns, unsigned nodeId, unsigned id, const VALUE &data)
    {
        NodePtr &n  = ns.findNode(nodeId);
        if(n && n->enabled())
        {
            MqttOutputNode *o = dynamic_cast<MqttOutputNode *>(n.get());
            o->enqueue(data);
        }
        return true; // just forward
    }

    virtual void start(NodeSet &ns,  NodePtr &node)
    {
        if(node && node->enabled())
        {
            MqttOutputNode *o = dynamic_cast<MqttOutputNode *>(node.get());
            o->open(ns);
        }

    }
    // called when a node set is stopped
    /*!
     * \brief stop
     */
    virtual void stop(NodeSet &/*ns*/,  NodePtr &node)
    {
        if(node && node->enabled())
        {
            MqttOutputNode *o = dynamic_cast<MqttOutputNode *>(node.get());
            o->close();
        }
    }

    // Properties
    virtual void load(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void save(PropertiesEditorDialog &dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void load(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);
    virtual void save(WebProperties *dlg,NodeSet &ns,MRL::PropertyPath p);



};


}
#endif // MQTNODETYPE_H
